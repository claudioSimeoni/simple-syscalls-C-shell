#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int check_redirection_char(char* arg){
    if(!strcmp(arg, "<") || !strcmp(arg, ">") || !strcmp(arg, ">>") 
                         || !strcmp(arg, "2>") || !strcmp(arg, "2>>")) return 1;
    return 0;
}

void change_input_stream(char* new_stream){
    int new_fd = open(new_stream, O_RDONLY | O_CREAT, 0644);
    dup2(new_fd, STDIN_FILENO); 
    close(new_fd);
}

void change_output_stream(char* new_stream, int append){
    int new_fd = open(new_stream, O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC), 0644);
    dup2(new_fd, STDOUT_FILENO); 
    close(new_fd);
}

void change_error_stream(char* new_stream, int append){
    int new_fd = open(new_stream, O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC), 0644);
    dup2(new_fd, STDERR_FILENO); 
    close(new_fd);
}

/* these are used by the piping module */
void change_input_stream_fd(int new_fd){
    dup2(new_fd, STDIN_FILENO); 
    close(new_fd);
}

void change_output_stream_fd(int new_fd){
    dup2(new_fd, STDOUT_FILENO); 
    close(new_fd);
}