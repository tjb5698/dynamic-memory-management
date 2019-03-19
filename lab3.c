// Handler functions for the lab 3
// DO NOT MODIFY THIS CODE

#include "include/lab3.h"
#include "include/file_read.h"
#include "include/helpers.h"

#define ALLOC 0
#define FREE 1
#define REALLOC 2

#define make_key(a, b) (void*)(((long)a << 32) + b)
#define min(a, b) (((a) < (b)) ? (a) : (b))

extern Link_t* holes;
extern Link_t* blocks;
extern int offset;
extern Infos_t* infos;

// Perform all the my_alloc calls specified in the op struct, and stores the
// variables in the given given linked list, in a Variable_t struct
int request_alloc(Link_t* variables, ops_t* op, void* base_address)
{
    int index;
    int elem_size = op->elem_size;
    char label = op->label;
    void* key;
    void* addr;
    char* contents;
    Variable_t* var;
    // Perform all the allocations
    for (index = 0; index < op->nb_elems; index++) {
        var = malloc(sizeof(Variable_t));
        var->label = label;
        var->index = index;
        var->size = elem_size;
        addr = my_malloc(elem_size);
        var->address = addr;
        key = make_key(label, index);
        contents = malloc(elem_size);
        for (int i = 0; i < elem_size; i++) {
            contents[i] = (char)rand();
        }
        // Make sure the returned address is writeable
        memcpy(addr, contents, elem_size);
        var->contents = contents;
        insert_item(variables, key, var, sizeof(Variable_t));
        printf("Allocated block %c-%d at %ld\n", var->label, var->index, (void*)var->address - base_address);
    }
    return 0;
}

// Perform all the my_free calls specified in the op struct, and removes the
// variables in the given given linked list
int request_free(Link_t* variables, ops_t* op, void* base_address)
{
    int index, start, end;
    char label = op->label;
    void* key;
    Link_t* link;
    Variable_t* var;
    start = op->range_start;
    end = op->range_end;
    for (index = start; index <= end; index++) {
        key = make_key(label, index);
        link = extract_item(variables, key);
        // Make sure the variable was found
        if (link != NULL && link->data != NULL) {
            var = link->data;
            printf("Freed block %c-%d\n", var->label, var->index);
            my_free(var->address);
            free(var->contents);
            free(var);
            free(link);
        }
    }
    return 0;
}

// Perform all the my_realloc calls specified in the op struct, and stores the
// variables in the given given linked list, in a Variable_t struct
int request_realloc(Link_t* variables, ops_t* op, void* base_address)
{
    int index, start, end, old_size;
    int new_size = op->elem_size;
    char label = op->label;
    void* key;
    void *new_addr, *old_addr;
    char *new_contents, *old_contents;
    Link_t* link;
    Variable_t* var;
    start = op->range_start;
    end = op->range_end;
    for (index = start; index <= end; index++) {
        key = make_key(label, index);
        link = extract_item(variables, key);
        // Make sure the variable was found
        if (link != NULL && link->data != NULL) {
            var = link->data;
            old_size = var->size;
            old_addr = var->address;
            old_contents = var->contents;
            new_addr = my_realloc(old_addr, new_size);
            // Make sure the contents were not altered
            if (memcmp(new_addr, old_contents, min(old_size, new_size)) != 0) {
                printf("Error: contents of variable changed by realloc\n");
            }
            // Write new contents to the address
            new_contents = malloc(new_size);
            for (int i = 0; i < new_size; i++) {
                new_contents[i] = (char)rand();
            }
            memcpy(new_addr, new_contents, new_size);
            free(old_contents);
            var->address = new_addr;
            var->size = new_size;
            var->contents = new_contents;
            // Free the link and insert a new one
            free(link);
            insert_item(variables, key, var, sizeof(Variable_t));
            printf("Reallocated block %c-%d at %ld\n", var->label, var->index, (void*)var->address - base_address);
        }
    }
    return 0;
}

// Frees the list of all the variables as well as their contents
void free_var_list(Link_t* variables)
{
    Link_t* current = variables;
    Variable_t* current_var;
    Link_t* next;
    while (current != NULL) {
        current_var = current->data;
        next = current->next;
        free(current_var->contents);
        free(current_var);
        free(current);
        current = next;
    }
}

int main(int argc, char const* argv[])
{
    FILE* file = NULL;
    int alloc_type;
    int page_size;
    int nb_pages;
    int total_size;
    int err;
    void* base_address;
    char *token, *rest;
    char line[1024];
    ops_t ops;
    Link_t* variables = new_link(NULL, NULL, 0);
    // Read the command line arguments
    if (argc < 3) {
        printf("Not enough parameters specified.  \nUsage: ./project3 <allocation_type> <input_file>\n");
        return -1;
    }
    // Read the allocation type
    alloc_type = atoi(argv[1]);
    // Open the input file
    if (open_file(argv[2], &file) != 0) {
        return -1;
    }
    // Read the first line in the file
    if (!fgets(line, 1024, file)) {
        return 1;
    }
    rest = line;
    if ((token = strsep(&rest, " ")) == NULL) {
        printf("Test file badly formatted: first line is not readable\n");
        return 1;
    }
    nb_pages = atoi(token);
    // Get all the required size parameters
    page_size = getpagesize();
    total_size = page_size * nb_pages;
    // Allocate the memory block aligned with the start of a page
    base_address = mmap(0, total_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (base_address == MAP_FAILED) {
        printf("Error while allocating the inital memory\n");
        return 1;
    }
    // Setup the allocation scheme
    setup(alloc_type, total_size, page_size, base_address);
    // Initialize the RNG
    srand(0);

    // Go over all operations
    while (read_op(&ops, &file) == 0) {
        // printf("Read instruction %d\n", ops.op_type);
        switch (ops.op_type) {
        case ALLOC:
            err = request_alloc(variables, &ops, base_address);
            if (err != 0) {
                printf("Allocation failed for label %c, with %d elements of size %d bytes\n",
                    ops.label, ops.nb_elems, ops.elem_size);
            }
            break;
        case FREE:
            err = request_free(variables, &ops, base_address);
            if (err != 0) {
                printf("Free failed for label %c, going from element %d to element %d\n",
                    ops.label, ops.range_start, ops.range_end);
            }
            break;
        case REALLOC:
            err = request_realloc(variables, &ops, base_address);
            if (err != 0) {
                printf("Realloc failed for label %c, going from element %d to element %d with size %d\n",
                    ops.label, ops.range_start, ops.range_end, ops.elem_size);
            }
            break;
        default:
            break;
        }
    }

    // Free all the things
    munmap(base_address, total_size);
    free_linked_list(holes);
    free_linked_list(blocks);
    free_var_list(variables);
    free(infos);

    // Close the file
    close_file(file);

    return 0;
}
