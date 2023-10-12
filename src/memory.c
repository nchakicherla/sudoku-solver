#include "memory.h"

#define DEFAULT_GROUP_SZ            65536 // 64 KiB

typedef struct sMemGroup {
    size_t size;
    size_t next_available;
    size_t remaining_bytes;
    char* data;
} *MemGroup;

void*
wcalloc(size_t n, size_t size) {

    void* output_ptr = NULL;
    if(!(output_ptr = calloc(n, size))) {
        printf("\n\nNEW ALLOCATION FAILED! EXITING...\n");
        exit(EXIT_FAILURE);
    }
    return output_ptr;
}

void*
gmalloc(MemGroup mem_group, size_t needed_bytes) {

    if(mem_group->remaining_bytes < needed_bytes) {
        printf("\n\nALLOCATION ON GROUP FAILED! EXITING...\n");
        exit(EXIT_FAILURE);
    }

    void* output_ptr = &(mem_group->data[mem_group->next_available]);
    mem_group->next_available += needed_bytes;
    mem_group->remaining_bytes -= needed_bytes;

    return output_ptr;
}

MemGroup
initMemGroup(size_t size_request) {

    size_t group_size;

    if(size_request) {
        group_size = size_request;
    } else {
        group_size = DEFAULT_GROUP_SZ;
    }

    MemGroup mem_group = wcalloc(1, sizeof(struct sMemGroup));
    mem_group->data = wcalloc(group_size, 1);
    mem_group->size = group_size;
    mem_group->next_available = 0;
    mem_group->remaining_bytes = group_size;

    return mem_group;
}
/*
int
growMemGroup(MemGroup mem_group) {

    if(mem_group->size > MAX_GROUP_SZ / 2) {
        printf("\n\nNEW MEMGROUP SIZE EXCEEDS MAX OF %zu BYTES! EXITING...\n", (size_t)MAX_GROUP_SZ);
        exit(EXIT_FAILURE);
    }

    size_t old_size = mem_group->size;
    char* new_data = NULL;

    new_data = realloc(mem_group->data, old_size * 2);
    if(!new_data) exit(EXIT_FAILURE);

    mem_group->data = new_data;
    mem_group->size *= 2;
    mem_group->remaining_bytes += old_size;
    return 0;
}
*/
int
freeMemGroup(MemGroup *mem_group) {

    free((*mem_group)->data);
    free(*mem_group);
    *mem_group = NULL;

    return 1;
}
