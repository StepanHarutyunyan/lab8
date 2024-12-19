#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CHARACTERS 256

typedef struct HuffmanNode {
    char symbol;
    int frequency;
    struct HuffmanNode* left;
    struct HuffmanNode* right;
} HuffmanNode;

HuffmanNode* newNode(char symbol, int frequency) {
    HuffmanNode* node = (HuffmanNode*)malloc(sizeof(HuffmanNode));
    node->symbol = symbol;
    node->frequency = frequency;
    node->left = node->right = NULL;
    return node;
}

int compareNodes(const void* a, const void* b) {
    return ((HuffmanNode*)a)->frequency - ((HuffmanNode*)b)->frequency;
}

HuffmanNode* buildTree(int* frequencies) {
    HuffmanNode* nodes[MAX_CHARACTERS];
    int nodeCount = 0;

    for (int i = 0; i < MAX_CHARACTERS; i++) {
        if (frequencies[i] > 0) {
            nodes[nodeCount++] = newNode(i, frequencies[i]);
        }
    }

    while (nodeCount > 1) {
        qsort(nodes, nodeCount, sizeof(HuffmanNode*), compareNodes);

        HuffmanNode* left = nodes[0];
        HuffmanNode* right = nodes[1];

        HuffmanNode* parent = newNode('\0', left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;

        for (int i = 2; i < nodeCount; i++) {
            nodes[i - 2] = nodes[i];
        }

        nodes[nodeCount - 2] = parent;
        nodeCount--;
    }

    return nodes[0];
}

void generateCodes(HuffmanNode* node, char* code, int length, char codes[MAX_CHARACTERS][MAX_CHARACTERS]) {
    if (node == NULL) {
        return;
    }

    if (node->left == NULL && node->right == NULL) {
        code[length] = '\0';
        strcpy(codes[(unsigned char)node->symbol], code);
        return;
    }

    code[length] = '0';
    generateCodes(node->left, code, length + 1, codes);

    code[length] = '1';
    generateCodes(node->right, code, length + 1, codes);
}

char* encode(const char* input, char codes[MAX_CHARACTERS][MAX_CHARACTERS]) {
    int len = strlen(input);
    char* encoded = (char*)malloc(len * MAX_CHARACTERS * sizeof(char));

    encoded[0] = '\0';
    for (int i = 0; i < len; i++) {
        strcat(encoded, codes[(unsigned char)input[i]]);
    }

    return encoded;
}

char* decode(const char* encoded, HuffmanNode* root) {
    int len = strlen(encoded);
    char* decoded = (char*)malloc(len + 1);
    int index = 0;
    HuffmanNode* current = root;

    for (int i = 0; i < len; i++) {
        current = (encoded[i] == '0') ? current->left : current->right;

        if (current->left == NULL && current->right == NULL) {
            decoded[index++] = current->symbol;
            current = root;
        }
    }
    decoded[index] = '\0';

    return decoded;
}

int main() {
    const char* input = "this is an example for huffman encoding";

    int frequencies[MAX_CHARACTERS] = {0};
    for (int i = 0; input[i] != '\0'; i++) {
        frequencies[(unsigned char)input[i]]++;
    }

    HuffmanNode* root = buildTree(frequencies);

    char codes[MAX_CHARACTERS][MAX_CHARACTERS] = {0};
    char code[MAX_CHARACTERS];
    generateCodes(root, code, 0, codes);

    printf("Huffman Codes:\n");
    for (int i = 0; i < MAX_CHARACTERS; i++) {
        if (frequencies[i] > 0) {
            printf("'%c': %s\n", i, codes[i]);
        }
    }

    char* encoded = encode(input, codes);
    printf("\nEncoded String: %s\n", encoded);

    char* decoded = decode(encoded, root);
    printf("\nDecoded String: %s\n", decoded);

    free(encoded);
    free(decoded);

    return 0;
}
