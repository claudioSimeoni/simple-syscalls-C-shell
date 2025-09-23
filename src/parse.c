#include <string.h>
#include <unistd.h>

#include "../include/constants.h"
#include "../include/redirection.h"
#include "../include/piping.h"
#include "../include/errors.h"

/* TODO: ADD A CHECK FOR END OF ARGV DIMENSION */

/* parse_buffer analyzes the content of the buffer and, according to its format,
   calls the functions that runs the new process in the proper way. It returns -1
   if other modules have already executed the command and 0 if the command needs 
   to be executed. */
int parse_buffer(char* buff, char** argv){
    argv[0] = strtok(buff, " ");

    for(int i=1; i<ARGV_SIZE; i++){
        argv[i] = strtok(NULL, " ");
        if(argv[i] == NULL){
            return 0;
        }
        else if(check_redirection_char(argv[i])){
            char* new_stream = strtok(NULL, " ");

            if(new_stream == NULL) my_perror("redirection", "argument required");

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
            return 0;
        }
        else if(!strcmp(argv[i], "|")){
            char* argv2[ARGV_SIZE];
            argv2[0] = strtok(NULL, " ");

            if(argv2[0] == NULL) my_perror("piping", "argument required");

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