//
// Created by Jakub Posłuszny on 14.03.2018.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "blockarray.h"

static_array* create_static_array (int size, int block_size) {
    if(size <=0 || size > MAX_ARRAY_SIZE || block_size <= 0 || block_size > MAX_BLOCK_SIZE) {
        printf("Dimension ERROR.\n");
        return NULL;
    }

    static_array* array = calloc(1, sizeof(static_array));
    array -> size = size;
    array -> block_size = block_size;

    for(int i=0; i<size; i++)
        for(int j=0; j<block_size; j++)
            array->array[i][j]=0;
    return array;
}

dynamic_array* create_dynamic_array(int size, int block_size)   {
    if(size<0 || block_size<=0) {
        printf("Dimension ERROR.\n");
        return NULL;
    }

    dynamic_array* array = calloc(1, sizeof(dynamic_array));
    array -> array = calloc(1, sizeof(char *));
    array -> size = size;
    array -> block_size = block_size;
    array -> array = (char**) calloc((size_t)size, sizeof(char*));


    return array;
}

void delete_static_array(static_array* array) {
    for(int i=0; i<array->size; i++)
        for(int j=0; j<array->block_size; j++)
            array->array[i][j]=0;

    array->size=0;
    array->block_size=0;
}

void delete_dynamic_array(dynamic_array* array)  {
    for(int i=0; i<array->size; i++)
        free(array->array[i]);
    array->size=0;
    array->block_size=0;
    free(array->array);
    free(array);
}

void remove_static(static_array* array, int index) {
    for (int i=0; i<array->block_size; i++)
        array->array[index][i]=0;
}

void remove_dynamic(dynamic_array* array, int index) {
    if(array==NULL)
        return;
    free(array->array[index]);
    array->array[index]=0;
}

void add_static(static_array* array, int index, char* block) {
    if(index<0 || index>=array->size) {
        printf("Wrong INDEX\n");
        return;
    }
    if(block == NULL || strlen(block) > array->block_size) {
        printf("Wrong BLOCK SIZE\n");
        return;
    }

    /*if(array->array[index]!=NULL)
        remove_static(array, index);*/
    strcpy(array ->array[index], block);
}

void add_dynamic(dynamic_array* array, int index, char* block)   {
    if(array==NULL) {
        printf("Array doesn't exist\n");
        return;
    }
    if(index<0 || index>=array->size)   {
        printf("Wrong INDEX\n");
        return;
    }
    if(block==NULL || strlen(block)>array->block_size) {
        printf("Wrong BLOCK SIZE\n");
        return;
    }

/*    if(array->array[index]!=NULL)
        remove_dynamic(array, index);*/

    array -> array[index] = realloc(array->array[index], array->block_size * sizeof(char));
    strcpy(array ->array[index], block);
}

int sum_block(const char* block, int block_size)    {
    if(block==NULL)
        return 0;

    int sum=0;

    for (int i=0; i<block_size; i++)    {
            sum += (int) block[i];
    }
    return sum;
}

int find_block_by_sum_static(static_array* array, int sum) {
    if (array == NULL)  {
        printf("Array doesn't exist\n");
        return -1;
    }

    int temp =sum_block(array->array[0], array->block_size);
    int dif;
    int index=0;

    if(temp<sum)
         dif=sum-temp;
    else
        dif=temp -sum;


    for (int i=1; i < array->size; i++) {
        temp = sum_block(array->array[i], array->block_size);

        if(temp<sum)
            temp=sum-temp;
        else
            temp=temp-sum;

        if(temp<dif)    {
            dif=temp;
            index=i;
        }
    }

    return index;
}

int find_block_by_sum_dynamic(dynamic_array* array, int sum) {
    if (array==NULL)  {
        printf("Array doesn't exist\n");
        return -1;
    }

    int temp =sum_block(array->array[0], array->block_size);
    int dif;
    int index=0;

    if(temp<sum)
        dif=sum-temp;
    else
        dif=temp -sum;

    for (int i=1; i<array->size; i++) {
        temp = sum_block(array->array[i], array->block_size);

        if(temp<sum)
            temp=sum-temp;
        else
            temp=temp-sum;

        if(temp<dif)    {
            dif=temp;
            index=i;
        }
    }

    return index;
}

void print_static_array(struct static_array* array) {
    if(array==NULL) {
        printf("Array doesn't exist!");
        return;
    }
    for (int i = 0; i < array->size; i++) {
        printf("%d: %s", i, array->array[i]);
        printf("\n");
    }
}

