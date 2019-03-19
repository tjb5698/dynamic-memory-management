#include "include/my_malloc.h"
#include "include/helpers.h"
#include "include/link.h"

// Macros for the policies
#define FIRST_FIT 0
#define BEST_FIT 1
#define WORST_FIT 2

// Write all functions you need in this file
// Remember to update the associated old_size file to add the prototypes

// Names and ID:
// Student 1:
//      Name:
//      PSU email:

// Student 2:
//      Name:
//      PSU email:

// Global variables
Link_t* holes;
Link_t* blocks;
Infos_t* infos;
int offset = 8;

void setup(int alloc_type, int total_size, int page_size, void* start_address)
{
    // This function is called once, before any allocation is done
    // Use is to initialize any variables or structs you want to use
    // in further functions

    // Setup the memory infos
    infos = malloc(sizeof(Infos_t));
    infos->type = alloc_type;
    infos->total_size = total_size;
    infos->page_size = page_size;
    infos->nb_pages = total_size / page_size;
    infos->start_address = start_address;

    // Create the inital hole
    Hole_t* hole = malloc(sizeof(Hole_t));
    hole->address = start_address;
    hole->size = total_size;
    // Add the hole to the linked list
    holes = new_link(start_address, hole, sizeof(Hole_t));

    // Initialize the blocks list
    blocks = new_link(NULL, NULL, 0);
}

void merge_holes(Link_t* list)
{
    Link_t *previous, *current;
    Hole_t *prev_hole, *current_hole;
    current = list->next;
    previous = list;
    while (current != NULL) {
        prev_hole = previous->data;
        current_hole = current->data;
        if (prev_hole->address + prev_hole->size == current_hole->address) {
            prev_hole->size += current_hole->size;
            extract_item(list, current->key);
            free(current_hole);
            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}

Hole_t* get_first_fit(Link_t* list, int size)
{
    Link_t* current = list;
    Hole_t* current_hole;
    while (current != NULL) {
        current_hole = current->data;
        if (current_hole->size >= size + offset) {
            return current_hole;
        }
        current = current->next;
    }
    return NULL;
}

void* create_block(Link_t* list_holes, Link_t* list_blocks, Hole_t* hole, int size)
{
    void* out;
    Block_t* new_block;
    Link_t* old_link;
    // Create a new memory block
    new_block = malloc(sizeof(Block_t));
    out = hole->address + offset;
    new_block->address = out;
    insert_item(list_blocks, out, new_block, sizeof(Block_t));
    // Edit the hole
    old_link = extract_item(list_holes, hole->address);
    free(old_link);
    hole->size -= (size + offset);
    hole->address += (size + offset);
    // Don't insert if the hole is now of size 0
    if (hole->size != 0) {
        insert_item(list_holes, hole->address, hole, sizeof(Hole_t));
    } else {
        free(hole);
    }
    // Add the size to the memory block
    set_header(out, size, offset);
    // Return the address
    return out;
}

void* my_malloc(int size)
{
    // This function is called to request a continuous memory allocation
    // It should return a pointer to the start of the allocated memory
    void* out = NULL;
    Hole_t* hole;
    // Make sure that the allocated memory is word aligned
    size = get_word_aligned_size(size);
    switch (infos->type) {
    case FIRST_FIT:
        // Get the target hole
        hole = get_first_fit(holes, size);
        if (hole != NULL) {
            // Create a new memory block
            out = create_block(holes, blocks, hole, size);
            return out;
        }
        break;

    case BEST_FIT:
        break;

    case WORST_FIT:
        break;

    default:
        break;
    }

    return out;
}

int my_free(void* addr)
{
    // This function deallocated the memory at the given address
    // It returns 0 if the deallocation was successful
    // It returns 1 if the deallocation failed (e.g if no allocated memory exists
    // at the given address)
    // It should also merge neighouring free memory blocks
    Block_t* block;
    Hole_t* new_hole;
    Link_t* current = blocks;
    Link_t* old_block_link;
    int old_size;
    // Find the block related to the given address
    while (current != NULL) {
        block = current->data;
        if (addr == block->address) {
            // Get the size of the block, stored in its old_size
            old_size = get_header(addr, offset);
            // Create a new hole
            new_hole = malloc(sizeof(Hole_t));
            new_hole->address = addr - offset;
            new_hole->size = old_size + offset;
            insert_item(holes, new_hole->address, new_hole, sizeof(Hole_t));
            // Delete the old block
            old_block_link = extract_item(blocks, addr);
            free(old_block_link->data);
            free(old_block_link);
            // Merge the new holes if possible
            merge_holes(holes);
            // Return
            return 0;
        }
        current = current->next;
    }

    return 1;
}

void* my_realloc(void* addr, int new_size)
{
    // This function modifies the allocated space for the given pointer
    // If there is space for it, it will expand/contract the related block and return the original address
    // If there is not enough space, it will allocate a new block of the desired new_size and return its address
    // The original contents of the given block are carried over to the new block
    // The old block is then freed
    return addr;
}