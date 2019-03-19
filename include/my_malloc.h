#ifndef MY_MALLOC_H
#define MY_MALLOC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "link.h"

// Macros for the policies
#define FIRST_FIT 0
#define BEST_FIT 1
#define WORST_FIT 2

// Memory hole struct
struct {
    void* address;
    int size;
} typedef Hole_t;

// Mmeory block struct
struct {
    void* address;
} typedef Block_t;

// Memory allocator information
struct {
    int type;
    int total_size;
    int page_size;
    int nb_pages;
    void* start_address;
} typedef Infos_t;

// Setup the memory allocation scheme
void setup(int alloc_type, int total_size, int page_size, void* start_address);

// Merge all the neighboring holes in the given linked list of Hole_t structs
void merge_holes(Link_t* list);

// Return the hole corresponding to the first fit for the given size
Hole_t* get_first_fit(Link_t* list, int size);

// Given the list of holes and blocks, the desired and the size,
// creates a block in the this hole and return its address
void* create_block(Link_t* list_holes, Link_t* list_blocks, Hole_t* hole, int size);

// Allocate the requested number of bytes and return a pointer to it
void* my_malloc(int size);

// Free the memory designated by the given address
int my_free(void* addr);

// Reallocates the given pointer with the new given size, while conserving the contents of the object
void* my_realloc(void* addr, int size);

#endif // MY_MALLOC_H