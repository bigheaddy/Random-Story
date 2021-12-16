#include "rand_story.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
    
    // check whether the argc is valid.
    argumentCheck(argc, 2);

    // open the file.
    FILE * f = openFile(argv[1]);

    // operate the task 1
    story_step1(f);

    // close file.
    closeFile(f);
    
    return EXIT_SUCCESS;
}
