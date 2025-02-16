#include "game.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define MAX_WORD_LENGTH 25
#define MAX_NUM_WORDS 100
#define MAX_FILENAME_LENGTH 30

//Opens the strand data file via a user input or command line argument
FILE *open_data_file(int argc, char **argv) {
    FILE *data_file = NULL;
    char filename[MAX_FILENAME_LENGTH + 1]; 
    //Check if a filename is provided in argv[1]
    if (argc > 1) {
        data_file = fopen(argv[1], "r");
        if (data_file != NULL){
            return data_file;
        }
    }
    if (data_file == NULL) {
        printf("Provide strand data filename: ");
        //Read the filename from user input
        if (scanf("%s", filename) != 1) {
            printf("Error reading filename.\n");
            return NULL;
        }

        data_file = fopen(filename, "r");
        
        if (data_file == NULL) {
            printf("Error: Unable to open strand data file.\n");
            return NULL;
        }
    }

    return data_file;
}
//Opens dictionary file
FILE *open_dict_file(int argc, char **argv) {
    FILE *dict_file = NULL;
    const char *dict_filename = DICT_PATH;

    if (argc > 2) {
        dict_filename = argv[2];
    }
    dict_file = fopen(dict_filename, "r");

    if (dict_file == NULL) {
        printf("Error: Unable to open the dictionary file\n");
    }
    return dict_file;
}

//Required
void print_instructions(strand_t *strand) {
    printf("\n  +-- Welcome to Strands! --+\n");
    printf("\nInstructions:\n");
    printf("\tFind all %d %d-letter words\n", strand->numWords, strand->wordLen);
    printf("\tEach word's letters must touch\n");
    printf("\tA letter can be repeated in a word\n");
    printf("\tType a word then press 'Enter'\n");
    printf("\tType 'quit' when done\n");
}

//Play game required function
void play_game(strand_t *strand) {
    int foundWords = 0;
    char user_input[MAX_WORD_LENGTH]; //Allows the user input into an array 
    char already_found[MAX_NUM_WORDS][MAX_WORD_LENGTH];
    int num_already_found = 0;
    bool allWordsFound = false; //Flag to indicate whether all words have been found

    printf("\nGuess a word or 'quit': ");
    while (true) {
        if (fscanf(stdin, "%s", user_input) != 1) {
            printf("Error reading input.\n");
            break;
        }

        if (strcmp(user_input, "quit") == 0) {
            printf("Thanks for playing!\n");
            break;
        }
        if (strlen(user_input) != strand->wordLen) {
            printf("\nPlease enter a %d-letter word.\n", strand->wordLen);
            printf("\nGuess a word or 'quit': ");
            continue;
        }

        bool alreadyFound = false;
        for (int i = 0; i < num_already_found; i++) {
            if (strcmp(already_found[i], user_input) == 0) {
                alreadyFound = true;
                break;
            }
        }
        //Checks if the word was already found through if true then runs this block
        if (alreadyFound) {
            printf("\nYou have already guessed %s!\n", user_input);
            if (!allWordsFound) {
                printf("You have found %d of %d %d-letter words in the strand so far.\n", foundWords, strand->numWords, strand->wordLen);
            }
        } else { 
            if (is_word_in_strand(strand, user_input)) {
                printf("\n%s is a valid word in the strand!\n", user_input);
                foundWords++;
                
                //Check if all words have been found
                if (foundWords == strand->numWords) {
                    allWordsFound = true;
                    printf("You found all %d %d-letter words!\n", foundWords, strand->wordLen);
                    printf("Great job! Thanks for playing!\n");
                    break;
                }
                //If all words are  not found then it runs this function
                if (!allWordsFound) {
                    printf("You have found %d of %d %d-letter words in the strand so far.\n", foundWords, strand->numWords, strand->wordLen);
                }
            } else {
                printf("\n%s is not a valid word in the strand.\n", user_input);
                if (!allWordsFound) {
                    printf("You have found %d of %d %d-letter words in the strand so far.\n", foundWords, strand->numWords, strand->wordLen);
                }
            }
            //If a word has been found adds one to the found count
            strcpy(already_found[num_already_found], user_input);
            num_already_found++;
        }

        printf("\nGuess a word or 'quit': ");
    }
}



