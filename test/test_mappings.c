#include "../src/mappings.c"

int testInitialize(Mappings* map, char* origin){
    int marks = 0;
    int valid = 1;

    if(((void*)map[0].address) == (void*)0x10000000 && !strcmp(map[0].identifier, origin)){
        marks += 1;
    }

    for(int i = 1; i < 4; i++){
        if(map[i].address != 0 || map[i].identifier != NULL){
            valid = 0;
            break;
        }
    }  

    if(valid){
        marks += 1;
    }

    printf(" %d/%d\n", marks , 2);

    return marks;
}

int testMake(Mappings* map, char* identifiers[], void* addresses[]){
    int marks = 0;

    for(int i = 1; i < 4; i++){
        makeEntry(&map, addresses[i], identifiers[i]);
    }

    int valid = 1;
    for(int i = 0; i < 4; i++){
        if(strcmp(map[i].identifier, identifiers[i]) != 0 || map[i].address != (uintptr_t)addresses[i]){
            valid = 0;
            break;
        }
    }
    
    if(valid){
        marks += 3;
    }

    printf(" %d/%d\n", marks, 3);

    return marks;
}

int testResize(Mappings** map, char* identifiers[], void* addresses[]){
    int marks = 0;

    makeEntry(map, addresses[4], identifiers[4]);
    makeEntry(map, addresses[5], identifiers[5]);

    int valid = 1;
    for(int i = 0; i < 6; i++){
        if((void*)(*map)[i].address != addresses[i] || strcmp((*map)[i].identifier, identifiers[i]) != 0){
            valid = 0;
            break;
        }
    }

    if(valid){
        marks += 2;
    }

    makeEntry(map, addresses[6], identifiers[6]); //resize occurs

    for(int i = 7; i < 11; i++){
        makeEntry(map, addresses[i], identifiers[i]);
    }

    valid = 1;
    for(int i = 0; i < 11; i++){
        if((*map)[i].address != (uintptr_t)addresses[i] || strcmp((*map)[i].identifier, identifiers[i]) != 0){
            valid = 0;
            break;
        }
    }

    if(valid){
        marks += 2;
    }

    printf(" %d/%d\n", marks, 4);

    return marks;
}

int testRemove(Mappings* map, char* identifiers[], char* identifiers_test[], void* addresses_test[]){
    int marks = 0;

    for(int i = 0; i < 11; i++){
        if(i == 0 || i%2){
            removeEntry(map, identifiers[i]);
        }
    }

    int valid = 1;
    for(int i = 0; i < 6; i++){
        if(strcmp(map[i].identifier, identifiers_test[i]) != 0 || map[i].address != (uintptr_t)addresses_test[i]){
            valid = 0;
        }
    }

    if(valid){
        marks += 2;
    }

    valid = 1;
    for(int i = 6; i < 11; i++){
        if(map[i].identifier != NULL || map[i].address != 0){
            valid = 0;
        }
    }

    if(valid){
        marks += 2;
    }

    printf(" %d/%d\n", marks, 4);

    return marks;
}

int testGet(Mappings* map, char* identifiers[], void* addresses[]){
    int marks = 0;
    void* origin = getOrigin(map);

    if(origin == addresses[0]){
        marks += 1;
    }

    int valid = 1;
    for(int i = 0; i < 6; i++){
        if(getPointer(map, identifiers[i]) != addresses[i]){
            valid = 0;
            break;
        }
    }

    if(valid){
        marks += 1;
    }

    printf(" %d/%d\n", marks, 2);

    return marks;
}

int main(){
    printf("\033[1m\033[36m\nTesting Mappings:\n\n\033[0m");
    int total = 0;
    char* identifiers[11] = {"origin", "element1", "element2", "element3", "element4", "element5", "element6", "element7", "element8", "element9", "element10"};
    void* addresses[11] = {(void*)0x10000000, (void*)0x1000000a, (void*)0x10000014, (void*)0x1000001e, (void*)0x10000028, (void*)0x10000032, (void*)0x1000003c, (void*)0x10000046, (void*)0x10000050, (void*)0x1000005a, (void*)0x10000064};
    char* identifiers_test[6] = {"origin", "element2", "element4", "element6", "element8", "element10"};
    void* addresses_test[6] = {(void*)0x10000000, (void*)0x10000014, (void*)0x10000028, (void*)0x1000003c, (void*)0x10000050, (void*)0x10000064};

    printf("\033[35mTesting initialization:\033[0m");
    Mappings* map = initializeMap(3, addresses[0], identifiers[0]);
    total += testInitialize(map, identifiers[0]);

    printf("\033[35mTesting make entry:\033[0m");
    total += testMake(map, identifiers, addresses);

    printf("\033[35mTesting resizing:\033[0m");
    map = resizeMap(map, 5);
    total += testResize(&map, identifiers, addresses);

    printf("\033[35mTesting remove entry:\033[0m");
    total += testRemove(map, identifiers, identifiers_test, addresses_test);

    printf("\033[35mTesting get origin & pointer:\033[0m");
    total += testGet(map, identifiers_test, addresses_test);

    printf("\033[33musing deallocate_map() at the end\033[0m\n");

    deallocateMap(map);

    FILE *file = fopen("test/temporary.txt", "w");
    if (file == NULL) {
        perror("Runtime error - rerun program");
        return 1;
    }

    fprintf(file, "%d\n%d", total, 16);
    fclose(file);
}