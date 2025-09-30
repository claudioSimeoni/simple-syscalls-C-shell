#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "errors.h"
#include "history.h"
#include "parse.h"

/* this section is used by the command `his` for stopping normal input handling mode, since
   to detect arrow_keys properly we want to stop buffering and echoing (otherwise we would have
   1. to wait for a `\n` to read the entered keys, 2. to stop displaying every character's traditional
   echo) */
struct termios orig_termios;

void disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode); /* restore on exit */

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON); /* disable echo and canonical mode */
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

void display_command(History* history, int ptr){
    check_syscall(write(STDIN_FILENO, history->vec[ptr], strlen(history->vec[ptr])), "write"); 
}

int check_command(char* command){
    if(!strcmp(command, "his") || !strcmp(command, "cd") || !strcmp(command, "exit")) return 1;
    return 0;
}

int handle_command(char* argv[], History* history, int pipefd[]){
    if(!strcmp(argv[0], "his")){
        if(argv[1] != NULL) my_perror("his", "no argument required"); 
        if(history->empty) my_perror("his", "no elements in the history");
        
        /* ptr will track the current command  selected */
        int ptr = (history->end - 1 + HISTORY_SIZE) % HISTORY_SIZE; 
        enable_raw_mode();

        /* displaying the last used command (will always be `his` since you just called it)*/
        check_syscall(write(STDOUT_FILENO, "\33[2K\r", strlen("\33[2K\r")), "write");
        display_command(history, ptr);

        /* start looping for detecting arrow keys */
        while(1){
            char c; 
            check_syscall(read(STDIN_FILENO, &c, 1), "read");
            
            if(c == 27){
                char seq[2];
                check_syscall(read(STDIN_FILENO, seq, 1), "read");
                check_syscall(read(STDIN_FILENO, seq + 1, 1), "read");
                
                if(seq[0] == '['){
                    /* arrow up */
                    if(seq[1] == 'A'){
                        if(ptr != history->beg){
                            ptr--;
                            ptr = (ptr + HISTORY_SIZE) % HISTORY_SIZE;
                        }
                        check_syscall(write(STDOUT_FILENO, "\33[2K\r", strlen("\33[2K\r")), "write");
                        display_command(history, ptr);
                    }
                    /* arrow down */
                    else if(seq[1] == 'B'){
                        if(ptr != (history->end - 1 + HISTORY_SIZE) % HISTORY_SIZE){
                            ptr++;
                            ptr = (ptr + HISTORY_SIZE) % HISTORY_SIZE;
                        }
                        check_syscall(write(STDOUT_FILENO, "\33[2K\r", strlen("\33[2K\r")), "write");
                        display_command(history, ptr);
                    }
                }
            }
            else if(c == '\n'){
                break;
            }
        }
        /* disabling raw mode and launching the selected command */
        disable_raw_mode();
        return parse_buffer(history->vec[ptr], argv, history, pipefd); 
    }
    else if(!strcmp(argv[0], "cd")){
        if(argv[1] == NULL) my_perror("cd", "argument required");
        if(argv[2] != NULL) my_perror("cd", "only one argument required");

        /* writing to the write end of the pipe the command and the argument */
        check_syscall(write(pipefd[1], "cd\n", strlen("cd\n")), "write");
        check_syscall(write(pipefd[1], argv[1], strlen(argv[1])), "write");
        return -1;
    }
    else if(!strcmp(argv[0], "exit")){
        if(argv[1] != NULL) my_perror("exit", "no argument required");

        /* writing to the write end of the pipe the command */
        check_syscall(write(pipefd[1], "exit", strlen("exit")), "write");
        return -1;
    }
    else return 0; 
}