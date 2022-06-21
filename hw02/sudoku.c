#include "sudoku.h"
#include <stdio.h>
#include <string.h>

static int convert_to_decimal(unsigned int digit);

static unsigned int bitset_add(unsigned int original, int number);

static unsigned int bitset_drop(unsigned int original, int number);

static bool bitset_is_set(unsigned int original, int query);

static bool bitset_is_unique(unsigned int original);

bool eliminate_row(unsigned int sudoku[9][9], int row_index) {
    int seen[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0}, changes = 0;
    for (int i = 0; i < 9; i++) {
        if (bitset_is_unique(sudoku[row_index][i])) { seen[i] = convert_to_decimal(sudoku[row_index][i]); }
    }
    for (int i = 0; i < 9; i++) {
        if (!bitset_is_unique(sudoku[row_index][i])) {
            for (int j = 0; j < 9; j++) {
                if (seen[j] != 0 && bitset_is_set(sudoku[row_index][i], seen[j])) {
                    changes++;
                    sudoku[row_index][i] = bitset_drop(sudoku[row_index][i], seen[j]);
                }
            }
        }
    }
    return changes > 0;
}

bool eliminate_col(unsigned int sudoku[9][9], int col_index) {
    int seen[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0}, changes = 0;
    for (int i = 0; i < 9; i++) {
        if (bitset_is_unique(sudoku[i][col_index])) { seen[i] = convert_to_decimal(sudoku[i][col_index]); }
    }
    for (int i = 0; i < 9; i++) {
        if (!bitset_is_unique(sudoku[i][col_index])) {
            for (int j = 0; j < 9; j++) {
                if (seen[j] != 0 && bitset_is_set(sudoku[i][col_index], seen[j])) {
                    changes++;
                    sudoku[i][col_index] = bitset_drop(sudoku[i][col_index], seen[j]);
                }
            }
        }
    }
    return changes > 0;
}

bool eliminate_box(unsigned int sudoku[9][9], int row_index, int col_index) {
    int seen[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0}, changes = 0, i = 0;
    int max_row = row_index + 3, max_col = col_index + 3;
    for (int row = row_index; row < max_row; row++) {
        for (int col = col_index; col < max_col; col++) {
            if (bitset_is_unique(sudoku[row][col])) {
                seen[i] = convert_to_decimal(sudoku[row][col]);
            }
            i++;
        }
    }
    for (int row = row_index; row < max_row; row++) {
        for (int col = col_index; col < max_col; col++) {
            if (!bitset_is_unique(sudoku[row][col])) {
                for (int j = 0; j < 9; j++) {
                    if (seen[j] != 0 && bitset_is_set(sudoku[row][col], seen[j])) {
                        changes++;
                        sudoku[row][col] = bitset_drop(sudoku[row][col], seen[j]);
                    }
                }
            }
        }
    }
    return changes > 0;
}

bool needs_solving(unsigned int sudoku[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (size_t j = 0; j < 9; j++) {
            if (!bitset_is_unique(sudoku[i][j])) { return true; }
        }
    }
    return false;
}

