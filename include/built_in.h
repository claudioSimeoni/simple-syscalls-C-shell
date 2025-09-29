#ifndef BUILT_IN_H
#define BUILT_IN_H

#include "history.h"

int check_command(char* command);
int handle_command(char* argv[], History* history, int pipefd[]); 

#endif