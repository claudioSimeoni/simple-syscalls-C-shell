#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include <libgen.h> /* this one contains dirname */

#include "../include/history.h"
#include "../include/constants.h"
#include "../include/parse.h"
#include "../include/errors.h"

int min(int a, int b){
    return (a < b) ? a : b;
}

/* this function gets the absolute path for history.c so that .history is always in the same dir
   and it is independent from the dir you are currently in */
void retrieve_full_path(char* full_path){
    char* file_path = "/.history";

    /* getting the directory of the current source file */
    char source_path[BUFFER_SIZE];
    strncpy(source_path, __FILE__, BUFFER_SIZE - 1);
    source_path[BUFFER_SIZE - 1] = '\0';
    char* dir = dirname(source_path);

    strncpy(full_path, dir, BUFFER_SIZE - 1);
    strncat(full_path, file_path, BUFFER_SIZE - 1 - min(BUFFER_SIZE - 1, strlen(dir)));
    full_path[BUFFER_SIZE - 1] = '\0';
}

/* History (declared in history.h) is a queue-like struct that handles history saving and 
   retrieving by using a .history file that is added in the main dir of the project */

void init_history(History* h){
    h->beg = 0; 
    h->end = 0;
    h->empty = 1;
}

void push_history(History* h, char* new_command){
    strncpy(h->vec[h->end], new_command, BUFFER_SIZE - 1);
    h->vec[h->end][BUFFER_SIZE - 1] = '\0';
    if(h->end == h->beg && !h->empty){
        h->beg++; h->end++;
    }
    else{
        h->end++;
    }
    h->empty = 0;
    h->beg %= HISTORY_SIZE;
    h->end %= HISTORY_SIZE;
}

void read_history(History* h){
    char full_path[BUFFER_SIZE];
    retrieve_full_path(full_path);
    int fd = check_syscall(open(full_path, O_RDONLY | O_CREAT, 0644), "open");

    char buff[BUFFER_SIZE];
    while(my_getline(fd, buff, BUFFER_SIZE)){
        push_history(h, buff);
    }

    check_syscall(close(fd), "close");
}

void write_history(History* h){
    if(h->empty) return; 

    char full_path[BUFFER_SIZE];
    retrieve_full_path(full_path);
    int fd = check_syscall(open(full_path, O_WRONLY | O_CREAT | O_TRUNC, 0644), "open");

    int beg = h->beg, end = h->end;
    if(beg >= end) end += HISTORY_SIZE;
    
    for(int i=beg; i<end; i++){
        int j = i % HISTORY_SIZE;
        check_syscall(write(fd, h->vec[j], strlen(h->vec[j])), "write");
        check_syscall(write(fd, "\n", 1) , "write");
    }

    check_syscall(close(fd), "close");
}