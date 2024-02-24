#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

int* count_character_frequencies(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return NULL;
    }

    int* frequency = (int*)malloc(256 * sizeof(int));
    if (frequency == NULL) {
        printf("Error allocating memory.\n");
        fclose(file);
        return NULL;
    }

    // Initialize all frequencies to 0
    for (int i = 0; i < 256; i++) {
        frequency[i] = 0;
    }

    int c;
    while ((c = fgetc(file)) != EOF) {
        frequency[c]++;
    }

    fclose(file);

    return frequency;
}

// Implement other functions here