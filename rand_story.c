#include "rand_story.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//any functions you want your main to use


// check the number of command line argument
void argumentCheck(int argc, int want_argc){
    if(argc != want_argc){
        fprintf(stderr,"Wrong number of input arguments\n");
        exit(EXIT_FAILURE);
    }
}

// open file and check whether open it legally
FILE * openFile(const char * name){
    FILE * f = fopen(name,"r");
    if(f == NULL){
        fprintf(stderr,"fail to open %s file\n", name);
        exit(EXIT_FAILURE);
    }
    return f;
}

// reach the end of file and close it successfully
void closeFile(FILE * f){
    if (!feof(f)) {
        fprintf(stderr, "invalid input format\n");
        exit(EXIT_FAILURE);
    }

    if (fclose(f) != 0) {
        fprintf(stderr, "Failed to close the input file!\n");
        exit(EXIT_FAILURE);
    }
}

// Check whether the pointer returned by strdup and strndup was successfully allocated.
void checkPtr(char * ptr){
    if(ptr == NULL){
        fprintf(stderr,"There is something wrong with the operation in strdup/strndup()\n");
        exit(EXIT_FAILURE);
    }
}

// ==============================================================
// ======                    Step 1                   ===========
// ==============================================================

// Verify that the underscores in the same line are paired
size_t underscoreIsPair(const char * words){
    size_t i = 0;
    size_t num = 0;
    while(words[i] != '\n' && words[i] != '\0'){
        if(words[i++] == '_'){
            num++;
        }
    }
    // if the number of '_' is odd, ilegal input file
    if(num % 2 == 1){
        return 0;
    }
    return 1;
}

// find the location of second '_'
char * secondUnderscore(char * first){
    char *second = first;
    second ++;
    while(*second != '\n' && *second != '\0'){
        if(*second == '_'){
            return second;
        }
        second ++;
    }
    return second;
}

// Replace the word and form new sentences
// return the index before which all characters/words have been checked.
size_t replaceWord(char ** words, const char * u1, const char * u2, const char *newword){
    char * new_sentence = NULL;
    size_t front_len = 0;
    size_t new_wordlen = 0;
    size_t back_len = 0;

    // copy the content of old sentence before the first '_'
    new_sentence = strndup(*words, u1 - *words);
    checkPtr(new_sentence);
    front_len = strlen(new_sentence);
    new_wordlen = strlen(newword);

    // add the new word into the new sentence
    new_sentence = realloc(new_sentence, (front_len + new_wordlen + 1)* sizeof(*new_sentence));
    new_sentence = strcat(new_sentence, newword);
    new_sentence[front_len + new_wordlen] = '\0';

    // add the content of old sentence after the second '_'
    back_len = strlen(u2) - 1;
    new_sentence = realloc(new_sentence, (front_len + new_wordlen + back_len + 1)* sizeof(*new_sentence));
    new_sentence = strcat(new_sentence, u2 + 1);
    new_sentence[front_len + new_wordlen + back_len] = '\0';

    free(*words);
    *words = NULL;
    *words = strdup(new_sentence);
    checkPtr(*words);
    free(new_sentence);
    new_sentence = NULL;
    
    // Returns the index of the last letter of the replaced word,
    //to make it easier to continue indexing the following characters of the sentence.
    return front_len + new_wordlen - 1;
}

// print the story.
void printStory(const char* words){
    printf("%s",words);
}

// ------ generalize the main part of step 1 ---- 
void story_step1(FILE * f){
    char * curr = NULL;
    size_t line_size = 0;
    const char * new_word = chooseWord("cat",NULL);

    while(getline(&curr, &line_size, f) != EOF){
        if(!underscoreIsPair(curr)){
            fprintf(stderr,"Ilegal content in the input file\n");
            exit(EXIT_FAILURE);
        }
        char * first = NULL;
        char * second = NULL;
        size_t next_index = 0;
        first = curr;
        // for each sentence, check and replace its word.
        while(*first != '\n' && *first != '\0'){
            if(*first == '_'){
                second = secondUnderscore(first);
                next_index = replaceWord(&curr, first, second, new_word);
                second = NULL;
                first = curr + next_index;
            }
            first ++;
        }
        first = NULL;
        printStory(curr);
        free(curr);
        curr = NULL;
    }
    free(curr);
}


// ==============================================================
// ======                    Step 2                   ===========
// ==============================================================


