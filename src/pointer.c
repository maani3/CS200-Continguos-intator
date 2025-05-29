#include "../include/block.h"

typedef struct BlockPointer{
    Block* block;
    DataType type;
    int units;
    int offset;
    struct BlockPointer* next;

} BlockPointer;

BlockPointer* initializeBlockPointer(DataType type, int units, void* data){
    
    if(units == 0){
        return NULL;
    }


    BlockPointer* new_block_ptr = (BlockPointer*) malloc(sizeof(BlockPointer));

    if (new_block_ptr == NULL){
        return NULL;
    }

    new_block_ptr -> block = initializeBlock(type, units, data);

    if( new_block_ptr -> block == NULL ){
        free(new_block_ptr);
        return NULL;
    }

    new_block_ptr -> type = type;
    new_block_ptr -> units = units;
    new_block_ptr -> offset = 0;
    new_block_ptr -> next = NULL;


    return new_block_ptr;    
}

BlockPointer* pointerMalloc(DataType type, int units, BlockPointer* origin){

    if(units == 0 || origin == NULL){
        return NULL;
    }

    BlockPointer* current_block_ptr = origin;

    while(current_block_ptr -> next != NULL){
        current_block_ptr = current_block_ptr -> next;
    }


    BlockPointer* new_block_ptr = (BlockPointer*) malloc(sizeof(BlockPointer));

    if (new_block_ptr == NULL){
        return NULL;
    }

    new_block_ptr -> block = blockMalloc(type, units);

    if(new_block_ptr -> block == NULL){
        free(new_block_ptr);
        return NULL;
    }

    new_block_ptr -> type = type;
    new_block_ptr -> units = units;
    new_block_ptr -> offset = (current_block_ptr -> offset)+1;
    current_block_ptr -> next = new_block_ptr;

    return new_block_ptr;

}

void pointerFree(BlockPointer* block_ptr, BlockPointer* origin){
    
    if(origin == NULL || block_ptr == NULL){
        return;
    }

    BlockPointer* current_block_ptr = origin;
    BlockPointer* temp = current_block_ptr;

    while(current_block_ptr != NULL && current_block_ptr != block_ptr){
        temp = current_block_ptr;
        current_block_ptr = current_block_ptr -> next;
    }

    if(current_block_ptr == NULL){
        return;
    }

    temp -> next = current_block_ptr -> next;
    current_block_ptr -> next = NULL;
    blockFree( current_block_ptr -> block );
    free(current_block_ptr);
    current_block_ptr = NULL; 

    while(temp != NULL){

        if(temp -> next == NULL){
            return;
        }
        temp = temp -> next;
        temp -> offset = temp -> offset - 1;
        
    }
   return;
}

void pointerStoreData(BlockPointer* block_ptr, void* data){
    
    if(block_ptr == NULL || data == NULL){
        return;
    }

    blockStoreData(block_ptr -> block, data);

}

void pointerAccessData(BlockPointer* block_ptr, void* dest){

    if(block_ptr == NULL || dest == NULL){
        return;
    }

    blockAccessData(block_ptr -> block, dest, block_ptr -> type, block_ptr -> units);
    
}

void pointerRealloc(BlockPointer* block_ptr, BlockPointer* origin, DataType type, int new_units){

    if (block_ptr == NULL || origin == NULL || new_units == 0){
        return;
    }

    blockRealloc(block_ptr -> block, block_ptr -> type, new_units);
    block_ptr -> units = new_units;
}

BlockPointer* getNext(BlockPointer* block_ptr){
    if (block_ptr == NULL){
        return NULL;
    }

    return block_ptr -> next;
}

BlockPointer* getPrevious(BlockPointer* block_ptr, BlockPointer* origin){
    
    if(origin == NULL || block_ptr == NULL){
        return NULL;
    }

    if(origin == block_ptr){
        return NULL;
    }

    BlockPointer* curr_block_ptr = origin;

    
    while(curr_block_ptr -> next != block_ptr && curr_block_ptr -> next != NULL){

        curr_block_ptr = curr_block_ptr -> next;

    }

    return curr_block_ptr;
    
}

void pointerCompleteDeallocateHelper(BlockPointer* block_ptr, BlockPointer* origin){
   
   pointerFree(block_ptr, origin);
   return;

}

void pointerCompleteDeallocate(BlockPointer* origin){
   if(origin == NULL){
    return;
   }

   BlockPointer* curr_block_ptr = origin;
   BlockPointer* temp = curr_block_ptr;

   while(curr_block_ptr != NULL){
    
        temp = curr_block_ptr -> next;

        pointerCompleteDeallocateHelper(curr_block_ptr, origin);
        
        curr_block_ptr = temp;
        origin = curr_block_ptr;
    
   }

   return;
}


/*
------------------------------------------------------------------------------
DO NOT EDIT THE FUNCTIONS BELOW - TESTING PURPOSES
*/

Block* getBlock(BlockPointer* block_ptr){
    return block_ptr->block;
}

DataType getType(BlockPointer* block_ptr){
    return block_ptr->type;
}

int getUnits(BlockPointer* block_ptr){
    return block_ptr->units;
}

int getOffset(BlockPointer* block_ptr){
    return block_ptr->offset;
}