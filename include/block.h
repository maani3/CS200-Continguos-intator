#ifndef BLOCK_H
#define BLOCK_H

#include "datatype.h"

typedef struct Block Block;

Block* initializeBlock(DataType type, int units, void* data);

Block* blockMalloc(DataType type, int units);

void blockFree(Block* block);

void blockStoreData(Block* block, void* data);

void blockAccessData(Block* block, void* var, DataType type, int units);

void blockRealloc(Block* block, DataType type, int new_units);

int getBytes(Block* block);

void* getMemSpace(Block* block);

#endif