#include <math.h>

#include "include/helpers.h"
#include "include/link.h"
#include "include/my_malloc.h"

float get_fragmentation(Link_t* list)
{
    int total_free, largest_hole;
    float frag;
    Link_t* current;
    Hole_t* current_hole;
    if (list == NULL || list->data == NULL) {
        return -1;
    }
    current = list;
    total_free = 0;
    largest_hole = 0;
    while (current != NULL) {
        current_hole = current->data;
        total_free += current_hole->size;
        if (current_hole->size > largest_hole) {
            largest_hole = current_hole->size;
        }
        current = current->next;
    }
    frag = (float)(total_free - largest_hole) / (float)total_free;
    return frag;
}

int get_header(void* addr, int offset)
{
    long out;
    memcpy(&out, addr - offset, offset);
    return (int)out;
}

void set_header(void* addr, int size, int offset)
{
    memcpy(addr - offset, &size, offset);
}

int get_word_aligned_size(int size)
{
    int out = ((size - 1) & ~0b111) + 8;
    return out;
}