#include <unistd.h>
#include <string.h>

#include "../include/errors.h"

void run_executable(char* argv[]){
    if (!argv || !argv[0]) my_perror("exec", "invalid argv");

    /* if the path is relative using execv otherwise using execvp */
    if(strchr(argv[0], '/')){
        execv(argv[0], argv);
    }
    else execvp(argv[0], argv);

    /* handling error with exec */
    my_perror(argv[0], "not an executable");
}