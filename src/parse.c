#include <string.h>
#include <unistd.h>

#include "../include/constants.h"
#include "../include/redirection.h"
#include "../include/utilities.h"
#include "../include/piping.h"

/* TODO: ADD A CHECK FOR END OF ARGV DIMENSION */

/* returns 1 on success and -1 on error. */
int parse_buffer(char* buff, char** argv){
    argv[0] = strtok(buff, " ");

    for(int i=1; i<ARGV_SIZE; i++){
        argv[i] = strtok(NULL, " ");
        if(argv[i] == NULL){
            return 1;
        }
        else if(check_redirection_char(argv[i])){
            char* new_stream = strtok(NULL, " ");

            if(new_stream == NULL){
                print_error("Redirection requires an argument!\n");
                return -1;
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
            return -1;
        }
        else if(!strcmp(argv[i], "|")){
            char* argv2[ARGV_SIZE];
            argv2[0] = strtok(NULL, " ");

            if(argv2[0] == NULL){
                print_error("Piping requires an argument!\n");
                return -1;
            }

            for(int j=1; j<ARGV_SIZE; j++){
                argv2[j] = strtok(NULL, " ");
                if(argv2[j] == NULL){
                    break;
                }
            }
                            
            argv[i] = NULL;
            execute_piping(argv, argv2);
            return -1; 
        }
    }
}