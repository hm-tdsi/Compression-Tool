#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "functions.h"

#define NUM_CHARACTERS 256  // or any other value you want

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

// Structure for a node in the Huffman binary tree
struct Node {
    char character;
    int frequency;
    struct Node* left;
    struct Node* right;
};

// Structure for a node in the priority queue
struct PQNode {
    struct Node* node;
    struct PQNode* next;
};

// Structure for the priority queue
struct PriorityQueue {
    struct PQNode* front;
};

// Function to create a new priority queue
struct PriorityQueue* createPriorityQueue(void) {
    struct PriorityQueue* pq = (struct PriorityQueue*)malloc(sizeof(struct PriorityQueue));
    pq->front = NULL;
    return pq;
}

// Function to create a new node
struct Node* createNode(char character, int frequency) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->character = character;
    newNode->frequency = frequency;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Function to check if the priority queue is empty
int isPriorityQueueEmpty(struct PriorityQueue* pq) {
    if (pq->front == NULL) {
        return 1;
    }
    return 0;
}

// Function to add a node to the priority queue
void addToPriorityQueue(struct PriorityQueue* pq, struct Node* node) {
    struct PQNode* newNode = (struct PQNode*)malloc(sizeof(struct PQNode));
    newNode->node = node;
    newNode->next = NULL;

    if (isPriorityQueueEmpty(pq)) {
        pq->front = newNode;
    } else {
        struct PQNode* current = pq->front;
        struct PQNode* prev = NULL;

        while (current != NULL && current->node->frequency < newNode->node->frequency) {
            prev = current;
            current = current->next;
        }

        if (prev == NULL) {
            newNode->next = pq->front;
            pq->front = newNode;
        } else {
            prev->next = newNode;
            newNode->next = current;
        }
    }
}

// Function to get the node with the minimum frequency from the priority queue
struct Node* getMinNode(struct PriorityQueue* pq) {
    if (isPriorityQueueEmpty(pq)) {
        return NULL;
    }

    struct PQNode* current = pq->front;
    struct PQNode* minNode = current;
    struct PQNode* prev = NULL;

    while (current != NULL) {
        if (current->node->frequency < minNode->node->frequency) {
            minNode = current;
            if (current != pq->front) {
                prev = current;
            }
        }
        current = current->next;
    }

    if (prev == NULL) {
        pq->front = minNode->next;
    } else {
        prev->next = minNode->next;
    }

    struct Node* node = minNode->node;
    free(minNode);
    return node;
}

struct Node* buildHuffmanTree(int* frequencies) {
    // Create a priority queue of nodes
    struct PriorityQueue* pq = createPriorityQueue();

    // Here, we assume that frequencies[i] represents the frequency of character i
    // and frequencies[i] > 0 for all valid characters i
    // You can implement your own priority queue or use a library like MinHeap
    // to efficiently build the Huffman tree
    
    // Create a priority queue of nodes based on the frequencies
    printf("Creating priority queue of nodes:\n");
    for (int i = 0; i < NUM_CHARACTERS; i++) {
        if (frequencies[i] > 0) {
            //Note: The character is stored as an integer in the node
            struct Node* newNode = createNode(i, frequencies[i]);
            addToPriorityQueue(pq, newNode);
        }
    }

    // Print the priority queue
    printf("Priority queue of nodes created\n");
    struct PQNode* current = pq->front;
    while (current != NULL) {
        printf("Character: '%c', Frequency: %d\n", current->node->character, current->node->frequency);
        current = current->next;
    }

    // For simplicity, let's assume we have a function called `getMinNode` that returns
    // the node with the minimum frequency from the priority queue

    // Build the Huffman tree by repeatedly combining the two nodes with the minimum frequency
    printf("Building Huffman tree:\n");
    while (!isPriorityQueueEmpty(pq)) {
        // Get the two nodes with the minimum frequency
        struct Node* leftNode = getMinNode(pq);
        struct Node* rightNode = getMinNode(pq);

        // Check if leftNode and rightNode are NULL
        if (leftNode == NULL && rightNode == NULL) {
            printf("Error: leftNode and rightNode should not be NULL at the same time.\n");
            break;
        }
        else if (leftNode == NULL && rightNode != NULL) {
            printf("Error: leftNode should not be NULL.\n");
            break;
        }
        else if (leftNode != NULL && rightNode == NULL) {
            // Add the leftNode back to the priority queue
            // This is the root of the Huffman tree
            addToPriorityQueue(pq, leftNode);
            break;
        }
        else if (leftNode != NULL && rightNode != NULL) {
            // Create a new node with the combined frequency
            printf("Combining nodes with characters '%c' and '%c' and frequencies %d and %d\n", leftNode->character, rightNode->character, leftNode->frequency, rightNode->frequency);
            struct Node* newNode = createNode('\0', leftNode->frequency + rightNode->frequency);
            newNode->left = leftNode;
            newNode->right = rightNode;

            // Add the new node back to the priority queue
            addToPriorityQueue(pq, newNode);
        }
    }

    // Return the root of the Huffman tree
    return getMinNode(pq);
}

// Function to print the Huffman binary tree
void printHuffmanTree(struct Node* root, int depth) {
    if (root == NULL) {
        return;
    }

    // Print the current node
    for (int i = 0; i < depth; i++)
    {
        printf("  ");
    }
    printf("Character: '%c', Frequency: %d\n", root->character, root->frequency);

    // Recursively print the left and right subtrees
    if (root->left != NULL)
    {
        for (int i = 0; i < depth; i++)
        {
            printf("  ");
        }
        printf("Left -->:\n");
        printHuffmanTree(root->left, depth + 1);
    }
    if (root->right != NULL)
    {
        for (int i = 0; i < depth; i++)
        {
            printf("  ");
        }
        printf("Right -->:\n");
        printHuffmanTree(root->right, depth + 1);
    }
}

