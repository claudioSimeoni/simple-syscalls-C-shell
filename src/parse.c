#include <string.h>

#include "../include/constants.h"

/* TODO: ADD A CHECK FOR END OF ARGV DIMENSION */
void parse_buffer(char* buff, char** argv){
    argv[0] = strtok(buff, ' ');

    for(int i=1; i<ARGV_SIZE; i++){
        if((argv[i] = strtok(NULL, ' ')) == NULL){
            /* removing the new line */
            char* new_line = strchr(argv[i - 1], '\n'); 
            *new_line = 0;
            break;
        }
        else if(!strcmp(argv[i], '<')){
            strtok(NULL, ' ');
            argv[i] = NULL;
            
        }
    }
}