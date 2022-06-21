#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <utime.h>
#include "view.h"

bool verbose = false;

size_t split_filename(const char *filename) {
    size_t length = strlen(filename), index = 0;
    while (1) {
        if (filename[index] == '/' && (length - index) < 100) {
            break;
        }
        index++;
    }
    return index;
}

void fill_zeros(char *header, size_t start, size_t end) {
    for (; start <= end; start++) {
        header[start] = '0';
    }
}

size_t calculate_control_sum(const char *header) {
    size_t result = 0, index = 0;
    for (; index < 512; index++) {
        if (index < 148 || 155 < index) {
            result += header[index];
        }
    }
    result += (8 * ' ');
    return result;
}

void fill_text(char *header, char *text, size_t start, size_t end) {
    size_t index = 0;
    for (; index < strlen(text) && start <= end; index++, start++) {
        header[start] = text[index];
    }
}

void fill_number(char *header, size_t number, int start, int end) {
    char octal[20];
    int length = sprintf(octal, "%zo", number);
    fill_zeros(header, start, end);

    int index = end;
    do {
        length--;
        header[index] = octal[length];
        index--;
    } while (length != 0);
}

void fill_header(char *header, char *filename, struct stat st) {
    size_t slash_position = 0;
    if (strlen(filename) >= 100) {
        slash_position = split_filename(filename) + 1;
        char *prefix = malloc(slash_position);
        prefix = memcpy(prefix, filename, slash_position);
        prefix[slash_position - 1] = '\0';
        fill_text(header, prefix, 345, 498);
        free(prefix);
    }

    fill_text(header, filename + slash_position, 0, 98);

    fill_number(header, st.st_mode, 100, 106);
    fill_zeros(header, 100, 103);

    fill_number(header, st.st_uid, 108, 114);
    fill_number(header, st.st_gid, 116, 122);
    fill_number(header, st.st_mtime, 136, 146);

    if (S_ISREG(st.st_mode)) {
        header[156] = '0';
        fill_number(header, st.st_size, 124, 134);
    } else {
        header[156] = '5';
        fill_number(header, 0, 124, 134);
        if (filename[strlen(filename) - 1] != '/' && slash_position == 0) {
            header[strlen(filename)] = '/';
        }
    }

    fill_text(header, "ustar", 257, 261);
    fill_zeros(header, 263, 264);

    struct passwd *pw = getpwuid(st.st_uid);
    if (pw != NULL) {
        fill_text(header, pw->pw_name, 265, 295);
    }

    struct group *gp = getgrgid(st.st_gid);
    if (gp != NULL) {
        fill_text(header, gp->gr_name, 297, 327);
    }

    fill_zeros(header, 329, 335);
    fill_zeros(header, 337, 343);

    size_t n = calculate_control_sum(header);
    header[155] = ' ';
    fill_number(header, n, 148, 153);
}

void stderr_info(const char *filename) {
    if (verbose) {
        fprintf(stderr, "%s\n", filename);
    }
}

int create_archive(int main_fd, char *filename, int *errors) {
    stderr_info(filename);

    struct stat st;
    if (stat(filename, &st) == -1) {
        *errors += 1;
        return perror("stat"), 1;
    }

    if (!(S_ISREG(st.st_mode) || S_ISDIR(st.st_mode))) {
        return 0;
    }

    if (S_ISREG(st.st_mode)) {
        int fd = open(filename, O_RDONLY);
        if (fd == -1) {
            *errors += 1;
            return perror("open"), 1;
        }

        char *header = malloc(512);
        memset(header, '\0', 512);
        fill_header(header, filename, st);
        write(main_fd, header, 512);
        free(header);

        char buffer[512];
        ssize_t bytes = 0;
        while (1) {

            bytes = read(fd, buffer, 512);

            if (bytes == -1) {
                close(fd);
                *errors += 1;
                return perror("read"), 1;
            }

            if (write(main_fd, buffer, bytes) == -1) {
                close(fd);
                *errors += 1;
                return perror("write"), 1;
            }

            if (bytes < 512) {
                if (bytes > 0) {
                    char *padding = malloc(512 - bytes);
                    memset(padding, '\0', 512 - bytes);
                    write(main_fd, padding, 512 - bytes);
                    free(padding);
                }
                break;
            }
        }
        close(fd);

    } else {

        struct dirent **ls;
        int wc_l = scandir(filename, &ls, NULL, alphasort);
        if (wc_l < 0) {
            *errors += 1;
            return perror("scandir"), 1;
        }

        char *header = malloc(512);
        memset(header, '\0', 512);
        fill_header(header, filename, st);
        write(main_fd, header, 512);
        free(header);

        int i = 0;
        while (i < wc_l) {
            char *tmp = ls[i]->d_name;
            if (strcmp(tmp, ".") != 0 && strcmp(tmp, "..") != 0) {

                int size = 3;
                struct view views[size];
                views[0] = view_create(filename, 0);
                if (filename[strlen(filename) - 1] != '/') {
                    views[1] = view_create("/", 0);
                    views[2] = view_create(tmp, 0);
                } else {
                    views[1] = view_create(tmp, 0);
                    size--;
                }

                char *new_filename = view_join(size, views);
                create_archive(main_fd, new_filename, errors);
                free(new_filename);
            }
            free(ls[i]);
            i++;
        }
        free(ls);
    }
    return 0;
}

