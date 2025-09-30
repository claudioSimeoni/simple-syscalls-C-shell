#ifndef PARSE_H
#define PARSE_H

#include "history.h"

ssize_t my_getline(int fd, char* buff, size_t bytes);
int parse_buffer(char* buff, char** argv, History* history, int pipefd[]); 

#endif