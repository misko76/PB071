#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "view.h"
#include "ini_parser.h"

static const char *other = "!-.:?_";

static const char *help =
        "\n\tUSAGE: $ ./merge-ini [OPTIONS] INPUT_FILE OUTPUT_FILE\n\n"
        "\t-d N, --max-depth N\tMax depth of includes. Negative means no limit. [default=10]\n"
        "\t-g, --include-guard\tInclude each file only once.\n"
        "\t-r, --report-cycles\tReport cyclic dependency of ini files even if include guard is enabled.\n"
        "\t-h, --help\t\tPrint this help.\n";

bool include_guard = false;
bool report_cycles = false;

bool is_number(char *string);

char *my_readline(FILE *file);

bool valid_format(char *filename, int max_depth, char *prefix, linked_list *files, linked_list *sections);

bool valid_line(char *buffer, int max_depth, char *prefix,
                linked_list *files, linked_list *sections, linked_list *keys);

void write(char *filename, FILE *output_file, char *prefix, linked_list *files);

int is_allowed(int ch);

int is_equals_sign(int ch);

int is_slash(int ch);

bool correct_path(char *path);

char *normalize_path(char *path);

char *get_prefix(char *normalized_path);

int main(int argc, char **argv) {
    int max_depth = 10;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            fprintf(stdout, "%s", help);
            return 0;
        }
    }
    if (argc < 3) {
        fprintf(stderr, "%s", help);
        return 1;
    }
    for (int i = 1; i < argc - 2; i++) {
        if (strcmp(argv[i], "-g") == 0 || strcmp(argv[i], "--include-guard") == 0) {
            include_guard = true;
        } else if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--report-cycles") == 0) {
            report_cycles = true;
        } else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--with-comments") == 0) {
            continue;
        } else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--max-depth") == 0) {
            if (i + 1 < argc - 2 && is_number(argv[i + 1])) {
                max_depth = atoi(argv[i + 1]);
                i++;
            } else {
                fprintf(stderr, "%s", help);
                return 1;
            }
        } else {
            fprintf(stderr, "%s", help);
            return 1;
        }
    }
    int in = argc - 2, out = argc - 1;

    linked_list files = {NULL, NULL, NULL};
    linked_list sections = {NULL, NULL, NULL};
    bool valid = valid_format(argv[in], max_depth, "", &files, &sections);
    clear(&files);
    clear(&sections);

    if (!valid) {
        fprintf(stderr, "Reading input file failed\n");
        return 1;
    }

    FILE *output_file = fopen(argv[out], "w");
    if (!output_file) {
        fprintf(stderr, "Writing output file failed\n");
        return 1;
    }

    write(argv[in], output_file, "", &files);
    fclose(output_file);

    clear(&files);
    return 0;
}

bool is_number(char *string) {
    unsigned int i = 0;
    if (string[0] == '-') { i = 1; }
    for (; i < strlen(string); i++) {
        if (string[i] > '9' || string[i] < '0') { return false; }
    }
    return true;
}

char *my_readline(FILE *file) {
    int counter = 0, size = 1;
    const int buffer_size = 100;
    char *result = NULL, *tmp = malloc(buffer_size * sizeof(char));
    if (tmp == NULL) { return NULL; }

    while (fgets(tmp, buffer_size, file) != NULL) {
        unsigned int length = strlen(tmp);
        char x = tmp[length - 1];

        result = realloc(result, size + buffer_size);
        if (counter == 0) {
            strcpy(result, tmp);
            counter++;
        } else { strcat(result, tmp); }

        if (strchr(&x, '\n') != NULL) { break; }
        size += buffer_size;
    }
    free(tmp);
    return result;
}

bool valid_format(char *filename, int max_depth, char *prefix, linked_list *files, linked_list *sections) {
    char *material = normalize_path(filename);
    node *x = duplicate(files, material);

    if (report_cycles) {
        if (x != NULL && strcmp(x->special_mode, "opened") == 0) {
            free(material);
            return false;
        }
    }

    if (include_guard) {
        if (x != NULL) {
            free(material);
            return true;
        }
    }

    node *y = add(files, material);

    if (max_depth < 0) { return false; }

    FILE *input_file = fopen(filename, "r");
    if (!input_file) { return false; }
    y->special_mode = "opened";

    linked_list keys = {NULL, NULL, NULL};
    keys.special_mode = "NOK";

    char *line;
    while ((line = my_readline(input_file)) != NULL) {

        if (!valid_line(line, max_depth, prefix, files, sections, &keys)) {
            free(line);
            clear(&keys);
            fclose(input_file);
            return false;
        }
        free(line);
    }
    clear(&keys);
    fclose(input_file);
    y->special_mode = "closed";
    return true;
}

