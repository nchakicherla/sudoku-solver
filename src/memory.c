#include "memory.h"

void*
wcalloc(size_t n, size_t size) {

    void* output_ptr = NULL;
    if(!(output_ptr = calloc(n, size))) {
        printf("\n\nNEW ALLOCATION FAILED! EXITING...\n");
        exit(EXIT_FAILURE);
    }
    return output_ptr;
}
