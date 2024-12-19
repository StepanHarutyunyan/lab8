#include <stdio.h>
#include <string.h>

#define WINDOW_SIZE 4096
#define MAX_ENCODED_LENGTH 1024  // Set a limit for encoding

// Structure to store the match details
typedef struct {
    int offset;
    int length;
    char next_char;
} Match;

// Function to find the longest match in the sliding window
Match findLongestMatch(const char* data, int pos, int window_size) {
    Match best_match = {0, 0, data[pos]}; // Start with the current character
    int end = (pos + window_size < strlen(data)) ? (pos + window_size) : strlen(data);

    // Look for matches in the sliding window
    for (int i = (pos - window_size >= 0 ? pos - window_size : 0); i < pos; i++) {
        int length = 0;
        // Compare the strings starting from i and pos
        while (pos + length < end && data[i + length] == data[pos + length]) {
            length++;
        }
        if (length > best_match.length) {
            best_match.offset = pos - i;
            best_match.length = length;
            best_match.next_char = data[pos + length]; // The next character after the match
        }
    }
    return best_match;
}

// Function to encode the input string using LZ77 algorithm
void encode(const char* input, char* encoded) {
    int pos = 0;
    int input_length = strlen(input);
    int index = 0;  // Position in the encoded output

    while (pos < input_length) {
        Match match = findLongestMatch(input, pos, WINDOW_SIZE);

        // Format the match into a string: (offset, length, next_char)
        index += sprintf(encoded + index, "(%d, %d, '%c') ", match.offset, match.length, match.next_char);

        // Move the position forward by match length + 1 (to move past the next_char)
        pos += match.length + 1; // Move to the next character after the match
    }
}

// Function to decode the LZ77 encoded string
void decode(const char* encoded, char* decoded) {
    int decoded_index = 0;  // Position in the decoded output
    const char* cursor = encoded;

    while (*cursor != '\0') {
        int offset, length;
        char next_char;

        // Parse the match format "(offset, length, next_char)"
        if (sscanf(cursor, "(%d, %d, '%c')", &offset, &length, &next_char) == 3) {
            // Copy the matched string (back-reference)
            if (offset > 0) {
                int start = decoded_index - offset;
                for (int i = 0; i < length; i++) {
                    decoded[decoded_index++] = decoded[start + i];
                }
            }
            // Add the next character after the match
            decoded[decoded_index++] = next_char;
        }

        // Move the cursor to the next match
        while (*cursor != ' ' && *cursor != '\0') cursor++;
        if (*cursor == ' ') cursor++;  // Skip the space separator
    }

    decoded[decoded_index] = '\0';  // Null-terminate the decoded string
}

int main() {
    const char* input = "ABABABABA";
    printf("Input: %s\n", input);

    // Allocate enough space for encoding
    char encoded[MAX_ENCODED_LENGTH];
    encode(input, encoded);
    printf("Encoded: %s\n", encoded);

    // Allocate enough space for decoding
    char decoded[MAX_ENCODED_LENGTH];
    decode(encoded, decoded);
    printf("Decoded: %s\n", decoded);

    return 0;
}
