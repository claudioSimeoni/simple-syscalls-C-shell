#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h> // included for exit

#include "../include/utilities.h"
#include "../include/redirection.h"

int execute_piping(char* argv1[], char* argv2[]){
    int pipefd[2];
    pipe(pipefd);

    int new_pid1 = fork();
    if(new_pid1 < 0){
        print_error("An error occurred during the creation of a new process!\n");
        return -1;
    }
    else if(new_pid1 == 0){
        change_output_stream_fd(pipefd[1]);

        if(strchr(argv1[0], '/')){
                execv(argv1[0], argv1);
        }
        else execvp(argv1[0], argv1);

        /* here you might want to change the output stream once again ? */

        /* handling error with execvp */
        print_error(argv1[0]);
        print_error(" is not an executable!\n");
        exit(1);
        /* TODO: understanding if it makes sense to use _exit */
    }
    else{
        waitpid(new_pid1, NULL, 0);

        int new_pid2 = fork();
        if(new_pid2 < 0){
            print_error("An error occurred during the creation of a new process!\n");
            return -1;
        }
        else if(new_pid2 == 0){
            change_input_stream_fd(pipefd[0]);

            if(strchr(argv2[0], '/')){
                    execv(argv2[0], argv2);
            }
            else execvp(argv2[0], argv2);

            /* here you might want to change the output stream once again ? */

            /* handling error with execvp */
            print_error(argv2[0]);
            print_error(" is not an executable!\n");
            exit(1);
            /* TODO: understanding if it makes sense to use _exit */
        }
        else{
            waitpid(new_pid2, NULL, 0);
        }
    }

    return 1;
}