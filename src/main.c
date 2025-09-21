#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h> // included for exit
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
            print_error("An error occurred during the creation of a new process!\n"); 
        }
        else if(new_pid == 0){
            char buffer[BUFFER_SIZE];
            int handle_read = mygetline(STDIN_FILENO, buffer, sizeof(buffer));

            /* handling reading errors */
            if(handle_read <= 0){
                print_error("Error occurred while reading from stdin!\n");
            }

            /* parsing the line read */
            char* new_argv[ARGV_SIZE];
            int handle_parsing = parse_buffer(buffer, new_argv);

            if(handle_parsing == -1) continue;

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
            waitpid(new_pid, NULL, 0);
        }
    }
}