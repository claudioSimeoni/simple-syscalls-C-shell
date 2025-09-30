#ifndef ERRORS_H
#define ERRORS_H

void my_perror(const char* context, const char* message);
void my_perror_noexit(const char* context, const char* message);

int check_syscall(int ret_value, const char* context);
int check_syscall_noexit(int ret_value, const char* context);

#endif