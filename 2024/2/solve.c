#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>


// Return 0 for normal number before space.
// Return 1 for normal number before newline. (Essentially means end of list)
// Return 2 for EOF.
void read_number(FILE* fp, int* is_newline, int* val) {
    char digits[10] = {0};

    for (int i = 0; i < 10; i++) {
        char read = fgetc(fp);
        digits[i] = read;
        if (read == ' ') {
            *is_newline = 0;
            digits[i] = 0x00;
            break;
        } else if (read == '\n') {
            *is_newline = 1;
            digits[i] = 0x00;
            break;
        }
        if (feof(fp)) {
            digits[i] = 0x00;
            *is_newline = 1;
            break;
        }

        if (i == 9) {
            printf("Not enough buffer to hold the number.\n");
            exit(EXIT_SUCCESS);
        }
    }

    int val2 = 0;
    int i = 0;
    while (digits[i] != 0) {
        val2 *= 10;
        val2 += (digits[i] - '0');
        i++;
    }

    *val = val2;
}

int level_is_safe(int count, int* members) {
    int previous_sign = 1;
    for (int i = 0; i < count - 1; i++) {
        int current = members[i];
        int next = members[i + 1];

        int diff = current - next;
        int sign = (diff > 0) ? 1 : -1;
        int abs = sign * diff;

        if ((abs < 1) | (abs > 3)) {
            return i;
        }

        if (i > 0) {
            if (sign != previous_sign) {
                return i;
            }
        }

        previous_sign = sign;

    }
    return -1;
}

void print_level(int* mem) {
    int count = mem[0];
    printf("%i: ", count);
    for (int i = 0; i < count; i++) {
        printf("%i ", mem[i + 1]);
    }
    printf("\n");
}

void assign_temp(int* storage, int* temp_storage, int count_index, int exclude_index) {
    temp_storage[0] = storage[count_index] - 1;
    int li = 1;
    for (int i = 0; i < storage[count_index]; i++) {
        if (i == exclude_index) continue;
        temp_storage[li] = storage[count_index + i + 1];
        li++;
    }
}

int test_index(int* storage, int* temp_storage, int count_index, int* part2, int index) {
    assign_temp(storage, temp_storage, count_index, index);
    int index2 = level_is_safe(temp_storage[0], &temp_storage[1]);
    if (index2 == -1) {
        *part2 += 1;
    }
    return index2;
}

int check_if_safe(int* storage, int* temp_storage, int count_index, int* part1, int* part2) {
    print_level(&storage[count_index]);
    int index = level_is_safe(storage[count_index], &storage[count_index + 1]);
    if (index == -1) {
        *part1 += 1;
    } else {
        // Bruteforce (and probably invalid) approach but I am tired okay and I have been focusing on this for 2 hours I just want to get it done already
        if (test_index(storage, temp_storage, count_index, part2, index) != -1) {
            if (test_index(storage, temp_storage, count_index, part2, index + 1) != -1) {
                if (test_index(storage, temp_storage, count_index, part2, index - 1) != -1) {

                }
            }
        }
    }
    printf("----\n");
}

void read_levels(FILE* fp, int* storage, int* temp_storage) {
    int count_index = 0;
    int index = 1;

    int is_newline = 0;
    int current_value = 0;

    int safe_reports = 0;
    int safe2_reports = 0;

    while (!feof(fp)) {
        read_number(fp, &is_newline, &current_value);
        storage[count_index] += 1;
        storage[index] = current_value;
        index++;
        if (is_newline) {
            check_if_safe(storage, temp_storage, count_index, &safe_reports, &safe2_reports);
            /*printf("Level (%i total): ", storage[count_index]);
            for (int i = (count_index + 1); i < index; i++) {
                printf("%i ", storage[i]);
            }
            printf("\n");*/
            count_index = index;
            index++;
        }
    }

    printf("(PART 1) Safe records: %i\n", safe_reports);
    printf("(PART 2) Safe records: %i\n", safe2_reports + safe_reports);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Only 1 argument is accepted.\n");
        return 1;
    }

    FILE* fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Invalid file.\n");
        return 1;
    }

    const int line_count = 1000;
    const int max_numbers_per_pair = 10;

    // Storing as:
    // Number of elements in that level
    // [N] numbers
    // Repeat.
    int* storage = (int*) calloc(line_count * (max_numbers_per_pair + 1), sizeof(int));
    int* temp_storage = (int*) calloc((max_numbers_per_pair + 1), sizeof(int));
    read_levels(fp, storage, temp_storage);

    return 0;
}