int extract_archive(char *archive) {
    int main_fd = open(archive, O_RDONLY);
    if (main_fd == -1) {
        return perror("open"), 1;
    }

    char buffer[512];

    char path[100];
    char mode[8];
    char size[12];
    char last_modification[12];
    char control_sum[8];

    while (read(main_fd, buffer, 512) && (buffer[0] != '\0')) {

        char *pointer = buffer;

        memcpy(path, pointer, 100);
        pointer += 100;
        memcpy(mode, pointer, 8);
        pointer += 24;
        memcpy(size, pointer, 12);
        pointer += 12;
        memcpy(last_modification, pointer, 12);
        pointer += 12;
        memcpy(control_sum, pointer, 8);

        stderr_info(path);

        mode[1] = '1';
        char *p1, *p2, *p3, *p4;
        long int decimal_mode = strtol(mode, &p1, 8);
        long int decimal_size = strtol(size, &p2, 8);
        long int decimal_mtime = strtol(last_modification, &p3, 8);
        long int decimal_sum = strtol(control_sum, &p4, 8);

        if ((p1 == mode) || (p2 == size) || (p3 == last_modification) || (p4 == control_sum)) {
            close(main_fd);
            write(2, "Invalid header\n", 15);
            return 1;
        }

        ssize_t s = calculate_control_sum(buffer);
        if (s != decimal_sum) {
            close(main_fd);
            write(2, "Invalid header\n", 15);
            return 1;
        }

        if (buffer[156] == '0') {

            if (access(path, F_OK) == 0) {
                close(main_fd);
                write(2, "File already exist\n", 19);
                return 1;
            }

            unsigned int length = strlen(path), i = 0;
            for (; i < length; i++) {
                if (path[i] == '/') {
                    char *tmp = malloc(i + 1);
                    memcpy(tmp, path, i);
                    tmp[i] = '\0';
                    mkdir(tmp, 0777);
                    free(tmp);
                }
            }

            int new_fd = creat(path, decimal_mode);
            if (new_fd == -1) {
                close(main_fd);
                return perror("creat"), 1;
            }

            char *content = malloc(decimal_size);
            if (read(main_fd, content, decimal_size) == -1) {
                free(content);
                close(new_fd);
                close(main_fd);
                return perror("read"), 1;
            }
            if (write(new_fd, content, decimal_size) == -1) {
                free(content);
                close(new_fd);
                close(main_fd);
                return perror("write"), 1;
            }
            free(content);
            close(new_fd);

        } else { mkdir(path, decimal_mode); }

        if (chmod(path, decimal_mode) == -1) {
            close(main_fd);
            return perror("chmod"), 1;
        }
        struct utimbuf ub;
        ub.actime = 0;
        ub.modtime = decimal_mtime;
        if (utime(path, &ub) == -1) {
            close(main_fd);
            return perror("utime"), 1;
        }
    }
    close(main_fd);
    return 0;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        write(2, "Invalid number of arguments\n", 29);
        return 1;
    }

    bool create = false;

    char *option = argv[1], *archive = argv[2];

    if (strchr(option, 'v') != NULL) {
        verbose = true;
    }

    if (strcmp(option, "c") == 0 || strcmp(option, "cv") == 0 || strcmp(option, "vc") == 0) {
        create = true;

    } else if (strcmp(option, "x") != 0 && strcmp(option, "xv") != 0 && strcmp(option, "vx") != 0) {
        write(2, "Invalid option\n", 16);
        return 1;
    }

    if (create) {
        int fd = creat(archive, 0666);
        if (fd == -1) {
            return perror("open"), 1;
        }
        int errors = 0, i = argc - 1;
        for (; i >= 3; i--) {
            create_archive(fd, argv[i], &errors);
        }
        struct stat st;
        stat(archive, &st);
        if (st.st_size == 0) {
            close(fd);
            write(2, "Empty archive\n", 14);
            return 1;
        }
        char ending[1024];
        memset(ending, '\0', 1024);
        write(fd, ending, 1024);
        close(fd);
        return errors;
    }
    return extract_archive(archive);
}
