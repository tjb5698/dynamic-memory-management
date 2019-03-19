#ifndef LINK_H
#define LINK_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Link of a linked list
struct link_struct {
    void* key;
    void* data;
    int size; // size of data
    struct link_struct* next;
} typedef Link_t;

// Create a new element of a linked list and return its pointer
Link_t* new_link(void* key, void* data, int size);

// Append an item to the end of the given linked list
void append_item(Link_t* list, void* key, void* data, int size);

// Insert an item in a sorted linked list (sorted by address value)
void insert_item(Link_t* list, void* key, void* data, int size);

// Remove and return an item for the head of the list
Link_t* pop_item(Link_t* list);

// Remove and return an item with the specified address from the list
// If the item is not found, it will return an empty link
Link_t* extract_item(Link_t* list, void* key);

// Return the link related to the given key without removing it from the list
Link_t* peek_item(Link_t* list, void* key);

// Frees the given liked list
void free_linked_list(Link_t* list);

#endif // LINK_H