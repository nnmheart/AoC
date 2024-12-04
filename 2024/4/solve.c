#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

int row_count;
int col_count;

char* word = "XMAS";

char get_char(char* buff, int x, int y) {
    if (y < 0) return '0';
    if (x < 0) return '0';
    if (y >= row_count) return '0';
    if (x >= col_count) return '0';
    return buff[y * (col_count + 1) + x];
}

int find_match(char* buff, int x, int y) {
    int match = 0;

    // East
    for (int i = 0; i < 4; i++) {
        if (get_char(buff, x + i, y) != word[i]) break;
        if (i == 3) match += 1;
    }
    // West
    for (int i = 0; i < 4; i++) {
        if (get_char(buff, x - i, y) != word[i]) break;
        if (i == 3) match += 1;
    }
    // North
    for (int i = 0; i < 4; i++) {
        if (get_char(buff, x , y + i) != word[i]) break;
        if (i == 3) match += 1;
    }
    // South
    for (int i = 0; i < 4; i++) {
        if (get_char(buff, x , y - i) != word[i]) break;
        if (i == 3) match += 1;
    }
    // North east
    for (int i = 0; i < 4; i++) {
        if (get_char(buff, x + i, y + i) != word[i]) break;
        if (i == 3) match += 1;
    }
    // South west
    for (int i = 0; i < 4; i++) {
        if (get_char(buff, x - i, y - i) != word[i]) break;
        if (i == 3) match += 1;
    }
    // North west
    for (int i = 0; i < 4; i++) {
        if (get_char(buff, x - i, y + i) != word[i]) break;
        if (i == 3) match += 1;
    }
    // South east
    for (int i = 0; i < 4; i++) {
        if (get_char(buff, x + i, y - i) != word[i]) break;
        if (i == 3) match += 1;
    }

    return match;
}

int match_x_mas(char* buff, int x, int y) {
    if (get_char(buff, x+1, y+1) != 'A') return 0;
    
    char tl = get_char(buff, x, y);
    char tr = get_char(buff, x + 2, y);
    char bl = get_char(buff, x, y + 2);
    char br = get_char(buff, x + 2, y + 2);

    if (tl == '0') return 0;
    if (tr == '0') return 0;
    if (bl == '0') return 0;
    if (br == '0') return 0;

    if ((tl != 'M') && (tl != 'S')) return 0;
    if ((tr != 'M') && (tr != 'S')) return 0;
    if ((bl != 'M') && (bl != 'S')) return 0;
    if ((br != 'M') && (br != 'S')) return 0;

    if ((tl == 'M') && (br != 'S')) return 0;
    if ((tl == 'S') && (br != 'M')) return 0;
    if ((tr == 'M') && (bl != 'S')) return 0;
    if ((tr == 'S') && (bl != 'M')) return 0;

    //printf("%c %c\n %c \n%c %c\n--------------\n", tl, tr, get_char(buff, x + 1, y + 1), bl, br);

    return 1;
}

int get_matches(char* buff, int file_size) {
    int part1_matches = 0;
    int part2_matches = 0;
    for (int y = 0; y < row_count; y++) {
        for (int x = 0; x < col_count; x++) {
            int match1 = find_match(buff, x, y);
            int match2 = match_x_mas(buff, x, y);
            part1_matches += match1;
            part2_matches += match2;
        }
    }
    printf("(PART 1) Total matches: %i\n", part1_matches);
    printf("(PART 2) Total matches: %i\n", part2_matches);
}

int main(int argc, char** argv) {
    printf("[INFO] This program works under assumption that line endings are LF and not CRLF.\n");
    if (argc != 2) {
        printf("Only 1 argument allowed.\n");
        return 1;
    }
    FILE* fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        printf("File is invalid.\n");
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    int file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* buff = (char*)malloc(file_size);
    fread(buff, 1, file_size, fp);

    col_count = 0;
    while (buff[col_count] != '\n') {
        col_count++;
    }
    row_count = (file_size + 1) / (col_count + 1);
    printf("Column count: %i\n", col_count);
    printf("Row count: %i\n", row_count);

    int matches = get_matches(buff, file_size);

    return 0;
}