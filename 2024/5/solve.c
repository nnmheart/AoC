#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

const int max_rules = 1176;
const int max_number_per_line = 100;

typedef struct MapItem {
    int num;
    int length;
    int* vals;
} MapItem;

MapItem* rules;
int* line_numbers;
int part1_result = 0;
int part2_result = 0;

void add_rule(int num, int small) {
    for (int i = 0; i < max_rules; i++) {
        MapItem rule = rules[i];
        if (rule.num == num) {
            rule.vals = (int*)realloc(rule.vals, (rule.length + 1) * sizeof(int));
            rule.vals[rule.length] = small;
            rule.length += 1;
            rules[i] = rule;
            return;
        }
        if (rule.num == -1) {
            rule.num = num;
            rule.vals = (int*)malloc(sizeof(int));
            rule.vals[0] = small;
            rule.length = 1;
            rules[i] = rule;
            return;
        }
    }
    printf("Could not hold buffer large enough for new rule.\n");
    exit(EXIT_FAILURE);
}

void initialize_rules() {
    rules = malloc(sizeof(MapItem) * max_rules);
    for (int i = 0; i < max_rules; i++) {
        rules[i].num = -1;
        rules[i].length = 0;
    }
}

int has_item(MapItem rule, int num) {
    for (int i = 0; i < rule.length; i++) {
        if (rule.vals[i] == num) {
            return 1;
        }
    }
    return 0;
}

int is_valid(int num1, int num2) {
    for (int i = 0; i < max_rules; i++) {
        MapItem rule = rules[i];
        if (rule.num == -1) break;
        if (rule.num == num1) {
            if (has_item(rule, num2)) {
                return 1;
            }
        }
        if (rule.num == num2) {
            if (has_item(rule, num1)) {
                return 0;
            }
        }
    }
    return 1;
}

void print_rules() {
    for (int i = 0; i < max_rules; i++) {
        MapItem rule = rules[i];
        if (rule.num == -1) break;
        printf("%i: ", rule.num);
        for (int i2 = 0; i2 < rule.length; i2++) {
            printf("%i ", rule.vals[i2]);
        }
        printf("\n");
    }
}

int read_number(char* buff, int* index) {
    int i = *index;
    int val = 0;

    while ((buff[i] != '|') && (buff[i] != '\n')) {
        val *= 10;
        val += (buff[i] - '0');
        i++;
    }

    *index = (i + 1);
    return val;
}

int read_number2(char* buff, int* index, int file_size) {
    int i = *index;
    int val = 0;

    while ((buff[i] != ',') && (buff[i] != '\n') && (i < file_size)) {
        val *= 10;
        val += (buff[i] - '0');
        i++;
    }

    *index = (i + 1);
    return val;
}

void fix_line(int* line_numbers, int length) {
    for (int i = 0; i < (length - 1); i++) {
        for (int j = (i + 1); j < length; j++) {
            if (!is_valid(line_numbers[i], line_numbers[j])) {
                int temp = line_numbers[i];
                line_numbers[i] = line_numbers[j];
                line_numbers[j] = temp;
            }
        }
    }
    for (int i = 0; i < length; i++) {
        printf("%i ", line_numbers[i]);
    }
    printf("\n");
    int middle_index = (length - 1) / 2;
    part2_result += line_numbers[middle_index];
}

void handle_line2(int* line_numbers, int length) {
    for (int i = 0; i < (length - 1); i++) {
        for (int j = (i + 1); j < length; j++) {
            if (!is_valid(line_numbers[i], line_numbers[j])) {
                fix_line(line_numbers, length);
                return;
            }
        }
    }

    for (int i = 0; i < length; i++) {
        printf("%i ", line_numbers[i]);
    }
    printf("\n");
    int middle_index = (length - 1) / 2;
    part1_result += line_numbers[middle_index];
}

void handle_line(char* buff, int* index, int file_size) {
    int i = 1;
    line_numbers[0] = read_number2(buff, index, file_size);
    while ((buff[*index - 1] != '\n') && (*index < file_size)) {
        line_numbers[i] = read_number2(buff, index, file_size);
        i++;
        if (i > max_number_per_line) {
            printf("Not enough buffer to hold numbers in line.\n");
            exit(EXIT_FAILURE);
        }
    }
    handle_line2(line_numbers, i);
}

void handle_file(char* buff, int file_size) {
    int i = 0;
    while (buff[i] != '\n') {
        int page = read_number(buff, &i);
        int before = read_number(buff, &i);
        add_rule(page, before);
    }
    i++;
    while (i < file_size) {
        handle_line(buff, &i, file_size);
    }
    printf("(PART 1) Total sum: %i\n", part1_result);
    printf("(PART 2) Total sum: %i\n", part2_result);
}

int main(int argc, char** argv) {
    printf("[NOTE] This program works under assumption that line endings are LF.\n");
    if (argc != 2) {
        printf("Only 1 argument supported.\n");
        return 1;
    }

    FILE* fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        printf("File is invalid.\n");
        return 1;
    }

    initialize_rules();
    line_numbers = (int*)malloc(sizeof(int) * max_number_per_line);

    fseek(fp, 0, SEEK_END);
    int file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* buff = (char*)malloc(file_size);
    fread(buff, sizeof(char), file_size, fp);
    handle_file(buff, file_size);
    return 0;
}