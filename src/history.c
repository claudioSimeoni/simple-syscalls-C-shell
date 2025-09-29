#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include "../include/history.h"
#include "../include/constants.h"
#include "../include/parse.h"
#include "../include/errors.h"

/* History (declared in history.h) is a queue-like struct that handles history saving and 
   retrieving by using a .history file that is added in the main dir of the project */

void init_history(History* h){
    h->beg = 0; 
    h->end = 0;
    h->empty = 1;
}

void push_history(History* h, char* new_command){
    strcpy(h->vec[h->end], new_command);
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
    int fd = check_syscall(open(".history", O_RDONLY | O_CREAT, 0644), "open");
    char buff[BUFFER_SIZE];
    
    while(my_getline(fd, buff, BUFFER_SIZE)){
        push_history(h, buff);
    }
}

void write_history(History* h){
    if(h->empty) return; 
    int fd = check_syscall(open(".history", O_WRONLY | O_CREAT | O_TRUNC, 0644), "open");
    
    int beg = h->beg, end = h->end;
    if(beg >= end) end += HISTORY_SIZE;
    
    for(int i=beg; i<end; i++){
        int j = i % HISTORY_SIZE;
        check_syscall(write(fd, h->vec[j], strlen(h->vec[j])), "write");
        check_syscall(write(fd, "\n", 1) , "write");
    }
}

/* utility function for checking that the history module works properly */
void print_history(History* h){
    if(h->empty) return; 
    int beg = h->beg, end = h->end;
    if(beg >= end) end += HISTORY_SIZE;
    
    for(int i=beg; i<end; i++){
        int j = i % HISTORY_SIZE;
        check_syscall(write(1, h->vec[j], strlen(h->vec[j])), "write");
        check_syscall(write(1, "\n", 1) , "write");
    }
}