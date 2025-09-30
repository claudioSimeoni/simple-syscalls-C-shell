#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "../include/constants.h"
#include "../include/redirection.h"
#include "../include/piping.h"
#include "../include/errors.h"
#include "../include/built_in.h"
#include "../include/history.h"

/* this getline differently from the libc one removes the new line 
   since it is useless for our shell purposes */
ssize_t my_getline(int fd, char* buff, size_t bytes){
    ssize_t ptr = 0;
    ssize_t handle_byte = 0;
    
    while(ptr < (ssize_t)(bytes - 1)){
        handle_byte = check_syscall(read(fd, buff + ptr, 1), "read");
        if(handle_byte == 0) break;

        if(buff[ptr] == '\n'){
            buff[ptr] = 0;
            return ptr;
        }
        ptr++;
    }
    
    buff[ptr] = 0;
    return ptr;
}

/* TODO: ADD A CHECK FOR END OF ARGV DIMENSION */

/* parse_buffer is the core of the project, it analyzes the content of the buffer 
   and, according to its format, calls the functions that runs the new process in 
   the proper way. It returns -1 if other modules have already executed the command 
   and 0 if the command needs to be executed by the main module. */
int parse_buffer(char* buff, char** argv, History* history, int pipefd[]){
    if(buff[0] == 0) _exit(0);
    
    /* if buff has at least one char it adds buff to the history */
    push_history(history, buff);
    write_history(history);

    argv[0] = strtok(buff, " ");


    /* if argv[0] corresponds to one of the built_in commands then it concludes the parsing 
       and sends the command to the built_in module's function `handle_command` */
    if(check_command(argv[0])){
        for(int i=1; i<ARGV_SIZE; i++){
            argv[i] = strtok(NULL, " ");
            if(argv[i] == NULL){
                return handle_command(argv, history, pipefd);
            }
        }
    }

    for(int i=1; i<ARGV_SIZE; i++){
        argv[i] = strtok(NULL, " ");

        if(argv[i] == NULL){
            return 0;
        }
        else if(check_redirection_char(argv[i])){
            char* new_stream = strtok(NULL, " ");

            if(new_stream == NULL) my_perror("redirection", "argument required");

            /* once a redirection token is detected it calls the respective function to handle 
               that redirection */
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

            /* once the piping token is detected it keeps parsing from that on and sends the two commands 
               to the piping module that will run both processes */
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