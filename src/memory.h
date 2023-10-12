#ifndef S_MEMORY_H
#define S_MEMORY_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct sMemGroup *MemGroup;

void*
wcalloc(size_t n, size_t size);

void*
gmalloc(MemGroup mem_group, size_t needed_bytes);

MemGroup
initMemGroup(size_t size_request);
/*
int
growMemGroup(MemGroup mem_group);
*/
int
freeMemGroup(MemGroup *mem_group);

#endif // S_MEMORY_H
