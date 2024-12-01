#include<stdlib.h>
#include<stdio.h>
#include<malloc.h>

// Code as whole is inefficient.
// But I don't really care, I just want to code without thinking about what I am doing.

int read_number(FILE* fp, int* dstVal) {
    if (feof(fp)) return 1;
    char chars[10] = {0};
    for (int i = 0; i < 10; i++) {
        char c = fgetc(fp);
        if ((c == ' ') && (i == 0)) {
            i -= 1;
            continue;
        }
        chars[i] = c;
        if (feof(fp)) {
            chars[i] = 0x00;
            break;
        }
        if ((c == ' ') | (c == '\n')) {
            chars[i] = 0x00;
            break;
        }
        if (i == 9) {
            printf("Not enough buffer to hold current number.\n");
            exit(1);
        }
    }
    int len = 0;
    int val = 0;
    while (chars[len] != 0) {
        val *= 10;
        val += (chars[len] - '0');
        len++;
    }
    *dstVal = val;
    return 0;
}

void match_pair(int* pairs, int* distances, int total_pairs, int current_pair) {
    int smallest_left = pairs[2 * 0];
    int smallest_right = pairs[2 * 0 + 1];
    int index_small_left = 0;
    int index_small_right = 0;

    for (int i = 0; i < total_pairs; i++) {
        if (pairs[2 * i] < smallest_left) {
            smallest_left = pairs[2 * i];
            index_small_left = i;
        }
        if (pairs[2 * i + 1] < smallest_right) {
            smallest_right = pairs[2 * i + 1];
            index_small_right = i;
        }
    }

    if (smallest_right > smallest_left) {
        distances[current_pair] = smallest_right - smallest_left;
    } else {
        distances[current_pair] = smallest_left - smallest_right;
    }
    //printf("%i and %i with distance of %i\n", smallest_left, smallest_right, distances[current_pair]);
    pairs[2 * index_small_left] = 0x7FFFFFFF;
    pairs[2 * index_small_right + 1] = 0x7FFFFFFF;
}

int find_similarity_score(int* simmap, int* pairs, int pair_count) {
    int elnum = 0; // Element count in "simmap"

    for (int left = 0; left < pair_count; left++) {
        int number = pairs[2 * left];
        
        int simindex = 0;
        int simfound = 0;

        for (int i = 0; i < elnum; i++) {
            if (simmap[2 * i] == number) {
                simfound = 1;
                simindex = i;
                //printf("Found %i at %i\n", number, simindex);
                break;
            }
        }
        if (!simfound) {
            simindex = elnum;
            elnum++;
            simmap[2 * simindex] = number;
            simmap[2 * simindex + 1] = 0;
            //printf("Created %i at %i\n", number, simindex);
        }

        int appears = 0;
        for (int right = 0; right < pair_count; right++) {
            if (pairs[2 * right + 1] == number) {
                appears++;
            }
        }
        //printf("%i appears %i times.\n", number, appears);
        simmap[2 * simindex + 1] += (number * appears);
        //printf("%i similarity score is now %i\n", number, simmap[2 * simindex + 1]);
    }

    int score = 0;
    for (int i = 0; i < elnum; i++) {
        int num = simmap[2 * i];
        int num_score = simmap[2 * i + 1];
        //printf("%i has score %i\n", num, num_score);
        score += num_score;
    }
    return score;
}


int main(int argc, char** argv) {
    if (argc != 2) {
        printf("You passed no arguments, pass in filename, or you passed too many.\n");
        return 1;
    }
    char* filename = argv[1];
    
    FILE* fp = fopen(filename, "r");

    int pair_count = 0;
    printf("Reading to get pair count...\n");
    while (1) {
        int sample;
        if (read_number(fp, &sample) != 0) {
            break;
        }
        read_number(fp, &sample);
        pair_count++;
    }

    int* pairs = malloc((pair_count + 1) * 2 * sizeof(int));
    int* distances = malloc((pair_count + 1) * sizeof(int));
    int* similarity_map = malloc((pair_count + 1) * 2 * sizeof(int));

    fseek(fp, SEEK_SET, 0);
    printf("Pair count: %i\n", pair_count);
    for (int i = 0; i < pair_count; i++) {
        read_number(fp, &pairs[2 * i]);
        read_number(fp, &pairs[2 * i + 1]);
    }
    pairs[2 * pair_count] = 0x00;
    pairs[2 * pair_count + 1] = 0x00;

    int similarity_score = find_similarity_score(similarity_map, pairs, pair_count);

    for (int i = 0; i < pair_count; i++) {
        match_pair(pairs, distances, pair_count, i);
    }
    int distance_sum = 0;
    for (int i = 0; i < pair_count; i++) {
        distance_sum += distances[i];
    }
    printf("-------------------------------------------\n");
    printf("(PART ONE) Total distance: %i\n", distance_sum);
    printf("(PART TWO) Similarity score: %i\n", similarity_score);
}