// Function to generate the prefix-code table using the Huffman tree
void generatePrefixCodeTable(struct Node* root, char* prefixCode, int depth, char** prefixCodeTable) {
    if (root == NULL) {
        return;
    }

    // Check if the current node is a leaf node
    if (root->left == NULL && root->right == NULL) {
        // Assign the prefix code to the character in the prefix code table
        prefixCodeTable[root->character] = strdup(prefixCode);
        //printf("Character '%c': %s\n", root->character, prefixCode);
        return;
    }

    // Append '0' to the prefix code and traverse the left subtree
    prefixCode[depth] = '0';
    prefixCode[depth + 1] = '\0';
    generatePrefixCodeTable(root->left, prefixCode, depth + 1, prefixCodeTable);

    // Append '1' to the prefix code and traverse the right subtree
    prefixCode[depth] = '1';
    prefixCode[depth + 1] = '\0';
    generatePrefixCodeTable(root->right, prefixCode, depth + 1, prefixCodeTable);
}

// Function to print the prefix-code table 
void printPrefixCodeTable(char** prefixCodeTable) {
    printf("Prefix-code table:\n");
    for (int i = 0; i < NUM_CHARACTERS; i++) {
        if (prefixCodeTable[i] != NULL) {
            printf("Character '%c': %s\n", i, prefixCodeTable[i]);
        }
    }
}

// Header section for decoding the compressed file
struct Header {
    int frequencies[NUM_CHARACTERS];
};

// Function to write the header section to the output file
void writeHeader(FILE* outputFile, int* frequencies) {
    struct Header header;
    for (int i = 0; i < 256; i++) {
        if (frequencies[i] > 0) {
            printf("Character '%c' occurs %d times.\n", i, frequencies[i]);
        }
    }
    memcpy(header.frequencies, frequencies, sizeof(int) * NUM_CHARACTERS);
    fwrite(frequencies, sizeof(int), NUM_CHARACTERS, outputFile);

    // Write a separator between the header and the compressed data
    char separator[] = "===SEPARATOR!===";
    fwrite(separator, sizeof(char), strlen(separator), outputFile);
}

int main(void) {
    char filename[100];
    //printf("Enter the filename: ");
    //scanf("%s", filename);
    //strcpy(filename, "Text/135-0.txt");
    //strcpy(filename, "Text/test.txt");
    strcpy(filename, "Text/huffman-test.txt");

    int* frequencies = count_character_frequencies(filename);
    if (frequencies == NULL) {
        return 1;
    }

    for (int i = 0; i < 256; i++) {
        if (frequencies[i] > 0) {
            printf("Character '%c' occurs %d times.\n", i, frequencies[i]);
        }
    }

    // Build the Huffman binary tree
    printf("Building Huffman binary tree:\n");
    struct Node* root = buildHuffmanTree(frequencies);

    // Print the Huffman binary tree
    printf("\nPrinting Huffman binary tree:\n");
    printHuffmanTree(root, 0);

    // Generate the prefix-code table using the Huffman tree
    // This is a table to store the prefix code for each character
    char* prefixCodeTable[NUM_CHARACTERS] = {0};
    // This is a string to store the generated prefix code
    char prefixCode[NUM_CHARACTERS] = {0};
    prefixCode[0] = '\0';
    generatePrefixCodeTable(root, prefixCode, 0, prefixCodeTable);

    // Print the prefix-code table
    printPrefixCodeTable(prefixCodeTable);

    FILE* outputFile = fopen("output", "wb");
    writeHeader(outputFile, frequencies);

    // Free the memory allocated for the frequencies
    free(frequencies);

    // This starts the compression part
    // Open the source text file
    FILE* sourceFile = fopen(filename, "r");
    if (sourceFile == NULL) {
        printf("Failed to open the source file.\n");
        return -1;
    }

    // Encode and write the compressed content to the output file
    printf("Encoding and writing compressed content...\n");
    char ch;
    unsigned char byte = 0;
    int bitCount = 0;
    while ((ch = fgetc(sourceFile)) != EOF) {
        char* prefix = prefixCodeTable[ch];
        int prefixLength = strlen(prefix);
        for (int i = 0; i < prefixLength; i++) {
            if (prefix[i] == '1') {
                byte |= (1 << (7 - bitCount));
            }
            bitCount++;
            if (bitCount == 8) {
                fwrite(&byte, sizeof(unsigned char), 1, outputFile);
                printf("Byte: %x\n", byte);
                byte = 0;
                bitCount = 0;
            }
        }
    }

    // Write the remaining bits if any
    if (bitCount > 0) {
        fwrite(&byte, sizeof(unsigned char), 1, outputFile);
        printf("Byte: %x\n", byte);
    }

    // Close the source file and output file
    fclose(sourceFile);

    fclose(outputFile);
    // This ends the compression part






    // This starts the decompression part
    // Open the output file
    printf("Opening the output file...\n");
    outputFile = fopen("output", "rb");
    if (outputFile == NULL) {
        printf("Failed to open the output file.\n");
        return -1;
    }

    // Read the header section from the output file
    struct Header header;
    fread(header.frequencies, sizeof(int), NUM_CHARACTERS, outputFile);

    // Print the frequencies of each character
    printf("Frequencies of each character:\n");
    for (int i = 0; i < NUM_CHARACTERS; i++) {
        if (header.frequencies[i] > 0) {
            printf("Character '%c': %d\n", i, header.frequencies[i]);
        }
    }

    // Close the output file
    fclose(outputFile);



    return 0;
}
