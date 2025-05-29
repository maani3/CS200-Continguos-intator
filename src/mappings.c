#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct Mappings {
    char* identifier;
    uintptr_t address;

} Mappings;


Mappings* initializeMap(int num_allocations, void* addr, char* identifier){

    if(num_allocations <= 0 || addr == NULL || identifier == NULL){
        return NULL;
    }

    Mappings* new_map = (Mappings *)calloc(num_allocations + 2, sizeof(Mappings));

    if(new_map == NULL){
        return NULL;
    }

    new_map[0].identifier = identifier;
    new_map[0].address = (uintptr_t)addr;

    new_map[num_allocations + 1].identifier = identifier;
    new_map[num_allocations + 1].address = (uintptr_t)addr;

    return new_map;

   
}

void deallocateMap(Mappings* map){

    if(map == NULL){
        return;
    }

    free(map);
    return;

}

Mappings* resizeMap(Mappings* map, int new_num_allocs){

    if(map == NULL){
        return NULL;
    }

    Mappings* larger_map = initializeMap(new_num_allocs, (void*)map[0].address, map[0].identifier);
    
    if(larger_map == NULL){
        return NULL;
    }

    Mappings* current_map = map + 1;
    int length = 1;

    while(current_map -> identifier != map -> identifier){
        length++;
        current_map++;
    }

    for(int i = 0 ; i < length; i++){
        larger_map[i] = map[i];
    }


    deallocateMap(map);

    return larger_map;
    
}

void makeEntry(Mappings** map, void* addr, char* identifier){

    if( map == NULL || addr == NULL || identifier == NULL){
        return;
    }

    Mappings* start_map = *(map);
    Mappings* current_map = start_map + 1 ;
    int inserted = 0;
    int length = 0;
    int unique = 1;

    while(current_map -> identifier != NULL && (current_map -> identifier != start_map -> identifier)){

        if(current_map -> identifier == identifier){
            unique = 0;
            return;
        }
        current_map++;
    }

    current_map = start_map + 1;

    while(current_map != NULL && (current_map -> identifier != start_map -> identifier)){

        if(current_map -> identifier == NULL && current_map -> address == 0 && unique == 1){
            current_map -> identifier = identifier;
            current_map -> address = (uintptr_t)addr;
            inserted = 1;
            break;
        }

        current_map++;
        length++;
    }

    if (current_map -> identifier == start_map -> identifier && inserted == 0){
        *map = resizeMap(*map, 2*length);
        makeEntry(map, addr, identifier);
    }

}

void removeEntry(Mappings* map, char* identifier){

    int length = 1;
    Mappings* current_map = map + 1;
    int empty = 0;

    while(map -> identifier != current_map -> identifier && current_map != NULL){

        if(current_map -> identifier != NULL){
            empty = 1;
        }

        length++;
        current_map++;
        
    }

    length++;
    current_map = map + 1;

    if( map -> identifier == identifier && empty == 0){

        map[length-1].identifier = map[1].identifier;

        for(int i = 0; i < length-2; i++){
            map[i] = map[i+1];
        }

        map[length - 2].identifier = NULL;
        map[length - 2].address = 0;
        return;
    }

    while(current_map -> identifier != map -> identifier && current_map != NULL){
        if(current_map -> identifier == identifier){

        for( int j = (int)(current_map - map); j < length - 2  ; j ++){

            map[j] = map[j+1];

        }

        map[length - 2].identifier = NULL;
        map[length - 2].address = 0; 
    }
    current_map++;
    }

    

    return;
    
}

void* getOrigin(Mappings* map){

    if(map == NULL || map -> identifier == NULL){
        return NULL;
    }

    return((void*)(map -> address));


}

void* getPointer(Mappings* map, char* identifier){

    if(identifier == NULL){
        return NULL;
    }
    Mappings* current_map = map + 1;

    if(strcmp(map -> identifier, identifier) == 0){
        return ((void*)(map -> address));
    }

    while(current_map -> identifier != map -> identifier && current_map -> identifier != NULL){
        if(strcmp(current_map -> identifier, identifier) == 0){
            return ((void*)(current_map -> address));
        }
        current_map++;
    }

    if(current_map -> identifier == map -> identifier){
        return NULL;
    }

    return NULL;
    
}








/*
------------------------------------------------------------------------------
DO NOT EDIT THE FUNCTIONS BELOW - TESTING PURPOSES
*/

uintptr_t getAddress(Mappings* map, int index){
    return map[index].address;
}

char* getIdentifier(Mappings* map, int index){
    return map[index].identifier;
}

