#ifndef ERRORS_H
#define ERRORS_H

int check_syscall(int ret_value, const char* context);
void my_perror(const char* context, const char* message);

#endif