#include "../src/pointer.c"

int testInitialization(BlockPointer* origin, DataType type, int units, char* data){
    int marks = 0;

    if(origin->type == type && origin->units == units && origin->offset == 0 && origin->next == NULL && getBytes(origin->block) == (int)(sizeof(char) * units) && !strcmp(data, ((char*)getMemSpace(origin->block)))){
        marks += 1;
    }

    printf(" %d/%d\n", marks, 1);

    return marks;
}

int testMalloc(BlockPointer* origin, BlockPointer* pointer_1, BlockPointer* pointer_2, BlockPointer* pointer_3, DataType* types){
    int marks = 0;
    int valid_1 = 1;
    int valid_2 = 1;
    int i = 0;
    BlockPointer* temp = origin;
    
    BlockPointer* pointer_4 = pointerMalloc(INT_TYPE, 0, NULL);

    if(origin->next == pointer_1 && pointer_1->next == pointer_2 && pointer_2->next == pointer_3 && pointer_3->next == NULL && pointer_4 == NULL){
        marks += 2;
    }

    while(temp != NULL){
        if(temp->type != types[i]){
            valid_1 = 0;
            break;
        }

        if(temp->offset != i){
            valid_2 = 0;
            break;
        }

        i++;
        temp = temp->next;
    }

    if(valid_1){
        marks += 1;
    }

    if(valid_2){
        marks += 1;
    }

    printf(" %d/%d\n", marks, 4);

    return marks;
}

int testFree(BlockPointer* origin, BlockPointer* pointer_1, BlockPointer* pointer_2, BlockPointer* pointer_3, DataType* types){
    int marks = 0;
    int valid_link = 1;
    int valid_offset = 1;
    BlockPointer* pointer_4 = pointerMalloc(types[4], 2, origin);
    BlockPointer* pointer_5 = pointerMalloc(types[5], 4, origin);
    BlockPointer* pointer_6 = pointerMalloc(types[6], 11, origin);
    BlockPointer* pointer_7 = pointerMalloc(types[7], 3, origin);
    BlockPointer* pointer_8 = pointerMalloc(types[8], 5, origin);
    BlockPointer* pointer_9 = pointerMalloc(types[9], 1, origin);

    pointerFree(pointer_7, origin);
    
    if(pointer_6->next != pointer_8){
        valid_link = 0;
    }

    if(pointer_8->offset != 7 || pointer_9->offset != 8){
        valid_offset = 0;
    }

    pointerFree(pointer_5, origin);

    if(pointer_4->next != pointer_6){
        valid_link = 0;
    }

    if(pointer_6->offset != 5 || pointer_8->offset != 6 || pointer_9->offset != 7){
        valid_offset = 0;
    }

    pointerFree(pointer_8, origin);

    if(pointer_6->next != pointer_9){
        valid_link = 0;
    }

    if(pointer_9->offset != 6){
        valid_offset = 0;
    }
    
    pointerFree(pointer_9, origin);

    if(pointer_6->next != NULL){
        valid_link = 0;
    }

    pointerFree(pointer_6, origin);

    if(pointer_4->next != NULL){
        valid_link = 0;
    }

    if(pointer_4->offset != 4){
        valid_offset = 0;
    }

    pointerFree(pointer_4, origin);

    if(origin->next != pointer_1 || pointer_1->next != pointer_2 || pointer_2->next != pointer_3 || pointer_3->next != NULL){
        valid_link = 0;
    }

    if(valid_link){
        marks += 2;
    }

    if(valid_offset){
        marks += 2;
    }

    pointerFree(pointer_3, NULL);

    printf(" %d/%d\n", marks, 4);

    return marks;
}

int testStoreAccess(BlockPointer* pointer_1, BlockPointer* pointer_2, BlockPointer* pointer_3){
    int marks = 0;
    char p1[] = "HelloWorld";
    float p2 = (float)rand()/(float)RAND_MAX;
    double p3[] = {(double)rand()/(double)RAND_MAX, (double)rand()/(double)RAND_MAX};

    pointerStoreData(pointer_1, &p1);
    pointerStoreData(pointer_2, &p2);
    pointerStoreData(pointer_3, &p3);

    if(!strcmp(p1, ((char*)getMemSpace(pointer_1->block))) && *((float*)getMemSpace(pointer_2->block)) == p2 && (((double*)getMemSpace(pointer_3->block))[0] == p3[0] && ((double*)getMemSpace(pointer_3->block))[1] == p3[1])){
        marks += 1;
    }

    char p1_access[11];
    float p2_access;
    double p3_access[2];

    pointerAccessData(pointer_1, p1_access);
    pointerAccessData(pointer_2, &p2_access);
    pointerAccessData(pointer_3, p3_access);

    if(!strcmp(p1_access, p1) && p2_access == p2 && (p3_access[0] == p3[0] && p3_access[1] == p3[1])){
        marks += 1;
    }

    printf(" %d/%d\n", marks, 2);
    
    return marks;
}

