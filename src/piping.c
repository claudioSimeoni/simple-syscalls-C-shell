#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>

#include "../include/errors.h"
#include "../include/redirection.h"
#include "../include/utilities.h"

int execute_piping(char* argv1[], char* argv2[]){
    int pipefd[2];
    check_syscall(pipe(pipefd), "pipe");

    int new_pid1 = check_syscall(fork(), "fork");
    if(new_pid1 == 0){
        check_syscall(close(pipefd[0]), "close");
        change_output_stream_fd(pipefd[1]);
        run_executable(argv1);
    }

    int new_pid2 = check_syscall(fork(), "fork");
    if(new_pid2 == 0){
        check_syscall(close(pipefd[1]), "close");
        change_input_stream_fd(pipefd[0]);
        run_executable(argv2);
    }
    
    check_syscall(close(pipefd[0]), "close");
    check_syscall(close(pipefd[1]), "close");
    check_syscall(waitpid(new_pid1, NULL, 0), "waitpid");
    check_syscall(waitpid(new_pid2, NULL, 0), "waitpid");

    return 0;
}