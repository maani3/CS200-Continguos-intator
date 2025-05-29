#ifndef MAPPINGS_H
#define MAPPINGS_H

#include <stdint.h>

typedef struct Mappings Mappings;

Mappings* initializeMap(int num_allocations, void* addr, char* identifier);

void deallocateMap(Mappings* map);

Mappings* resizeMap(Mappings* map, int new_num_allocs);

void makeEntry(Mappings** map, void* addr, char* identifier);

void removeEntry(Mappings* map, char* identifier);

void* getOrigin(Mappings* map);

void* getPointer(Mappings* map, char* identifier);

uintptr_t getAddress(Mappings* map, int index);

char* getIdentifier(Mappings* map, int index);

#endif