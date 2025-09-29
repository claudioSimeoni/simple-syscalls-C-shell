#ifndef REDIRECTION_H
#define REDIRECTION_H

int check_redirection_char(char* arg);

void change_input_stream(char* new_stream);
void change_output_stream(char* new_stream, int append);
void change_error_stream(char* new_stream, int append);

void change_input_stream_fd(int new_fd);
void change_output_stream_fd(int new_fd);

#endif