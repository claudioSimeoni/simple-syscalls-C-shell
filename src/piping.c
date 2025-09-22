#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h> // included for _exit
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

        if(strchr(argv1[0], '/')){
            execv(argv1[0], argv1);
        }
        else execvp(argv1[0], argv1);

        print_error(argv1[0]);
        print_error(" is not an executable!\n");
        _exit(1);
    }

    int new_pid2 = fork();
    if(new_pid2 < 0){
        print_error("An error occurred during the creation of a new process!\n");
        return -1;
    }
    else if(new_pid2 == 0){
        close(pipefd[1]);
        change_input_stream_fd(pipefd[0]);
        if(strchr(argv2[0], '/')){
            execv(argv2[0], argv2);
        }
        else execvp(argv2[0], argv2);

        print_error(argv2[0]);
        print_error(" is not an executable!\n");
        _exit(1);
    }
    
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(new_pid1, NULL, 0);
    waitpid(new_pid2, NULL, 0);

    return 1;
}