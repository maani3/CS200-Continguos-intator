#include "../src/block.c"
#include <time.h>

int testInitialization(){
    int marks = 0;
    char* data = "HelloWorld";
    Block* block_01 = initializeBlock(CHAR_TYPE, 11, data);
    Block* block_0x = initializeBlock(CHAR_TYPE, 0, data);

    if(block_01->bytes == (sizeof(char) * 11) && !strcmp(data, ((char*)block_01->mem_space)) && block_0x == NULL){
        marks += 1;
    }

    free(block_01->mem_space);
    free(block_01);


    printf(" %d/%d\n", marks, 1);
    return marks;
}

int testMalloc(){
    int marks = 0;

    Block* block_1 = blockMalloc(DOUBLE_TYPE, 1);
    Block* block_2 = blockMalloc(CHAR_TYPE, 1);

    Block* block_x = blockMalloc(DOUBLE_TYPE, 0);
    if(block_1->bytes == sizeof(double) && block_1->mem_space && block_2->bytes == sizeof(char) && block_2->mem_space && !block_x){
        marks += 1;
    }

    blockFree(block_1);
    blockFree(block_2);

    Block* block_3 = blockMalloc(INT_TYPE, 10);
    
    /*
    If the below case fails, it means that the actual allocated
    memory is not of integer size. Hence, storing and then consequently 
    accessing it as an integer gives value mismatch.
    */
    int valid = 1;
    if(block_3->bytes == 10 * sizeof(int) && block_3->mem_space){
        for(int i = 0; i < 10; i++){
            ((int*)block_3->mem_space)[i] = i;
        }
        
        for(int i = 0; i < 10; i++){
            if(((int*)block_3->mem_space)[i] != i){
                valid = 0;
                break;
            }
        }
    }
    else{
        valid = 0;
    }

        if(valid){
        marks += 1;
    }


    blockFree(block_3);

    printf(" %d/%d\n", marks, 2);
    return marks;
}

int testStoreAccess(){
    int marks = 0;
    double b1 = (double)rand()/(double)RAND_MAX;
    char b2 = 'a' + (rand() % 26);
    float b3 = (float)rand()/(float)RAND_MAX;
    
    //units storage + access
    Block* block_1 = blockMalloc(DOUBLE_TYPE, 1);
    Block* block_2 = blockMalloc(CHAR_TYPE, 1);
    Block* block_3 = blockMalloc(FLOAT_TYPE, 1);

    blockStoreData(block_1, &b1);
    blockStoreData(block_2, &b2);
    blockStoreData(block_3, &b3);
    blockStoreData(NULL, &b3);
    blockStoreData(block_1, NULL);

    if(*((double*)block_1->mem_space) == b1 && *((char*)block_2->mem_space) == b2 && *((float*)block_3->mem_space) == b3){
        marks += 1;
    }

    double b1_temp;
    char b2_temp;
    float b3_temp;

    blockAccessData(block_1, &b1_temp, DOUBLE_TYPE, 1);
    blockAccessData(block_2, &b2_temp, CHAR_TYPE, 1);
    blockAccessData(block_3, &b3_temp, FLOAT_TYPE, 1);
    blockAccessData(block_2, NULL, CHAR_TYPE, 1);
    blockAccessData(NULL, &b3_temp, FLOAT_TYPE, 1);

    if(b1_temp == b1 && b2_temp == b2 && b3_temp == b3){
        marks += 1;
    }

    blockFree(block_3);

    //Size mismatch
    //i) dest < src
    int temp1[3] = {__INT_MAX__, __INT_MAX__, __INT_MAX__};

    blockAccessData(block_1, &temp1[1], INT_TYPE, 1);

    int corrupt_bit = 0;
    if(marks < 0 || marks > 3 || temp1[0] != __INT_MAX__ || temp1[2] != __INT_MAX__){
        corrupt_bit = 1;
        marks = 0;
    }

    blockFree(block_1);

    if(corrupt_bit){
        printf("\033[31m Stack corrupted\033[0m");
    }

    //ii) src < dest
    int temp2 = __INT_MAX__;

    blockAccessData(block_2, &temp2, INT_TYPE, 1);
    
    // printf("\n temp2: %x | %x", __INT_MAX__, temp2);
    
    if(temp2 == __INT_MAX__){
        marks -= 1;
    }

    blockFree(block_2);

    //Non-unit storage + access
    Block* block_4 = blockMalloc(INT_TYPE, 20);
    int store_b4[20];
    int access_b4[20];

    for(int i = 0; i < 20; i++){
        store_b4[i] = i;
    }

    blockStoreData(block_4, store_b4);

    int valid = 1;
    for(int i = 0; i < 20; i++){
        if(((int*)block_4->mem_space)[i] != store_b4[i]){
            valid = 0;
        }
    }

    blockAccessData(block_4, access_b4, INT_TYPE, 20);

    for(int i = 0; i < 20; i++){
        if(access_b4[i] != ((int*)block_4->mem_space)[i]){
            valid = 0;
        }
    }

    if(valid){
        marks += 1;
    }
    
    blockFree(block_4);
    
    printf(" %d/%d\n", marks, 3);
    return marks;
}

