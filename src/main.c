#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "../include/utilities.h"
#include "../include/constants.h" 
#include "../include/parse.h"

int main(int argc, char* argv[]){
    while(1){
        char* prompt = "\033[33mshelldelporcodio> \033[0m";
        write(STDOUT_FILENO, prompt, strlen(prompt));

        int new_pid = fork();

        if(new_pid < 0){
            print_error("An error occurred during the creation of a new process!"); 
        }
        else if(new_pid == 0){
            char buffer[BUFFER_SIZE];
            int char_read = mygetline(STDIN_FILENO, buffer, sizeof(buffer));

            /* handling reading errors */
            if(char_read <= 0){
                print_error("Error occurred while reading from stdin!");
            }

            /* parsing the line read */
            char* new_argv[ARGV_SIZE];
            parse_buffer(buffer, new_argv);

            if(strchr(new_argv[0], '/')){
                execv(new_argv[0], new_argv);
            }
            else execvp(new_argv[0], new_argv);

            /* handling error with execvp */
            print_error(new_argv[0]);
            print_error(" is not an executable!\n");
            exit(1);
            /* TODO: understanding if it makes sense to use _exit*/
        }
        else{
            /* TODO: remove this section */
            waitpid(new_pid, NULL, 0);
            printf("\033[32m\nThis is the message from the parent process: pid = %d, new pid = %d\n\033[0m", getpid(), new_pid);
            continue;
        }
    }
}