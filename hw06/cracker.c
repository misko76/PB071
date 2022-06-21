#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "md5.h"

bool transformations = false;
FILE *file = NULL;

bool equal_hash(char *string, const char *hash) {
    MD5_CTX md5_ctx;
    MD5_Init(&md5_ctx);
    MD5_Update(&md5_ctx, string, strlen(string) - 1);

    unsigned char md5_hash[16];
    MD5_Final(md5_hash, &md5_ctx);

    char result[100];
    char *pointer = result;

    for (unsigned int i = 0; i < sizeof(md5_hash); ++i) {
        sprintf(pointer, "%02x", md5_hash[i]);
        pointer += 2;
    }

    if (strcmp(result, hash) == 0) {
        return true;
    }
    return false;
}

void check_password(unsigned int start, char *word, const char *hash) {
    if (equal_hash(word, hash)) {
        printf("password found\n%s", word);
        fclose(file);
        exit(0);
    }

    if (transformations) {
        for (unsigned int i = start; i < strlen(word); ++i) {
            switch (word[i]) {

                case 'a':
                    word[i] = '@';
                    check_password(i, word, hash);
                    word[i] = 'a';

                    word[i] = '4';
                    check_password(i, word, hash);
                    word[i] = 'a';
                    break;

                case 'A':
                    word[i] = '@';
                    check_password(i, word, hash);
                    word[i] = 'A';

                    word[i] = '4';
                    check_password(i, word, hash);
                    word[i] = 'A';
                    break;

                case 'b':
                    word[i] = '8';
                    check_password(i, word, hash);
                    word[i] = 'b';
                    break;

                case 'B':
                    word[i] = '8';
                    check_password(i, word, hash);
                    word[i] = 'B';
                    break;

                case 'e':
                    word[i] = '3';
                    check_password(i, word, hash);
                    word[i] = 'e';
                    break;

                case 'E':
                    word[i] = '3';
                    check_password(i, word, hash);
                    word[i] = 'E';
                    break;

                case 'i':
                    word[i] = '!';
                    check_password(i, word, hash);
                    word[i] = 'i';
                    break;

                case 'I':
                    word[i] = '!';
                    check_password(i, word, hash);
                    word[i] = 'I';
                    break;

                case 'l':
                    word[i] = '1';
                    check_password(i, word, hash);
                    word[i] = 'l';
                    break;

                case 'L':
                    word[i] = '1';
                    check_password(i, word, hash);
                    word[i] = 'L';
                    break;

                case 'o':
                    word[i] = '0';
                    check_password(i, word, hash);
                    word[i] = 'o';
                    break;

                case 'O':
                    word[i] = '0';
                    check_password(i, word, hash);
                    word[i] = 'O';
                    break;

                case 's':
                    word[i] = '$';
                    check_password(i, word, hash);
                    word[i] = 's';

                    word[i] = '5';
                    check_password(i, word, hash);
                    word[i] = 's';
                    break;

                case 'S':
                    word[i] = '$';
                    check_password(i, word, hash);
                    word[i] = 'S';

                    word[i] = '5';
                    check_password(i, word, hash);
                    word[i] = 'S';
                    break;

                case 't':
                    word[i] = '7';
                    check_password(i, word, hash);
                    word[i] = 't';
                    break;

                case 'T':
                    word[i] = '7';
                    check_password(i, word, hash);
                    word[i] = 'T';
                    break;
            }
        }
    }
}

bool valid_hash(char *hash) {
    if (strlen(hash) != 32) {
        return false;
    }
    const char valid_characters[] = "ABCDEFabcdef0123456789";
    for (int i = 0; i < 32; ++i) {
        if (strchr(valid_characters, hash[i]) == NULL) {
            return false;
        }
    }
    return true;
}

int main(int argc, char **argv) {
    if (argc != 3 && argc != 4) {
        fprintf(stderr, "Invalid number of arguments\n");
        return 1;
    }
    char *filename = NULL;
    char *hash = NULL;

    if (argc == 3) {
        filename = argv[1];
        hash = argv[2];
    }

    if (argc == 4) {
        if (strcmp("-t", argv[1]) != 0) {
            fprintf(stderr, "Invalid option\n");
            return 1;
        }
        transformations = true;
        filename = argv[2];
        hash = argv[3];
    }

    if (!valid_hash(hash)) {
        fprintf(stderr, "Invalid hash\n");
        return 1;
    }

    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Invalid file\n");
        return 1;
    }

    char buffer[500];
    while (fgets(buffer, 500, file) != NULL) {
        check_password(0, buffer, hash);
    }

    printf("password not found\n");
    fclose(file);
    return 0;
}
