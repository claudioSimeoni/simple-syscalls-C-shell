#include <errno.h>
#include <unistd.h>
#include <string.h>

const char* pref = "\033[31m";
const char* post = "\033[0m"; 
const char* error = "Error occurred with ";

void my_perror(const char* context, const char* message){
    write(STDERR_FILENO, pref, strlen(pref));
    write(STDERR_FILENO, error, strlen(error));
    write(STDERR_FILENO, context, strlen(context));
    write(STDERR_FILENO, ": ", 2);
    write(STDERR_FILENO, message, strlen(message));
    write(STDERR_FILENO, post, strlen(post));   
    write(STDERR_FILENO, "\n", 1);  
    _exit(1);
}

int check_syscall(int ret_value, const char* context){
    if(ret_value == -1){       
        my_perror(context, strerror(errno));     
    }
    return ret_value;
}

