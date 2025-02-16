#include "strand.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_WORD_LENGTH 25 //Maximum word length ever given by the struct

strand_t *create_strand(FILE *dataFP, FILE *dictFP) {
    //Allocate memory for strand struct
    strand_t *strand = malloc(sizeof(strand_t));
    if (strand == NULL) {
        fprintf(stderr, "Memory Allocation Failed in Strand Struct");
        exit(EXIT_FAILURE);
    }

    //Read the dimensions of the grid and the desired word length
    if (fscanf(dataFP, "%d %d %d", &strand->rows, &strand->cols, &strand->wordLen) != 3) {
        fprintf(stderr, "Error: Failed to read grid dimensions and word length from file.\n");
        exit(EXIT_FAILURE);
    }

    //Allocate memory for the letters grid
    strand->letters = malloc(strand->rows * sizeof(char *));
    if (strand->letters == NULL) {
        fprintf(stderr, "Memory Allocation failed in letter array");
        exit(EXIT_FAILURE);
    }

    //Read the grid letters into the letters array
    for (int i = 0; i < strand->rows; i++) {
        strand->letters[i] = malloc((strand->cols + 1) * sizeof(char)); // Allocate memory for each row with space for null terminator
        if (strand->letters[i] == NULL) {
            fprintf(stderr, "Memory Allocation failed in letter array");
            exit(EXIT_FAILURE);
        }

        //Read letters one by one until the end of the row
        for (int j = 0; j < strand->cols; j++) {
            if (fscanf(dataFP, " %c", &strand->letters[i][j]) != 1) { // Read a character and skip any leading whitespaces
                fprintf(stderr, "Error: Failed to read enough letters for row %d.\n", i + 1);
                exit(EXIT_FAILURE);
            }
        }
        //null termination after reading all letters
        strand->letters[i][strand->cols] = '\0';
    }

    //Count the number of valid words in the dictionary file with the specified word length

    strand->numWords = count_words(dictFP, strand->wordLen, strand);

    //Allocate memory for the words array based on the number of words
    strand->words = (char **) malloc(strand->numWords * sizeof(char *));
    add_word_to_strand(dictFP, strand);

    if (strand->words == NULL) {
        fprintf(stderr, "Memory Allocation failed in words array");
        exit(EXIT_FAILURE);
    }

    // // Initialize words array to NULL
    // for (int i = 0; i < strand->numWords; i++) {
        
    // }

    return strand;
}


//Helper Function


void add_word_to_strand(FILE* dictFP, strand_t *strand) {//Function to count the numnber of words and give it to the user of how many valid words are actually in the grid.
    char word[MAX_WORD_LENGTH]; 
    int count = 0; 

    //count the number of words in the dictionary file with the specified word length
    while (fscanf(dictFP, "%s", word) != EOF) {
        if (strlen(word) == strand->wordLen && is_word_in_strand(strand, word)) {
            strand->words[count] = (char *) malloc((strand->wordLen + 1) * sizeof(char));
            strcpy(strand->words[count], word);
            count++;
        }
    }

    rewind(dictFP); 
}


int count_words(FILE* dictFP, int wordLen, strand_t *strand) {//Function to count the numnber of words and give it to the user of how many valid words are actually in the grid.
    char word[MAX_WORD_LENGTH]; 
    int count = 0; 

    //count the number of words in the dictionary file with the specified word length
    while (fscanf(dictFP, "%s", word) != EOF) {
        if (strlen(word) == wordLen && is_word_in_strand(strand, word)) {
            count++;
        }
    }

    rewind(dictFP); 
    return count;
}

// //Helper Function
// char **generation_words(FILE *dictFP, int wordLen, int numWords) {
//     char **words = (char **)malloc(numWords * sizeof(char *));
//     if (words == NULL) {
//         fprintf(stderr, "Memory Allocation failed in words array");
//         exit(EXIT_FAILURE);
//     }
//     char word[MAX_WORD_LENGTH];
//     int index = 0;
//     while (fscanf(dictFP, "%s", word) != EOF) {
//         if (strlen(word) == wordLen) {
//             char *new_word = strdup(word); //Create a duplicate of the word
//             if (new_word == NULL) {
//                 fprintf(stderr, "Memory Allocation failed in words array");
//                 exit(EXIT_FAILURE);
//             }
//             words[index] = new_word; //Assign the duplicated word to the words array
//             index++;
//             if (index == numWords) {
//                 break;
//             }
//         }
//     }
//     rewind(dictFP);
//     return words;
// }



// REQUIRED
bool is_word_in_strand(strand_t *strand, char *word) {
    //Allocate a 2D array to track visited cells
    bool **visited = (bool **)malloc(strand->rows * sizeof(bool *));
    for (int i = 0; i < strand->rows; i++) {
        visited[i] = (bool *)calloc(strand->cols, sizeof(bool));
    }

    //Iterate through the strand and call the recursive function
    for (int i = 0; i < strand->rows; i++) {
        for (int j = 0; j < strand->cols; j++) {
            if (is_word_in_strand_recursive(strand, word, strlen(word), i, j, 0, visited)) {
                // Free the visited array
                for (int k = 0; k < strand->rows; k++) {
                    free(visited[k]);
                }
                free(visited);
                return true;
            }
        }
    }

    //Free the visited array
    for (int k = 0; k < strand->rows; k++) {
        free(visited[k]);
    }
    free(visited);

    return false;
}


bool is_word_in_strand_recursive(strand_t *strand, char *word, int wordLen, int row, int col, int index, bool **visited){
    if (index == wordLen){
        return true; // Word of the specified length found
    }
    if (row < 0 || row >= strand->rows || col < 0 || col >= strand->cols || strand->letters[row][col] != word[index]){
        return false; // Out of bounds or mismatch, return false
    }

    //Define the offsets for neighboring cells (up, down, left, right, and diagonal)
    int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    //sMark the current position as visited
    visited[row][col] = true;

    //Iterate through neighboring cells recursively
    for(int i = 0; i < 8; i++){
        int newRow = row + dr[i];
        int newCol = col + dc[i];
        if (is_word_in_strand_recursive(strand, word, wordLen, newRow, newCol, index + 1, visited)){
            return true;
        }
    }

    visited[row][col] = false; //unmark the current position before returning
    return false;
}

// REQUIRED
void print_strand_matrix(strand_t * strand) {
    for (int i = 0; i < strand->rows; i++) {
        for (int j = 0; j < strand->cols; j++) {
            printf("+---");
        }
        printf("+\n");

        for (int j = 0; j < strand->cols; j++) {
            printf("| %c ", strand->letters[i][j]);
        }
        printf("|\n");
    }

    for (int j = 0; j < strand->cols; j++) {
        printf("+---");
    }
    printf("+\n");
}




// REQUIRED
void free_strand(strand_t * strand) { //Frees the strand data
    for (int i = 0; i < strand->rows; i++){
        free(strand->letters[i]);
    }
    free(strand->letters);
    for (int i = 0; i < strand->numWords; i++){
        free(strand->words[i]);
    }
    free(strand->words);
    free(strand);   
}