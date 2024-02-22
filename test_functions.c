#include <stdio.h>
#include "functions.h"

void assert_equal(int a, int b, const char* test_name) {
    if (a != b) {
        printf("%s failed: %d is not equal to %d\n", test_name, a, b);
    } else {
        printf("%s passed\n", test_name);
    }
}

void test_count_character_frequencies() {
    int* frequencies = count_character_frequencies("Text/test.txt");
    assert_equal(frequencies['a'], 3, "Frequency of 'a'");
    assert_equal(frequencies['b'], 2, "Frequency of 'b'");
    assert_equal(frequencies['c'], 1, "Frequency of 'c'");
}

// Define other tests here

int main() {
    test_count_character_frequencies();
    // Call other tests here
    return 0;
}