bool valid_line(char *buffer, int max_depth, char *prefix,
                linked_list *files, linked_list *sections, linked_list *keys) {

    struct view view = view_create(buffer, 0);
    view = view_trim(view, NULL);
    if (view_empty(view) || *view.begin == ';') { return true; }

    if (*view.begin == '[' && *(view.end - 1) == ']') {
        view = view_remove_prefix(view, 1);
        view = view_remove_suffix(view, 1);
        view = view_trim(view, NULL);

        struct view views[4] = {view_create("[", 0), view_create(prefix, 0), view, view_create("]", 0)};
        char *section = view_join(4, views);

        if (duplicate(sections, section) || !view_all(view, is_allowed)) {
            free(section);
            return false;
        }
        clear(keys);
        add(sections, section);
        keys->special_mode = "OK";
        return true;
    }
    if (view_any(view, is_equals_sign) && *view.begin != '=') {
        view = view_head(&view, '=');
        view = view_trim_back(view, NULL);
        char *key = view_materialize(view);

        if (strcmp(keys->special_mode, "NOK") == 0 || duplicate(keys, key) || !view_all(view, is_allowed)) {
            free(key);
            return false;
        }
        add(keys, key);
        return true;
    }
    if (view_cmp(view_word_front(&view, isspace), view_create(".include", 0)) == 0) {
        view = view_word_back(&view, isspace);
        char *path = view_materialize(view);

        if (!correct_path(path)) {
            free(path);
            return false;
        }

        char *normalized_path = normalize_path(path);
        char *new_prefix = get_prefix(normalized_path);

        bool x = valid_format(path, max_depth - 1, new_prefix, files, sections);

        free(path);
        free(normalized_path);
        free(new_prefix);

        keys->special_mode = "NOK";
        return x;
    }
    return false;
}

void write(char *filename, FILE *output_file, char *prefix, linked_list *files) {
    if (include_guard) {
        char *material = normalize_path(filename);
        if (duplicate(files, material)) {
            free(material);
            return;
        } else { add(files, material); }
    }

    FILE *input_file = fopen(filename, "r");

    char *line;
    while ((line = my_readline(input_file)) != NULL) {

        struct view view;
        view = view_create(line, 0);
        view = view_trim(view, NULL);

        if (*view.begin == '.') {
            view = view_word_back(&view, isspace);
            char *path = view_materialize(view);
            char *normalized_path = normalize_path(path);
            char *new_prefix = get_prefix(normalized_path);

            write(path, output_file, new_prefix, files);
            free(path);
            free(normalized_path);
            free(new_prefix);

        } else if (*view.begin == '[' && *(view.end - 1) == ']') {
            view = view_remove_prefix(view, 1);
            view = view_remove_suffix(view, 1);
            view = view_trim(view, NULL);

            struct view views[4] = {view_create("[", 0), view_create(prefix, 0), view, view_create("]", 0)};
            char *section = view_join(4, views);
            fputs(section, output_file);
            fputc('\n', output_file);
            free(section);

        } else { fputs(line, output_file); }

        free(line);
    }
    fclose(input_file);
    fputc('\n', output_file);
}

int is_allowed(int ch) {
    if (isalpha(ch) || isdigit(ch) || (strchr(other, ch) != NULL)) { return 1; }
    return 0;
}

int is_equals_sign(int ch) {
    if (ch == '=') { return 1; }
    return 0;
}

int is_slash(int ch) {
    if (ch == '/' || ch == '\\') { return 1; }
    return 0;
}

bool correct_path(char *path) {
    if (path[0] == '~' || path[0] == '/' || path[0] == '\\' ||
        strstr(path, ":/") || strstr(path, ":\\")) { return false; }

    unsigned int length = strlen(path);
    if (length <= 4 || strcmp(&path[length - 4], ".ini") != 0) { return false; }

    int count = 0;
    struct view view = view_create(path, 0);
    struct view one_dot = view_create(".", 0);
    struct view two_dots = view_create("..", 0);
    struct view head;

    while (view_length(view)) {
        head = view_word_front(&view, is_slash);
        if (view_cmp(head, two_dots) == 0 && count == 0) { return false; }
        else if (view_cmp(head, two_dots) == 0) { count--; }
        else if (view_cmp(head, one_dot) != 0) { count++; }
        view = view_remove_prefix(view, 1);
    }
    return true;
}

char *normalize_path(char *path) {
    struct view view = view_create(path, 0);
    struct view result = view_create("", 0);
    struct view one_dot = view_create(".", 0);
    struct view two_dots = view_create("..", 0);
    struct view slash = view_create("/", 0);
    struct view backslash = view_create("\\", 0);

    int length = 3;
    struct view head;
    while (!view_empty(view)) {
        head = view_word_front(&view, is_slash);
        if (view_cmp(head, two_dots) == 0) { result = view_tail(&result, *view.begin); }
        else if (view_cmp(head, one_dot) != 0) {

            struct view views[3] = {result, head};
            if (*view.begin == '/') { views[2] = slash; }
            else if (*view.begin == '\\') { views[2] = backslash; }
            else { length--; }

            char *joined = view_join(length, views);
            char new[500];
            strcpy(new, joined);

            free(joined);
            result = view_create(new, 0);
        }
        view = view_remove_prefix(view, 1);
    }
    return view_materialize(result);
}

char *get_prefix(char *normalized_path) {
    unsigned int length = strlen(normalized_path), index = 0;
    char *result = malloc(length);
    for (; index < length - 4; index++) {
        if (normalized_path[index] == '/' || normalized_path[index] == '\\') {
            result[index] = ':';
        } else if (is_allowed(normalized_path[index])) {
            result[index] = normalized_path[index];
        } else {
            result[index] = '?';
        }
    }
    result[index] = ':';
    result[index + 1] = '\0';
    return result;
}