bool is_valid_array(unsigned int array[9]) {
    for (int i = 0; i < 9; i++) {
        if (array[i] == 0) {
            return false;
        }
        if (bitset_is_unique(array[i])) {
            for (int j = i + 1; j < 9; j++) {
                if (bitset_is_unique(array[j]) && array[i] == array[j]) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool is_valid(unsigned int sudoku[9][9]) {
    for (int i = 0; i < 9; i++) {
        if (i % 3 == 0) {
            for (int j = 0; j < 9; j += 3) {
                unsigned int box[9] = {sudoku[i][j], sudoku[i][j + 1], sudoku[i][j + 2],
                                       sudoku[i + 1][j], sudoku[i + 1][j + 1], sudoku[i + 1][j + 2],
                                       sudoku[i + 2][j], sudoku[i + 2][j + 1], sudoku[i + 2][j + 2]};
                if (!is_valid_array(box)) {
                    return false;
                }
            }
        }
        unsigned int col[9] = {sudoku[i][0], sudoku[i][1], sudoku[i][2], sudoku[i][3], sudoku[i][4], sudoku[i][5],
                               sudoku[i][6], sudoku[i][7], sudoku[i][8]};
        if (!is_valid_array(sudoku[i]) || !is_valid_array(col)) { return false; }
    }
    return true;
}

bool solve(unsigned int sudoku[9][9]) {
    while (needs_solving(sudoku)) {
        int changes = 0;
        for (int i = 0; i < 9; i++) {
            if (eliminate_row(sudoku, i)) { changes++; }
        }
        for (int i = 0; i < 9; i++) {
            if (eliminate_col(sudoku, i)) { changes++; }
        }
        for (int i = 0; i < 9; i += 3) {
            for (int j = 0; j < 9; j += 3) {
                if (eliminate_box(sudoku, i, j)) { changes++; }
            }
        }
        if (!changes) {
            return false;
        }
    }
    return true;
}

#define DIGITS "0123456789"

bool load1(unsigned int sudoku[9][9], const char *input, int length) {
    if (length != 81) {
        fprintf(stderr, "Failed to load input\n");
        return false;
    }
    for (int i = 0; i < 81; i++) {
        if (strchr(DIGITS, input[i]) == NULL) {
            fprintf(stderr, "Failed to load input\n");
            return false;
        }
        long long int number = strchr(DIGITS, input[i]) - DIGITS;
        sudoku[i / 9][i % 9] = (number != 0) ? bitset_add(0, (int) number) : 511;
    }
    return true;
}

#define DIGITS_PLUS "123456789.0"

bool load2(unsigned int sudoku[9][9], const char *input, int length) {
    if (length != 338) {
        fprintf(stderr, "Failed to load input\n");
        return false;
    }
    static char example[] =
            "+-------+-------+-------+\n"
            "| X X X | X X X | X X X |\n"
            "| X X X | X X X | X X X |\n"
            "| X X X | X X X | X X X |\n"
            "+-------+-------+-------+\n"
            "| X X X | X X X | X X X |\n"
            "| X X X | X X X | X X X |\n"
            "| X X X | X X X | X X X |\n"
            "+-------+-------+-------+\n"
            "| X X X | X X X | X X X |\n"
            "| X X X | X X X | X X X |\n"
            "| X X X | X X X | X X X |\n"
            "+-------+-------+-------+\n";

    int cells = 0;
    for (int i = 0; i < 338; i++) {
        if (example[i] != 'X' && input[i] != example[i]) {
            fprintf(stderr, "Failed to load input\n");
            return false;
        }
        if (example[i] == 'X') {
            if (strchr(DIGITS_PLUS, input[i]) == NULL) {
                fprintf(stderr, "Failed to load input\n");
                return false;
            }
            long long int number = strchr(DIGITS_PLUS, input[i]) - DIGITS_PLUS;
            sudoku[cells / 9][cells % 9] = (number == 9 || number == 10) ? 511 : bitset_add(0, (int) number + 1);
            cells++;
        }
    }
    return true;
}

bool load(unsigned int sudoku[9][9]) {
    static char buffer[350] = {0};
    int ch, index = 0, lf_count = 0;
    while ((ch = getchar()) != EOF && index < 350) {
        if (ch == '\n') { lf_count++; }
        if (lf_count == 1 && strchr(DIGITS, buffer[0]) != NULL) {
            return load1(sudoku, buffer, index);
        }
        if (lf_count == 13) {
            buffer[index] = (char) ch;
            return load2(sudoku, buffer, index + 1);
        }
        buffer[index] = (char) ch;
        index++;
    }
    return load1(sudoku, buffer, index);
}

void print(unsigned int sudoku[9][9]) {
    static char string[26] = "+-------+-------+-------+\n";
    for (int i = 0; i < 9; i++) {
        if (i % 3 == 0) { printf("%s", string); }
        for (int j = 0; j < 9; j++) {
            if (j % 3 == 0) { printf("| "); }
            if (sudoku[i][j] == 0) { printf("! "); }
            else bitset_is_unique(sudoku[i][j]) ? printf("%d ", convert_to_decimal(sudoku[i][j])) : printf(". ");
        }
        printf("|\n");
    }
    printf("%s", string);
}

static int convert_to_decimal(unsigned int digit) {
    int count = 0;
    while (digit) {
        digit >>= 1;
        count++;
    }
    return count;
}

static unsigned int bitset_add(unsigned int original, int number) {
    return original | (1 << (number - 1));
}

static unsigned int bitset_drop(unsigned int original, int number) {
    return original ^ (1 << (number - 1));
}

static bool bitset_is_set(unsigned int original, int query) {
    return (original & (1 << (query - 1))) != 0;
}

static bool bitset_is_unique(unsigned int original) {
    return !(original & (original - 1));
}