void print_dynamic_array(struct dynamic_array* array) {
    if(array==NULL) {
        printf("Array doesn't exist!");
        return;
    }
    for (int i=0; i< array -> size; i++) {
        printf("%d: %s", i, array->array[i]);
        printf("\n");
    }
}

void print_time_result(struct timeval start, struct timeval end) {
    long sec = end.tv_sec - start.tv_sec;
    long microsec = end.tv_usec - start.tv_usec;

    if(microsec<0) {
        sec-=1;
        microsec+=1000000;
    }

    printf("%ld.%06lds", sec, microsec);
}

void print_result(char* function_name, struct rusage susage, struct rusage eusage, struct timeval real_start, struct timeval real_end) {
    struct timeval user_start = susage.ru_utime;
    struct timeval user_end = eusage.ru_utime;
    struct timeval system_start = susage.ru_stime;
    struct timeval system_end = eusage.ru_stime;

    printf(" %s", function_name);
    printf("\nReal time:\t");
    print_time_result(real_start, real_end);
    printf("\nUser time:\t");
    print_time_result(user_start, user_end);
    printf("\nSystem time:\t");
    print_time_result(system_start, system_end);
    printf("\n\n");
}

char* create_block(int block_size) {
    char* block = calloc(block_size, sizeof(char));

    for(int i=0; i<block_size; i++)
        block[i]=(char)32 + rand()%95;

    return block;
}
void remove_block(char* block) {
    if(block==NULL)
        return;
    free(block);
}

static_array* create_random_static_array(int size, int block_size) {
    static_array* array = create_static_array(size, block_size);

    for(int i=0; i<size; i++) {
        char* block =create_block(block_size - 1);
        add_static(array, i, block);
        remove_block(block);
    }
    return array;
}

dynamic_array* create_random_dynamic_array(int size, int block_size) {

    dynamic_array* array = create_dynamic_array(size, block_size);

    for(int i=0; i<size; i++) {
        char* block =create_block(block_size - 1);
        add_dynamic(array, i, block);
        remove_block(block);
    }

    return array;
}

void time_of_create_static_array (int size, int block_size) {
    struct timeval real_start, real_end;
    struct rusage susage, eusage;

    gettimeofday(&real_start, NULL);
    getrusage(RUSAGE_SELF, &susage);

    static_array* array = create_static_array(size, block_size);

    gettimeofday(&real_end, NULL);
    getrusage(RUSAGE_SELF, &eusage);

    delete_static_array(array);

    print_result("Create static array", susage, eusage, real_start, real_end);
}

void time_of_create_dynamic_array (int size, int block_size) {
    struct timeval real_start, real_end;
    struct rusage susage, eusage;

    gettimeofday(&real_start, NULL);
    getrusage(RUSAGE_SELF, &susage);

    dynamic_array* array = create_dynamic_array(size, block_size);

    gettimeofday(&real_end, NULL);
    getrusage(RUSAGE_SELF, &eusage);

    delete_dynamic_array(array);

    print_result("Create dynamic array", susage, eusage, real_start, real_end);
}

void time_of_add_static(int amount) {
    struct timeval real_start, real_end;
    struct rusage susage, eusage;
    int block_size=rand()%(MAX_BLOCK_SIZE-1)+1;

    static_array* array = create_random_static_array(amount, block_size);
    char* block=create_block(array->block_size);

    gettimeofday(&real_start, NULL);
    getrusage(RUSAGE_SELF, &susage);

    for(int i=0; i<amount; i++)
        add_static(array, i, block);

    gettimeofday(&real_end, NULL);
    getrusage(RUSAGE_SELF, &eusage);

    delete_static_array(array);

    printf("%dx ", amount);
    print_result("add to static array", susage, eusage, real_start, real_end);
}

void time_of_add_dynamic(int amount) {
    struct timeval real_start, real_end;
    struct rusage susage, eusage;
    int block_size;
    do {
        block_size = rand() % (MAX_BLOCK_SIZE - 1) + 1;
    }while((block_size)%16==0);

    dynamic_array* array = create_random_dynamic_array(amount, block_size);
    char* block=create_block(array->block_size);

    gettimeofday(&real_start, NULL);
    getrusage(RUSAGE_SELF, &susage);

    for(int i=0; i<amount; i++)
        add_dynamic(array, i, block);

    gettimeofday(&real_end, NULL);
    getrusage(RUSAGE_SELF, &eusage);

    delete_dynamic_array(array);

    printf("%dx ", amount);
    print_result("add to dynamic array", susage, eusage, real_start, real_end);
}

