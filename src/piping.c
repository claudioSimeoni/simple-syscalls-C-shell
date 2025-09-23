#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h> // included for exit
#include <stdio.h>

#include "../include/utilities.h"
#include "../include/redirection.h"

int execute_piping(char* argv1[], char* argv2[]){
    int pipefd[2];
    pipe(pipefd);

    int new_pid1 = fork();
    if(new_pid1 < 0){
        print_error("An error occurred during the creation of a new process!\n");
        return -1;
    }
    else if(new_pid1 == 0){
        close(pipefd[0]);
        change_output_stream_fd(pipefd[1]);
        run_executable(argv1);
    }

    int new_pid2 = fork();
    if(new_pid2 < 0){
        print_error("An error occurred during the creation of a new process!\n");
        return -1;
    }
    else if(new_pid2 == 0){
        close(pipefd[1]);
        change_input_stream_fd(pipefd[0]);
        run_executable(argv2);
    }
    
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(new_pid1, NULL, 0);
    waitpid(new_pid2, NULL, 0);

    return 0;
}