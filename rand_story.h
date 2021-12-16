#ifndef __RAND_STORY_H__
#define __RAND_STORY_H__

#include "provided.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>



// check the number of command line argument, if argc != 2, exit the program.
void argumentCheck(int argc, int want_argc);

// open file and check it, if open unsuccessfully, exit program. 
//     otherwise, return FILE *
FILE * openFile(const char * name);

// check whether it reach the end of file and close it successfully
void closeFile(FILE * f);

// Check whether the pointer returned by strdup and strndup was successfully allocated.
void checkPtr(char * ptr);

// ==============================================================
// ======                    Step 1                   ===========
// ==============================================================

// Verify that the underscores in the same line are paired. 
//     if not, ilegal input, exit program.
size_t underscoreIsPair(const char * words);

// find the location of second '_', return it
char * secondUnderscore(char * first);

// Replace the word and form new sentences, 
//    return the index of the end character of new-word in new sentence.(for update purpose)
size_t replaceWord(char ** words, const char * u1, const char * u2, const char *newword);

// print the story.
void printStory(const char* words);

// generalize all the steps in 'step 1'
void story_step1(FILE * f);


// ==============================================================
// ======                    Step 2                   ===========
// ==============================================================

// check whether each line has a colon, if there is no colon, exit the program.
// otherwise, return the first colon 's index.
size_t hasColon(const char * words);

// check whether the category name exists, if exist, return the index in catarray_t.
// otherwise, return -1.
int categoryExist(const catarray_t * wordArray, const char * name);

// insert the new category, return the index of new category in catarray_t.
size_t newCategory(catarray_t * wordArray, const char * name);

// check whether the words in specific category exists, if exist, return its index.
// otherwise, return -1.
int hasWords(const catarray_t * wordArray, size_t category_index, const char * word);

// insert the word in specific category
//void insertWord(catarray_t * wordArray, int category_index, const char * word);
void insertWord(category_t * words_arr, const char * word);

// Store the contents of the input file in the category array
void setCategory(FILE * f, catarray_t * categories);

// free the categoty array
void freeCategory(catarray_t * categories);


// ==============================================================
// ======                    Step 3                   ===========
// ==============================================================


// If the string is a valid integer ( >= 1 or & <= the total number of recoding array), 
// returns the corresponding int. otherwise, returns 0.
size_t isValidInt(const char * num, const size_t words_size);

// Deletes the element corresponding to the index
void deleteElement(category_t * arr, size_t index);

// Select the word from the recording array(here call it database), 
// erase it from the array, and return it as newword.
char * historicWord(category_t * database, size_t num);


// choose a word from that category(dictionary or historic database).
char * getWord(const char * str, catarray_t * category_arr, category_t * history, int reuse);
//const char * chooseWord(char * category, catarray_t * cats);

// ------ generalize the main part of step 3 & 4 ---- 
// tell the story.
void tellStory(FILE * f, catarray_t * category_arr, int reuse);

// free the historic category_t.
void freeHistory(category_t * history);


// ==============================================================
// ======                    Step 4                   ===========
// ==============================================================

// check the number of command line argument, if it has 3 command line arguments
void argumentCheck4(int argc, char ** argv);

// There are not many new functions in step 4, 
// just a few modifications of the previous functions.

#endif
