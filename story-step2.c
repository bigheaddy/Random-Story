#include "rand_story.h"


int main(int argc, char** argv){
    
    argumentCheck(argc, 2);

    FILE * f = openFile(argv[1]);

    catarray_t categories = {NULL, 0};
    setCategory(f, &categories);
    
    printWords(&categories);

    freeCategory(&categories);

    closeFile(f);
    
    return EXIT_SUCCESS;
}
