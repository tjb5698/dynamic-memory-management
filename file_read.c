// Handler functions for the lab 3
// DO NOT MODIFY THIS CODE

#include "include/file_read.h"
#include "include/lab3.h"

int open_file(char const* filename, FILE** fd)
{
    char mode = 'r';
    *fd = fopen(filename, &mode);
    if (fd == NULL) {
        printf("Invalid input file specified: %s\n", filename);
        return -1;
    } else {
        return 0;
    }
}

void close_file(FILE* fd)
{
    fclose(fd);
}

int read_op(ops_t* ops, FILE** file)
{
    char line[1024];
    char *token, *rest;

    // Read the first line in the file
    if (!fgets(line, 1024, *file)) {
        return 1;
    }
    rest = line;

    // Get operation type
    if ((token = strsep(&rest, " ")) == NULL) {
        printf("Test file badly formatted: operation type is not readable\n");
        return 1;
    }
    if (strcmp(token, "M") == 0) {
        ops->op_type = ALLOC;
    } else if (strcmp(token, "F") == 0) {
        ops->op_type = FREE;
    } else if (strcmp(token, "R") == 0) {
        ops->op_type = REALLOC;
    }

    // Get label
    if ((token = strsep(&rest, " ")) == NULL) {
        printf("Test file badly formatted: label is not readable\n");
        return 1;
    }
    ops->label = *token;

    // Get third token
    if ((token = strsep(&rest, " ")) == NULL) {
        printf("Test file badly formatted: third field not readable\n");
        return 1;
    }
    ops->nb_elems = atoi(token);
    ops->range_start = atoi(token);

    // Get the fourth token
    if ((token = strsep(&rest, " ")) == NULL) {
        printf("Test file badly formatted: fourth field not readable\n");
        return 1;
    }
    ops->elem_size = atoi(token);
    ops->range_end = atoi(token);

    // Get the fifth token (if it exists)
    if (ops->op_type == REALLOC) {
        if ((token = strsep(&rest, " ")) == NULL) {
            printf("Test file badly formatted: fifth field not readable\n");
            return 1;
        }
        ops->elem_size = atoi(token);
    }
    return 0;
}