void time_of_remove_static(int amount) {
    struct timeval real_start, real_end;
    struct rusage susage, eusage;
    int block_size=rand()%(MAX_BLOCK_SIZE-1)+1;

    static_array* array = create_random_static_array(amount, block_size);

    gettimeofday(&real_start, NULL);
    getrusage(RUSAGE_SELF, &susage);

    for(int i=0; i<amount; i++)
        remove_static(array, i);

    gettimeofday(&real_end, NULL);
    getrusage(RUSAGE_SELF, &eusage);

    delete_static_array(array);

    printf("%dx ", amount);
    print_result("remove from static array", susage, eusage, real_start, real_end);
}

void time_of_remove_dynamic(int amount) {
    struct timeval real_start, real_end;
    struct rusage susage, eusage;
    int block_size;
    do {
        block_size=rand()%(MAX_BLOCK_SIZE-1)+1;
    } while((block_size-1)%16==0);

    dynamic_array* array = create_random_dynamic_array(amount, block_size);

    gettimeofday(&real_start, NULL);
    getrusage(RUSAGE_SELF, &susage);

    for(int i=0; i<amount; i++)
        remove_dynamic(array, i);

    gettimeofday(&real_end, NULL);
    getrusage(RUSAGE_SELF, &eusage);

    delete_dynamic_array(array);

    printf("%dx ", amount);
    print_result("remove from dynamic array", susage, eusage, real_start, real_end);
}

void time_of_remove_and_add_static(int amount) {
    struct timeval real_start, real_end;
    struct rusage susage, eusage;
    int block_size=rand()%(MAX_BLOCK_SIZE-1)+1;

    static_array* array = create_random_static_array(amount, block_size);
    char* block=create_block(array->block_size);

    gettimeofday(&real_start, NULL);
    getrusage(RUSAGE_SELF, &susage);

    for(int i=0; i<amount; i++) {
        remove_static(array, i);
        add_static(array, i, block);
    }

    delete_static_array(array);

    gettimeofday(&real_end, NULL);
    getrusage(RUSAGE_SELF, &eusage);

    delete_static_array(array);

    printf("%dx ", amount);
    print_result("add and remove to static array", susage, eusage, real_start, real_end);
}

void time_of_remove_and_add_dynamic(int amount) {
    struct timeval real_start, real_end;
    struct rusage susage, eusage;
    int block_size;
    do{
        block_size=rand()%(MAX_BLOCK_SIZE-1)+1;
    } while((block_size-1)%16==0);

    dynamic_array* array = create_random_dynamic_array(amount, block_size);
    char* block=create_block(array->block_size);

    gettimeofday(&real_start, NULL);
    getrusage(RUSAGE_SELF, &susage);

    for(int i=0; i<amount; i++) {
        remove_dynamic(array, i);
        add_dynamic(array, i, block);
    }

    delete_dynamic_array(array);

    gettimeofday(&real_end, NULL);
    getrusage(RUSAGE_SELF, &eusage);

    printf("%dx ", amount);
    print_result("add and remove from dynamic array", susage, eusage, real_start, real_end);
}

void time_of_find_block_by_sum_static(int sum) {
    struct timeval real_start, real_end;
    struct rusage susage, eusage;

    int block_size=rand()%(MAX_BLOCK_SIZE-1)+1;
    static_array* array = create_random_static_array(MAX_ARRAY_SIZE, block_size);

    gettimeofday(&real_start, NULL);
    getrusage(RUSAGE_SELF, &susage);

    find_block_by_sum_static(array, sum);

    gettimeofday(&real_end, NULL);
    getrusage(RUSAGE_SELF, &eusage);

    delete_static_array(array);

    printf("In %d lines", MAX_ARRAY_SIZE);
    print_result("find block by sum in static array", susage, eusage, real_start, real_end);
}

void time_of_find_block_by_sum_dynamic(int sum) {
    struct timeval real_start, real_end;
    struct rusage susage, eusage;
    int block_size;
    do {
        block_size = rand() % (MAX_BLOCK_SIZE - 1) + 1;
    } while((block_size-1)%16==0);

    dynamic_array* array = create_random_dynamic_array(MAX_ARRAY_SIZE, block_size);

    gettimeofday(&real_start, NULL);
    getrusage(RUSAGE_SELF, &susage);

    find_block_by_sum_dynamic(array, sum);

    gettimeofday(&real_end, NULL);
    getrusage(RUSAGE_SELF, &eusage);

    delete_dynamic_array(array);

    printf("In %d lines", MAX_ARRAY_SIZE);
    print_result("find block by sum in static array", susage, eusage, real_start, real_end);
}
