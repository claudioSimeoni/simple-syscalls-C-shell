#include <unistd.h>
#include <string.h>

/* this getline differently from the libc one removes the new line 
   since it is useless for our shell purposes */
ssize_t mygetline(int fd, char* buff, size_t bytes){
    int handle = check_syscall(read(fd, buff, bytes), "read"); 

    /* EOF reached */
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

void run_executable(char* argv[]){
    if(strchr(argv[0], '/')){
        execv(argv[0], argv);
    }
    else execvp(argv[0], argv);

    /* handling error with exec */
    my_perror(argv[0], "not an executable");
}