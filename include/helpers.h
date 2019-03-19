#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>

#include "link.h"

// Calculates the fragmentation of the given linked list of holes
float get_fragmentation(Link_t* list);

// Extracts and return the header of the given address, with the given offset
int get_header(void* addr, int offset);

// Sets the header for the given address, with the given size and offset
void set_header(void* addr, int size, int offset);

// Returns the smallest multiple of 8 greater than the given int
int get_word_aligned_size(int size);

#endif // HELPERS_H