#ifndef HISTORY_H
#define HISTORY_H

#include "constants.h"

typedef struct History{
    char vec[HISTORY_SIZE][BUFFER_SIZE];
    int beg, end, empty;
} History;

void init_history(History* h);
void push_history(History* h, char* new_command);
void read_history(History* h);
void write_history(History* h);

void print_history(History* h);

#endif