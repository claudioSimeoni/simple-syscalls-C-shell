#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "../include/errors.h"

int check_redirection_char(char* arg){
    if(!strcmp(arg, "<") || !strcmp(arg, ">") || !strcmp(arg, ">>") 
                         || !strcmp(arg, "2>") || !strcmp(arg, "2>>")) return 1;
    return 0;
}

void change_input_stream(char* new_stream){
    int new_fd = check_syscall(open(new_stream, O_RDONLY | O_CREAT, 0644), "open");
    check_syscall(dup2(new_fd, STDIN_FILENO), "dup2"); 
    check_syscall(close(new_fd), "close");
}

void change_output_stream(char* new_stream, int append){
    int new_fd = check_syscall(open(new_stream, O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC), 0644), "open");
    check_syscall(dup2(new_fd, STDOUT_FILENO), "dup2"); 
    check_syscall(close(new_fd), "close");
}

void change_error_stream(char* new_stream, int append){
    int new_fd = check_syscall(open(new_stream, O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC), 0644), "open");
    check_syscall(dup2(new_fd, STDERR_FILENO), "dup2"); 
    check_syscall(close(new_fd), "close");
}

void change_input_stream_fd(int new_fd){
    check_syscall(dup2(new_fd, STDIN_FILENO), "dup2"); 
    check_syscall(close(new_fd), "close");
}

void change_output_stream_fd(int new_fd){
    check_syscall(dup2(new_fd, STDOUT_FILENO), "dup2"); 
    check_syscall(close(new_fd), "close");
}