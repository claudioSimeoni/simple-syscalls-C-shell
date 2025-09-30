
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdio.h>

#include "../include/errors.h"
#include "../include/utilities.h"
#include "../include/constants.h" 
#include "../include/parse.h"
#include "../include/history.h"

int main(int argc, char* argv[]){
    while(1){
        /* writing yellow prompt */
        char* prompt = "\033[33mclaudio'sshell> \033[0m";
        check_syscall(write(STDOUT_FILENO, prompt, strlen(prompt)), "write");
        
        /* getting a new line */
        char buffer[BUFFER_SIZE];
        my_getline(STDIN_FILENO, buffer, sizeof(buffer));

        /* building a pipe for built_in commands that act on the shell itself and not on the single process,
           the reading end of the pipe will be used by the main process after waiting for the child one to finish */
        int pipefd[2];
        check_syscall(pipe(pipefd), "pipe");
        
        /* running the new process */
        int new_pid = check_syscall(fork(), "fork");
        if(new_pid == 0){
            syscall(close(pipefd[0]), "close");
            
            /* building a new history object that will track and update the .history file */
            History history; 
            init_history(&history);
            read_history(&history);

            /* parsing the line read */
            char* new_argv[ARGV_SIZE];
            int handle_parsing = parse_buffer(buffer, new_argv, &history, pipefd);
            if(handle_parsing < 0) _exit(0); /* if the command was already executed exit */

            run_executable(new_argv);
        }
        else{
            check_syscall(waitpid(new_pid, NULL, 0), "waitpid");
            syscall(close(pipefd[1]), "close");

            /* looking for an eventual built_in command */
            my_getline(pipefd[0], buffer, BUFFER_SIZE);
            if(!strcmp(buffer, "exit")){
                _exit(0);
            }
            else if(!strcmp(buffer, "cd")){
                my_getline(pipefd[0], buffer, BUFFER_SIZE);
                check_syscall_noexit(chdir(buffer), "chdir");
            }

            syscall(close(pipefd[0]), "close");
        }
    }
}