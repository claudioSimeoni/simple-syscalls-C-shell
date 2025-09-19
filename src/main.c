#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "utilities.h"

const int BUFFER_SIZE = 1024;
const int DEFAULT_SIZE = 1024;

int main(int argc, char* argv[]){
    while(1){
        char* prompt = "\033[33mshelldelporcodio> \033[0m";
        write(1, prompt, strlen(prompt));

        int npid = fork();

        if(npid < 0){
            print_error("An error occurred during the creation of a new process!"); 
        }
        else if(npid == 0){
            char buffer[BUFFER_SIZE];
            int char_read = mygetline(0, buffer, sizeof(buffer));

            /* handling error with mygetline */
            if(char_read <= 0){
                print_error("Error occurred while reading from stdin!");
            }

            /* parsing the line read */
            char* new_argv[DEFAULT_SIZE];
            new_argv[0] = strtok(buffer, " ");

            for(int i=1; i<DEFAULT_SIZE; i++){
                if((new_argv[i] = strtok(NULL, " ")) == NULL){
                    char* nl = strchr(new_argv[i - 1], '\n'); 
                    *nl = 0; 
                    break;
                }
            }

            execvp(new_argv[0], new_argv);

            /* handling error with execvp */
            print_error(new_argv[0]);
            print_error(" is not an executable!\n");  
            exit(1); 
        }
        else{
            wait(NULL);
            printf("\033[32mThis is the message from the parent process: pid = %d, new pid = %d\n\033[0m", getpid(), npid);
            continue;
        }
    }
}