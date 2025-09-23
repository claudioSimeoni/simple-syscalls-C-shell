#ifndef UTILITIES_H
#define UTILITIES_H

ssize_t mygetline(int fd, char* buff, size_t bytes); 

void print_error(char* err); 

void run_executable(char* argv[]);

#endif
