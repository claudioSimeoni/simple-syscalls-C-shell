
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#include "../include/errors.h"
#include "../include/utilities.h"
#include "../include/constants.h" 
#include "../include/parse.h"

int main(int argc, char* argv[]){
    while(1){
        char* prompt = "\033[33mclaudiosshell> \033[0m";
        check_syscall(write(STDOUT_FILENO, prompt, strlen(prompt)), "write");
        
        /* running the new process */
        int new_pid = check_syscall(fork(), "fork");
        if(new_pid == 0){
            char buffer[BUFFER_SIZE];
            int handle_read = mygetline(STDIN_FILENO, buffer, sizeof(buffer));
    
            if(handle_read <= 0){
                my_perror("mygetline", "EOF reached");
            }
    
            /* parsing the line read */
            char* new_argv[ARGV_SIZE];
            int handle_parsing = parse_buffer(buffer, new_argv);
            if(handle_parsing < 0) continue;

            run_executable(new_argv);
        }
        else{
            waitpid(new_pid, NULL, 0);
        }
    }
}