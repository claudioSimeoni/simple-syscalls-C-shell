#include <unistd.h>
#include <string.h>

/* this getline differently from the libc one removes the new line 
   since it is useless for our shell purposes */
ssize_t mygetline(int fd, char* buff, size_t bytes){
    int handle = read(fd, buff, bytes); 

    /* error occurred or EOF reached */
    if(handle == -1) return -1; 
    if(handle == 0) return 0; 
    
    for(int i=0; i<(int)bytes - 1; i++){
        if(buff[i] == '\n'){
            buff[i] = 0; 
            return i; 
        }

        if(i == (int)bytes - 2){
            buff[i + 1] = 0; 
            return i + 1; 
        }
    }
}

void print_error(char* err){
    char* pref = "\033[31m";
    char* post = "\033[0m"; 

    write(2, pref, strlen(pref)); 
    write(2, err, strlen(err)); 
    write(2, post, strlen(post)); 
}