int testRealloc(){
    int marks = 0;
    double b1 = (double)rand()/(double)RAND_MAX;
    char b2[6] = "cs200";
    int b3 = __INT_MAX__;
    
    Block* block_1 = blockMalloc(DOUBLE_TYPE, 1);
    Block* block_2 = blockMalloc(CHAR_TYPE, 6);
    Block* block_3 = blockMalloc(INT_TYPE, 1);

    blockStoreData(block_1, &b1);
    blockStoreData(block_2, b2);
    blockStoreData(block_3, &b3);

    blockRealloc(block_1, DOUBLE_TYPE, 10);
    blockRealloc(block_2, CHAR_TYPE, 2);
    blockRealloc(block_3, INT_TYPE, 0);

    //Checking block 1
    if(((double*)block_1->mem_space)[0] == b1 && block_1->bytes == (sizeof(double) * 10)){
        marks += 1;
    }

    for(int i = 0; i < 10; i++){
        ((double*)block_1->mem_space)[i] = (double)rand()/(double)RAND_MAX;
    }

    double b1_realloc[10];
    blockAccessData(block_1, b1_realloc, DOUBLE_TYPE, 10);

    int valid = 1;
    for(int i = 0; i < 10; i++){
        if(((double*)block_1->mem_space)[i] != b1_realloc[i]){
            valid = 0;
        }
    }

    if(valid){
        marks += 1;
    }

    //Checking block 2 + 3
    if(((char*)block_2->mem_space)[0] == b2[0] && ((char*)block_2->mem_space)[1] == b2[1] && block_2->bytes == (sizeof(char) * 2) && *((int*)block_3->mem_space) == b3 && block_3->bytes == sizeof(int)){
        marks += 2;
    }

    blockFree(block_1);
    blockFree(block_2);
    blockFree(block_3);

    printf(" %d/%d\n", marks, 4);

    return marks;
}

int main(){

    printf("\033[1m\033[36m\nTesting Block:\n\n\033[0m");
    srand(time(NULL));
    int total = 0;

    printf("\033[35mTesting initialization:\033[0m");
    total += testInitialization();

    printf("\033[33mblock_free() will be used from now\033[0m\n");

    printf("\033[35mTesting malloc:\033[0m");
    total += testMalloc();
    
    printf("\033[35mTesting data storage & access:\033[0m");
    total += testStoreAccess();

    printf("\033[35mTesting reallocation:\033[0m");
    total += testRealloc();

    FILE *file = fopen("test/temporary.txt", "w");
    if (file == NULL) {
        perror("Runtime error - rerun program");
        return 1;
    }

    fprintf(file, "%d\n%d", total, 11);
    fclose(file);
}