// check whether each line has a colon, if there is no colon, exit the program.
// otherwise, return the first colon 's index.
size_t hasColon(const char * words){
    // If it's an empty sentence, exit
    if(words == NULL){
        fprintf(stderr,"find an empty line, ilegal!\n");
        exit(EXIT_FAILURE);
    }

    char * p = NULL;
    p = strchr(words, ':');

    // If it's an sentence without any colon, exit.
    if(p == NULL){
        fprintf(stderr,"find a ilegal line without any colon!\n");
        exit(EXIT_FAILURE);
    }
    return p - words;
}

// check whether the category name exists, if exist, return the index in catarray_t.
// otherwise, return -1.   
int categoryExist(const catarray_t * word_array, const char * name){
    if(word_array == NULL|| word_array->n == 0 || word_array->arr == NULL){
        return -1;
    }

    for(size_t i = 0; i < word_array->n; i++){
        if(strcmp(name, word_array->arr[i].name) == 0){
            return i;
        }
    }
    return -1;
}

// insert the new category, return the index of new category in catarray_t.
size_t newCategory(catarray_t * word_array, const char * name){
    if(word_array == NULL){
        word_array = realloc(word_array, sizeof(*word_array));
        word_array->arr = NULL;
        word_array->n = 0;
    }
    size_t index = word_array->n;
    word_array->arr = realloc(word_array->arr, (index + 1) * sizeof(*(word_array->arr)));
    word_array->arr[index].name = strdup(name);
    checkPtr(word_array->arr[index].name);
    word_array->arr[index].words = NULL;
    word_array->arr[index].n_words = 0;
    word_array->n++;
    return index;
}

// check whether the words in specific category exists, if exist, return its index.
// otherwise, return -1.
int hasWords(const catarray_t * word_array, size_t category_index, const char * word){
    size_t word_num = word_array->arr[category_index].n_words;
    if(word_num != 0){
        for(size_t i = 0; i < word_num; i++){
            if(strcmp(word, word_array->arr[category_index].words[i]) == 0){
                return i;
            }
        }
    }
    return -1;
}

// insert the word in specific category
void insertWord(category_t * words_arr, const char * word){
    size_t index = words_arr->n_words;
    words_arr->words = realloc(words_arr->words, (index + 1) * sizeof(*words_arr->words));
    words_arr->words[index] = strdup(word);
    checkPtr(words_arr->words[index]);
    words_arr->n_words++;
}


// Store the contents of the input file in the category array
void setCategory(FILE * f, catarray_t * categories){
    // initialize the object called categories.
    categories->n = 0;
    categories->arr = NULL;
    char * curr = NULL;
    size_t line_size = 0;
    
    while(getline(&curr, &line_size, f) != EOF){
        // if the first colon exist, return its index. otherwise, exit.
        size_t first_colon = hasColon(curr);
        char *category_name = strndup(curr, first_colon);
        checkPtr(category_name);
        char *word = strdup(curr + first_colon + 1);
        checkPtr(word);
        char *n = strchr(word, '\n');
        if(n != NULL){
            *n = '\0';
        }
        int category_index = categoryExist(categories,category_name);
        // if the category don't exist
        if(category_index == -1){
            category_index = newCategory(categories, category_name);
        }

        int word_index = hasWords(categories, category_index, word);
        if(word_index == -1){
            insertWord(&(categories->arr[category_index]), word);
        }

        free(word);
        word = NULL;
        free(category_name);
        category_name = NULL;
        free(curr);
        curr = NULL;
    }
    free(curr);
}

// free the categoty array
void freeCategory(catarray_t * categories){
    for(size_t i = 0; i < categories->n; i++){
        size_t words_num = categories->arr[i].n_words;
        for(size_t j = 0; j < words_num; j++){
            free(categories->arr[i].words[j]);
            categories->arr[i].words[j] = NULL;
        }
        free(categories->arr[i].words);
        categories->arr[i].words = NULL;
        free(categories->arr[i].name);
        categories->arr[i].name = NULL;
    }
    free(categories->arr);
}



// ==============================================================
// ======                    Step 3                   ===========
// ==============================================================

// If the string is a valid integer ( >= 1 or & <= the total number of recoding array), 
// returns the corresponding int. otherwise, returns 0.
size_t isValidInt(const char * num, const size_t words_size){
    if(*num == '\0' || num == NULL){
        return 0;
    }
    size_t word_len = strlen(num);
    // check whether it is integer, if not, return 0.
    for(size_t i = 0; i < word_len; i++){
        if(!isdigit(num[i])){
            return 0;
        }
    }
    // check whether it's within boundary, if it is, return the int.
    // otherwise, it's invalid number, return 0.
    int number = atoi(num);
    return (number >= 1 && number <= words_size)? number : 0;
}

