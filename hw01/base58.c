#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BASE58 "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz"
#define MAXIMUM 0xffffffff

uint64_t power(uint64_t base, uint8_t exponent) {
    uint64_t result = 1;
    while (exponent > 0) {
        if (exponent % 2 == 1) { result *= base; }
        base *= base;
        exponent /= 2;
    }
    return result;
}

void dec_to_base58(uint64_t number) {
    char result[7] = {0};
    for (int i = 5; i >= 0; i--) {
        result[i] = BASE58[number % 58];
        number /= 58;
    }
    printf("%s", result);
}

void dec_to_ascii(uint64_t number) {
    char result[5] = {0};
    for (int i = 3; i >= 0; i--) {
        result[i] = (char) (number % 256);
        number /= 256;
    }
    printf("%s", result);
}

bool encode() {
    uint64_t my_number = 0;
    int ch, count = 0;
    while ((ch = getchar()) != EOF) {
        my_number <<= 8;
        my_number |= ch;
        count++;
        if (count == 4) {
            dec_to_base58(my_number);
            count = 0;
            my_number = 0;
        }
    }
    if (count != 0) {
        my_number <<= 8 * (4 - count);
        dec_to_base58(my_number);
    }
    printf("\n");
    return true;
}

bool decode() {
    uint64_t my_number = 0;
    int ch, count = 0;
    while ((ch = getchar()) != EOF) {
        const char *pointer = strchr(BASE58, ch);
        if (!isspace(ch) && !pointer) { return false; }
        if (!isspace(ch) && pointer) {
            my_number += power(58, 5 - count) * (int) (pointer - BASE58);
            count++;
            if (count == 6 && my_number > MAXIMUM) { return false; }
            if (count == 6) {
                dec_to_ascii(my_number);
                count = 0;
                my_number = 0;
            }
        }
    }
    return (count == 0);
}

int main(int argc, char **argv) {
    if ((argc == 1) || (argc == 2 && !strcmp(argv[1], "-e"))) {
        encode();
    } else if (argc == 2 && !strcmp(argv[1], "-d")) {
        if (!decode()) {
            fprintf(stderr, "Input isn't encoded via Base58!\n");
            return EXIT_FAILURE;
        }
    } else {
        fprintf(stderr, "Invalid switch, use -e or -d\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
