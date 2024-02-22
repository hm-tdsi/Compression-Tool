#include <stdio.h>
#include "functions.h"

int* count_character_frequencies(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return NULL;
    }

    static int frequency[256] = {0};
    int c;
    while ((c = fgetc(file)) != EOF) {
        frequency[c]++;
    }

    fclose(file);

    return frequency;
}

// Implement other functions here