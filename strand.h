#ifndef STRAND_H
#define STRAND_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// REQUIRED STRUCT - DO NOT REMOVE STRUCT MEMBERS
typedef struct strand
{
    char** words;
    char** letters;
    int rows, cols;
    int numWords;
    int wordLen;

} strand_t;

// REQUIRED FUNCTIONS
strand_t * create_strand(FILE * dataFP, FILE * dictFP);
bool is_word_in_strand(strand_t * strand, char * word);
void print_strand_matrix(strand_t * strand);
void free_strand(strand_t * strand);

//Helps Funcions I added
int count_words(FILE* dictFP, int wordLen, strand_t *strand);
void add_word_to_strand(FILE* dictFP, strand_t *strand);
//char **generation_words(FILE *dictFP, int wordLen, int numWords);
bool is_word_in_strand_recursive(strand_t *strand, char *word,int wordLen, int row, int col, int index, bool **visited);



#endif