#include <stdio.h>
#include <string.h>
#include "functions.h"

int main() {
    char filename[100];
    //printf("Enter the filename: ");
    //scanf("%s", filename);
    strcpy(filename, "Text/135-0.txt");

    int* frequencies = count_character_frequencies(filename);
    if (frequencies == NULL) {
        return 1;
    }

    for (int i = 0; i < 256; i++) {
        printf("Character '%c' occurs %d times.\n", i, frequencies[i]);
    }

    return 0;
}