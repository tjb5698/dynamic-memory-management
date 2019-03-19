// Linked list library for the lab 3
// DO NOT MODIFY THIS CODE

#include "include/link.h"

// Create a new element of a linked list and return its pointer
Link_t* new_link(void* key, void* data, int size)
{
    Link_t* new = NULL;
    new = malloc(sizeof(Link_t));
    new->key = key;
    new->data = data;
    new->size = size;
    new->next = NULL;
    return new;
}

// Append an item to the end of the given linked list
void append_item(Link_t* list, void* key, void* data, int size)
{
    Link_t* new = new_link(key, data, size);
    Link_t* current = list;
    if (current->key == NULL) {
        // If the list is empty
        current->key = key;
        current->data = data;
        current->size = size;
        current->next = NULL;
        free(new);
    } else {
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new;
    }
}

// Insert an item in a sorted linked list (sorted by address value)
void insert_item(Link_t* list, void* key, void* data, int size)
{
    Link_t* new = new_link(key, data, size);
    Link_t* current = list;
    Link_t* previous = current;
    // Search for where to insert the new link

    if (current->key == NULL) {
        // If the list is empty
        current->key = key;
        current->data = data;
        current->size = size;
        current->next = NULL;
        free(new);
    } else {
        while (current->key < key && current->next != NULL) {
            previous = current;
            current = current->next;
        }
        if (current->key < key) {
            // Add at the end of the list
            current->next = new;
        } else if (current == previous) {
            // Add at the head of the list
            new->size = current->size;
            new->key = current->key;
            new->next = current->next;
            new->data = current->data;
            current->size = size;
            current->key = key;
            current->next = new;
            current->data = data;
        } else {
            // Add between two links
            previous->next = new;
            new->next = current;
        }
    }
}

// Remove and return an item for the head of the list
Link_t* pop_item(Link_t* list)
{
    Link_t* out = new_link(NULL, NULL, 0);
    Link_t* old;
    out->size = list->size;
    out->key = list->key;
    out->size = list->size;
    out->data = list->data;
    if (list->next == NULL) {
        list->size = 0;
        list->key = NULL;
        list->data = NULL;
    } else {
        list->size = list->next->size;
        list->key = list->next->key;
        list->data = list->next->data;
        old = list->next;
        list->next = list->next->next;
        free(old);
    }
    return out;
}

// Remove and return an item with the specified address from the list
// If the item is not found, it will return NULL
Link_t* extract_item(Link_t* list, void* key)
{
    Link_t* out;
    Link_t* current = list;
    Link_t* previous = current;
    while (current->key != key && current->next != NULL) {
        previous = current;
        current = current->next;
    }
    if (current->key != key) {
        // Address not found
        return NULL;
    } else if (current == previous) {
        // Address at the head of the list
        out = pop_item(list);
        return out;
    } else {
        // Address in the list
        out = current;
        previous->next = current->next;
        return out;
    }
}

// Return the link related to the given key without removing it from the list
Link_t* peek_item(Link_t* list, void* key)
{
    Link_t* current = list;
    while (current != NULL && current->key != key) {
        current = current->next;
    }
    return current;
}

// Frees all the given linked list, assuming that data is an allocated struct
void free_linked_list(Link_t* list)
{
    Link_t *current, *previous;
    current = list;
    previous = current;
    while (current != NULL) {
        current = current->next;
        free(previous->data);
        free(previous);
        previous = current;
    }
}