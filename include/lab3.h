#ifndef LAB3_H
#define LAB3_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "my_malloc.h"

struct {
    int op_type;
    char label;
    int nb_elems;
    int elem_size;
    int range_start;
    int range_end;
} typedef ops_t;

struct {
    char label;
    int index;
    void* address;
    int size;
    char* contents;
} typedef Variable_t;

int request_alloc(Link_t* variables, ops_t* op, void* base_address);
int request_free(Link_t* variables, ops_t* op, void* base_address);
int main(int argc, char const* argv[]);

#endif // LAB3_H