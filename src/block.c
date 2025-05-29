#include "../include/datatype.h"

typedef struct Block { 
    void* mem_space;
    int bytes;

} Block;

int getSize(DataType type){
    /*
    This function has already been implemented for you. Do not edit it.
    */

    if(type == INT_TYPE){
        return sizeof(int);
    }
    else if(type == FLOAT_TYPE){
        return sizeof(float);
    }
    else if(type == CHAR_TYPE){
        return sizeof(char);
    }
    else if(type == DOUBLE_TYPE){
        return sizeof(double);
    }
    else{
        return 0;
    }
}

Block* initializeBlock(DataType type, int units, void* data){
    
    if(units == 0){
        return NULL;
    }

    int size = units * getSize(type);
    Block* new_block = (Block*) malloc(sizeof(Block));
    
    if (new_block == NULL || data == NULL){
        return NULL;
    }

    new_block -> mem_space = (void*) malloc(size);

    if(new_block -> mem_space == NULL){
        free(new_block);
        return NULL;
    } 

    new_block -> bytes = size;
    memcpy(new_block -> mem_space, data , size);

    return new_block;

}

Block* blockMalloc(DataType type, int units){

    void* data = (void*)calloc(units, getSize(type));
    
    if( data == NULL ){
        return NULL;
    }

    Block* new_block = initializeBlock(type, units, data);
    free(data);
    return new_block;
    
}

void blockFree(Block* block){

    if( block != NULL){
        free(block -> mem_space);
        free(block);
    }
}

void blockStoreData(Block* block, void* data){

    if(data == NULL || block == NULL){
        return;
    }

    memcpy(block -> mem_space, data, block -> bytes);

    

}

void blockAccessData(Block* block, void* dest, DataType type, int units){
    int length;

    if( block == NULL || dest == NULL){
        return;
    }

    if( block -> bytes < units * getSize(type)){
        length = block -> bytes;
    }else {
        length = units * getSize(type);
    }

    memcpy(dest, block -> mem_space, length);

}

void blockRealloc(Block* block, DataType type, int new_units){
    int length;
    int size = getSize(type) * new_units;

    if(block == NULL || size <= 0){
        return;
    }

    if(size > block -> bytes ){

        length = block -> bytes;

    }else if (size < block -> bytes ){

        length = size;

    }else{

        return;

    }

    void* new_space = malloc(size);

    if(new_space == NULL){

        return;
    }

    memcpy(new_space, block -> mem_space, length);

    free(block -> mem_space);
    block -> mem_space = new_space;
    block -> bytes = size; 
    new_space = NULL;

}

/*
------------------------------------------------------------------------------
DO NOT EDIT THE FUNCTIONS BELOW - TESTING PURPOSES
*/

int getBytes(Block* block){
    return block->bytes;
}

void* getMemSpace(Block* block){
    return block->mem_space;
}
