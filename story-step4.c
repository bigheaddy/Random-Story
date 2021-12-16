#include "rand_story.h"


int main(int argc, char** argv){
    int reuse = 1;
    FILE * category_file = NULL;
    FILE * story = NULL;

    if(argc == 4){
        argumentCheck4(argc, argv);
        reuse = 0;
        category_file = openFile(argv[2]);
        story = openFile(argv[3]);
    }
    else{
        argumentCheck(argc, 3);
        category_file = openFile(argv[1]);
        story = openFile(argv[2]);
    }
    

    // set the category array.
    catarray_t categories = {NULL, 0};
    setCategory(category_file, &categories);
    
    // tell the story.
    tellStory(story, &categories, reuse);

    freeCategory(&categories);

    closeFile(category_file);
    closeFile(story);
    
    return EXIT_SUCCESS;
}
