#include "rand_story.h"


int main(int argc, char** argv){
    int reuse = 1;
    argumentCheck(argc, 3);

    FILE * category_file = openFile(argv[1]);
    FILE * story = openFile(argv[2]);

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
