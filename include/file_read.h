#ifndef FILE_READ_H
#define FILE_READ_H

#include <stdio.h>
#include <stdlib.h>

#include "lab3.h"

#define ALLOC 0
#define FREE 1
#define REALLOC 2

int open_file(char const* filename, FILE** fd);
void close_file(FILE* fd);
int read_op(ops_t* ops, FILE** file);

#endif // FILE_READ_H