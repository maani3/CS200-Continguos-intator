#ifndef POINTER_H
#define POINTER_H

#include "../include/block.h"

typedef struct BlockPointer BlockPointer;

BlockPointer* initializeBlockPointer(DataType type, int units, void* data);

BlockPointer* pointerMalloc(DataType type, int units, BlockPointer* origin);

void pointerFree(BlockPointer* block_ptr, BlockPointer* origin);

void pointerStoreData(BlockPointer* block_ptr, void* data);

void pointerAccessData(BlockPointer* block_ptr, void* dest);

void pointerRealloc(BlockPointer* block_ptr, BlockPointer* origin, DataType type, int new_units);

void pointerCompleteDeallocate(BlockPointer* origin);

BlockPointer* getNext(BlockPointer* block_ptr);

BlockPointer* getPrevious(BlockPointer* block_ptr, BlockPointer* origin);

Block* getBlock(BlockPointer* block_ptr);

DataType getType(BlockPointer* block_ptr);

int getUnits(BlockPointer* block_ptr);

int getOffset(BlockPointer* block_ptr);

#endif