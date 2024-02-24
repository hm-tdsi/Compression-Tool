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
void printHuffmanTree(struct Node* root) {
    if (root == NULL) {
        return;
    }

    // Print the current node
    printf("Character: '%c', Frequency: %d\n", root->character, root->frequency);

    // Recursively print the left and right subtrees
    printf("Left subtree:\n");
    printHuffmanTree(root->left);
    printf("Right subtree:\n");
    printHuffmanTree(root->right);
}

int main(void) {
    char filename[100];
    //printf("Enter the filename: ");
    //scanf("%s", filename);
    //strcpy(filename, "Text/135-0.txt");
    strcpy(filename, "Text/test.txt");
    //strcpy(filename, "Text/huffman-test.txt");

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

    // TODO: Perform further operations with the Huffman binary tree
    // Print the Huffman binary tree
    printf("\nPrinting Huffman binary tree:\n");
    printHuffmanTree(root);

    // Free the memory allocated for the frequencies
    free(frequencies);
    return 0;
}
