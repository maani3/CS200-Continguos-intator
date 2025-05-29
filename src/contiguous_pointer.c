#include "../include/pointer.h"
#include "../include/mappings.h"

typedef struct ContiguousPointer{
    BlockPointer* block_pointer;
    Mappings* map;
} ContiguousPointer;

ContiguousPointer* initializeContiguous(int num_allocations) { 

   
    if(num_allocations <= 0){
        return NULL;
    }

    ContiguousPointer* new_contiguos_pointer = (ContiguousPointer*)malloc(sizeof(ContiguousPointer));

    if(new_contiguos_pointer == NULL){
        return NULL;
    }

    char* identifier[1] = {"origin"};

    BlockPointer* new_block_ptr = initializeBlockPointer(CHAR_TYPE, 7, (void*)(identifier[0]));

    if(new_block_ptr == NULL){
        free(new_contiguos_pointer);
        return NULL;
    }
    
    new_contiguos_pointer -> map = initializeMap(num_allocations, (void*)(new_block_ptr), identifier[0]);

    if(new_contiguos_pointer -> map == NULL){
        free(new_block_ptr);
        free(new_contiguos_pointer);
        return NULL;
    }

    new_contiguos_pointer -> block_pointer = NULL;
    return new_contiguos_pointer;

}

void contiguousMalloc(ContiguousPointer* c_ptr, DataType type, int units, char* identifer){

    if(c_ptr == NULL || identifer == NULL){
        return;
    }

    BlockPointer* new_block_ptr = pointerMalloc(type, units, getOrigin(c_ptr -> map));
    
    if(new_block_ptr == NULL){
        return;
    }

    if(getPointer(c_ptr -> map, identifer) == NULL){

        makeEntry(&(c_ptr -> map), (void*)(new_block_ptr), identifer);
        c_ptr -> block_pointer = new_block_ptr;
        return;

    }else{
        pointerFree(new_block_ptr, getOrigin(c_ptr -> map));
    }

}

void contiguousFree(ContiguousPointer* c_ptr, char* identifier){

    if (c_ptr == NULL || getPointer(c_ptr->map, identifier) == NULL) {
        return;
    }

    BlockPointer* origin = (BlockPointer*)getOrigin(c_ptr->map);
    BlockPointer* target_block_ptr = (BlockPointer*)getPointer(c_ptr->map, identifier);

    if (target_block_ptr == origin) {
        if (getNext(origin) == NULL) {
            
            pointerFree(origin, origin);
            removeEntry(c_ptr->map, identifier);
            c_ptr->block_pointer = NULL;
            return;

        } else {
            return;
        }
    }

    if (target_block_ptr == c_ptr->block_pointer) {
        c_ptr->block_pointer = getPrevious(target_block_ptr, origin);
    }

    pointerFree(target_block_ptr, origin);
    removeEntry(c_ptr->map, identifier);

    if (getNext(origin) == NULL) {
        c_ptr->block_pointer = NULL;
    }

}

void contiguousRealloc(ContiguousPointer* c_ptr, DataType type, int new_units, char* identifier){
    
}

void storeData(ContiguousPointer* c_ptr, char* identifier, void* data){
    
    if( c_ptr == NULL || identifier == NULL || data == NULL){
        return;
    }

    if(getPointer(c_ptr -> map, identifier)== NULL){
        return;
    }

    if(getOrigin(c_ptr -> map) == getPointer(c_ptr -> map, identifier)){
        return;
    }

    pointerStoreData((BlockPointer*)(getPointer(c_ptr -> map, identifier)), data);

}

void accessData(ContiguousPointer* c_ptr, char* identifier, void* dest){

    if( c_ptr == NULL || identifier == NULL || dest == NULL){
        return;
    }

    if(getPointer(c_ptr -> map, identifier)== NULL){
        return;
    }

    pointerAccessData((BlockPointer*)(getPointer(c_ptr -> map, identifier)), dest);


}

void increaseAllocations(ContiguousPointer* c_ptr, int new_num_allocs){

    if(c_ptr == NULL || new_num_allocs <= 0){
        return;
    }

    resizeMap(c_ptr -> map, new_num_allocs);
    
}

void incrementPointer(ContiguousPointer* c_ptr){

    if(c_ptr == NULL){
        return;
    }

    if(getNext(c_ptr -> block_pointer)== NULL){
        return;
    }

    c_ptr -> block_pointer = (BlockPointer*) getNext(c_ptr -> block_pointer);
    
}

void decrementPointer(ContiguousPointer* c_ptr){
    
    if(c_ptr == NULL){
        return;
    }

    if(getPrevious(c_ptr -> block_pointer, (BlockPointer*)(getOrigin(c_ptr -> map)))== NULL){
        return;
    }

    if(getPrevious(c_ptr -> block_pointer, (BlockPointer*)(getOrigin(c_ptr -> map)))==(BlockPointer*)(getOrigin(c_ptr -> map))){
        return;
    }

    c_ptr -> block_pointer = (BlockPointer*)(getPrevious(c_ptr -> block_pointer, (BlockPointer*)(getOrigin(c_ptr -> map))));
}

void changePointer(ContiguousPointer* c_ptr, char* identifier){

    if(c_ptr == NULL || identifier == NULL){
        return;
    }

    if(getPointer(c_ptr -> map, identifier) == getOrigin(c_ptr -> map)){
        return;
    }

    c_ptr -> block_pointer = (BlockPointer*) getPointer(c_ptr -> map, identifier);
    return;
    
}

void completeDeallocation(ContiguousPointer** c_ptr){
     if (c_ptr == NULL || *c_ptr == NULL) {
        return;
    }

    BlockPointer* origin = (BlockPointer*)getOrigin((*c_ptr)->map);

    pointerCompleteDeallocate(origin);

    deallocateMap((*c_ptr)->map);

    free(*c_ptr);

    *c_ptr = NULL;




}