// Deletes the element corresponding to the index
void deleteElement(category_t * arr, size_t index){
    size_t words_num = arr->n_words;
    char * temp = NULL;
    temp = arr->words[index];
    arr->words[index] = arr->words[words_num - 1];
    arr->words[words_num - 1] = temp;
    free(arr->words[words_num - 1]);
    arr->words = realloc(arr->words, (words_num - 1) * sizeof(*(arr->words)));
    arr->n_words--;
}



// Select the word from the recording array(here call it database), 
// erase it from the array, and return it as newword.
char * historicWord(category_t * database, size_t num){
    size_t index = database->n_words - num;
    char * newword = strdup(database->words[index]);
    checkPtr(newword);

    // Delete the indexed word from historic category_t
    deleteElement(&*database, index);
    return newword;
}


// choose a word from that category(dictionary or historic database).
char * getWord(const char * str, catarray_t * category_arr, category_t * history, int reuse){
    size_t history_size = history->n_words;
    char * newword = NULL;
    size_t number = isValidInt(str, history_size);
    if(number > 0){
        return historicWord(history, number);
    }
    else{
        int category_index = categoryExist(category_arr, str);
        // if neither a valid integer nor a valid category name, exit the program.
        if(category_index == -1){
            fprintf(stderr,"There isn't any record about '%s' category!\n", str);
            exit(EXIT_FAILURE);
        }
        // get the category name, and choose a word from it.
        char * category_name = category_arr->arr[category_index].name;
        newword = strdup(chooseWord(category_name, category_arr));
        checkPtr(newword);
        
        // if words can not be reused, delete the word.
        if(reuse == 0){
            int word_index = hasWords(category_arr, category_index, newword);
            deleteElement(&category_arr->arr[category_index], word_index);
        }
    }
    return newword;
}

// free the historic category_t.
void freeHistory(category_t * history){
    if(history != NULL){
        for(size_t i = 0; i < history->n_words; i++){
            free(history->words[i]);
            history->words[i] = NULL;
        }
        free(history->words);
        free(history->name);
    }
}


// ------ generalize the main part of step 3 & 4 ---- 
void tellStory(FILE * f, catarray_t * category_arr, int reuse){
    // initialize the historic words array.
    category_t history = {NULL, NULL, 0};
    history.name = strdup("USED_words");
    checkPtr(history.name);

    char * curr = NULL;
    size_t line_size = 0;

    while(getline(&curr, &line_size, f) != EOF){
        if(!underscoreIsPair(curr)){
            fprintf(stderr,"Ilegal content in the input file\n");
            exit(EXIT_FAILURE);
        }
        char * first = NULL;
        char * second = NULL;
        int next_index = 0;
        first = curr;
        // for each sentence, check and replace its word.
        while(*first != '\n' && *first != '\0'){
            if(*first == '_'){
                second = secondUnderscore(first);
                // 1. get the category name
                char * category_name = NULL;
                category_name = strndup(first + 1, second - first);
                checkPtr(category_name);
                category_name[second - first - 1] = '\0';
                
                // 2. choose a word from that category(dictionary or historic database).
                char * new_word = getWord(category_name, &*category_arr, &history, reuse);

                // 3. replace the word into new sentence.
                next_index = replaceWord(&curr, first, second, new_word);
                
                // 4. update historic datebase
                insertWord(&history, new_word);

                // 5. free these temporary pointers.
                free(new_word);
                new_word = NULL;
                free(category_name);
                category_name = NULL;

                second = NULL;
                first = curr + next_index;
            }
            first ++;
        }
        first = NULL;
        printStory(curr);
        free(curr);
        curr = NULL;
    }
    free(curr);
    freeHistory(&history);
}



// ==============================================================
// ======                    Step 4                   ===========
// ==============================================================

void argumentCheck4(int argc, char ** argv){
    // check whether the number of arguments is 4. otherwise, exit the program.
    argumentCheck(argc, 4);

    if(strcmp("-n", argv[1]) != 0){
        fprintf(stderr,"The first argument is ilegal input\n");
        exit(EXIT_FAILURE);
    }
}

