#include <string.h>
#include <unistd.h>

#include "../include/constants.h"
#include "../include/redirection.h"
#include "../include/utilities.h"

/* TODO: ADD A CHECK FOR END OF ARGV DIMENSION */
void parse_buffer(char* buff, char** argv){
    argv[0] = strtok(buff, " ");

    for(int i=1; i<ARGV_SIZE; i++){
        if((argv[i] = strtok(NULL, " ")) == NULL){
            break;
        }
        else if(check_redirection_char(argv[i])){
            char* new_stream = strtok(NULL, " ");

            if(new_stream == NULL){
                print_error("Redirection requires an argument!");
                break;
            }

            if(!strcmp(argv[i], "<")){
                change_input_stream(new_stream);  
            }
            else if(!strcmp(argv[i], ">")){ 
                change_output_stream(new_stream, 0);
            }
            else if(!strcmp(argv[i], ">>")){
                change_output_stream(new_stream, 1);
            }
            else if(!strcmp(argv[i], "2>")){     
                change_error_stream(new_stream, 0);    
            }
            else if(!strcmp(argv[i], "<")){
                change_error_stream(new_stream, 1);
            }
            argv[i] = NULL; 
            break;
        }
    }
}