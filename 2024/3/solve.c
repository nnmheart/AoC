#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>

int is_digit(char c) {
    return (c >= '0') && (c <= '9');
}

int parse_number(char* mem, int index, int len) {
    int val = 0;
    for (int i = 0; i < len; i++) {
        val *= 10;
        val += (mem[index + i] - '0');
    }
    return val;
}

int parse_multiplication(char* mem, int i) {
    if (mem[i + 1] != 'u') return 0;
    if (mem[i + 2] != 'l') return 0;
    if (mem[i + 3] != '(') return 0;
    
    if (!is_digit(mem[i + 4])) return 0;
    
    int len_1 = 0;
    int start_indx = i + 4;
    while (is_digit(mem[start_indx + len_1])) {
        len_1 += 1;
    }
    
    if (mem[start_indx + len_1] != ',') return 0;
    
    int start_indx2 = start_indx + len_1 + 1;
    int len_2 = 0;
    while (is_digit(mem[start_indx2 + len_2])) {
        len_2 += 1;
    }
    
    if (mem[start_indx2 + len_2] != ')') return 0;
    
    int num1 = parse_number(mem, start_indx, len_1);
    int num2 = parse_number(mem, start_indx2, len_2);
    
    return num1 * num2;
}

void parse_instructions(char* mem, int size) {
    int mul_enabled = 1;
    int total_mul = 0;
    int part2_total_mul = 0;
    for (int i = 0; i < size - 4; i++) {
        if (mem[i] == 'm') {
            int result = parse_multiplication(mem, i);
            total_mul += result;
            if (mul_enabled) {
                part2_total_mul += result;
            }
            continue;
        } else if (mem[i] == 'd') {
            if (mem[i + 1] != 'o') continue;
            if (mem[i + 2] == '(') {
                if (mem[i + 3] != ')') continue;
                mul_enabled = 1;
            }
            if (mem[i + 2] != 'n') continue;
            if (mem[i + 3] != '\'') continue;
            if (mem[i + 4] != 't') continue;
            if (mem[i + 5] != '(') continue;
            if (mem[i + 6] != ')') continue;
            mul_enabled = 0;
        }
    }
    printf("(PART 1) Total multiplication: %i\n", total_mul);
    printf("(PART 2) Total multiplication: %i\n", part2_total_mul);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Pass only in filename.\n");
        return 1;
    }
    FILE* fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        printf("Invalid file.\n");
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    int filesz = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* buffer = (char*)malloc(sizeof(char) * filesz);
    fread(buffer, sizeof(char), filesz, fp);
    parse_instructions(buffer, filesz);

    return 0;
}