int testRealloc(BlockPointer* origin, BlockPointer* pointer_1, BlockPointer* pointer_2, BlockPointer* pointer_3){
    int marks = 0;
    int valid = 1;
    char p1[5] = {'H', 'e', 'l', 'l', 'o'};
    pointerRealloc(pointer_1, origin, CHAR_TYPE, 5);
    double p3 = ((double*)getMemSpace(pointer_3->block))[0];
    pointerRealloc(pointer_3, origin, DOUBLE_TYPE, 1);

    if(pointer_1->offset == 1 && pointer_1->next == pointer_2 && pointer_1->units == 5 && pointer_3->offset == 3 && pointer_3->next == NULL && pointer_3->units == 1){
        marks += 1;
    }

    for(int i = 0; i < 5; i++){
        if(((char*)getMemSpace(pointer_1->block))[i] != p1[i]){
            valid = 0;
            break;
        }
    }

    if(*((double*)getMemSpace(pointer_3->block)) != p3){
        valid = 0;
    }

    BlockPointer* pointer_4 = pointerMalloc(INT_TYPE, 1, origin);
    BlockPointer* pointer_5 = pointerMalloc(CHAR_TYPE, 3, origin);

    pointerRealloc(pointer_4, origin, INT_TYPE, 0);

    if(valid && pointer_3->next == pointer_4 && pointer_4->next == pointer_5 && pointer_4->offset == 4 && pointer_4->units == 1){
        marks += 1;
    }

    pointerFree(pointer_4, origin);
    pointerFree(pointer_5, origin);

    printf(" %d/%d\n", marks, 2);
    
    return marks;
}

int testNextPrev(BlockPointer* origin, BlockPointer* pointer_1, BlockPointer* pointer_2, BlockPointer* pointer_3){
    int marks = 0;

    BlockPointer* origin_next = getNext(origin);
    BlockPointer* p1_next = getNext(pointer_1);
    BlockPointer* p2_next = getNext(pointer_2);
    BlockPointer* p3_next = getNext(pointer_3);

    if(origin_next == pointer_1 && p1_next == pointer_2 && p2_next == pointer_3 && p3_next == NULL){
        marks += 1;
    }

    BlockPointer* origin_prev = getPrevious(origin, origin);
    BlockPointer* p1_prev = getPrevious(pointer_1, origin);
    BlockPointer* p2_prev = getPrevious(pointer_2, origin);
    BlockPointer* p3_prev = getPrevious(pointer_3, origin);

    if(origin_prev == NULL && p1_prev == origin && p2_prev == pointer_1 && p3_prev == pointer_2){
        marks += 1;
    }

    printf(" %d/%d\n", marks, 2);

    return marks;
}

int main(){

    printf("\033[1m\033[36m\nTesting Block Pointer:\n\n\033[0m");

    int total = 0;
    DataType types[10] = {CHAR_TYPE, CHAR_TYPE, FLOAT_TYPE, DOUBLE_TYPE, INT_TYPE, INT_TYPE, CHAR_TYPE, DOUBLE_TYPE, FLOAT_TYPE, CHAR_TYPE};

    printf("\033[35mTesting initialization:\033[0m");
    char* data = "CS200";
    int units = 6;
    BlockPointer* origin = initializeBlockPointer(types[0], units, data);
    total += testInitialization(origin, types[0], units, data);

    BlockPointer* pointer_1 = pointerMalloc(types[1], 11, origin);
    BlockPointer* pointer_2 = pointerMalloc(types[2], 1, origin);
    BlockPointer* pointer_3 = pointerMalloc(types[3], 2, origin);

    printf("\033[35mTesting malloc:\033[0m");
    total += testMalloc(origin, pointer_1, pointer_2, pointer_3, types);

    printf("\033[33mpointer_free() will be used from now on\033[0m\n");

    printf("\033[35mTesting free:\033[0m");
    total += testFree(origin, pointer_1, pointer_2, pointer_3, types);

    printf("\033[35mTesting store & access data:\033[0m");
    total += testStoreAccess(pointer_1, pointer_2, pointer_3);

    printf("\033[35mTesting realloc:\033[0m");
    total +=  testRealloc(origin, pointer_1, pointer_2, pointer_3);

    printf("\033[35mTesting get next & previous:\033[0m");
    total += testNextPrev(origin, pointer_1, pointer_2, pointer_3);

    pointerCompleteDeallocate(origin);

    FILE *file = fopen("test/temporary.txt", "w");
    if (file == NULL) {
        perror("Runtime error - rerun program");
        return 1;
    }

    fprintf(file, "%d\n%d", total, 18);
    fclose(file);
}