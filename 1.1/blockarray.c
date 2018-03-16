#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "blockarray.h"

static_array* create_static_array (int size, int block_size) {
    if(size <=0 || size > MAX_ARRAY_SIZE || block_size <= 0 || block_size > MAX_BLOCK_SIZE) {
        printf("Dimension ERROR.\n");
        return NULL;
    }

    static_array* array = calloc(1, sizeof(struct static_array));
    array -> size = size;
    array -> block_size = block_size;

    for(int i=0; i<size; i++)
        for(int j=0; j<block_size; j++)
            array->array[i][j]=0;
    return array;
}

dynamic_array* create_dynamic_array(int size, int block_size)   {
    if(size <0 || block_size <= 0) {
        printf("Dimension ERROR.\n");
        return NULL;
    }

    dynamic_array* array = calloc(1, sizeof(struct static_array));
    array -> array = calloc(1, sizeof(char *));
    array -> size = size;
    array -> block_size = block_size;

    return array;
}

void delete_static_array(struct static_array* array) {
    for(int i=0; i<array->size; i++)
        for(int j=0; j<array->block_size; j++)
            array->array[i][j]=0;

    array->size=0;
    array->block_size=0;
}

void delete_dynamic_array(struct dynamic_array* array)  {
    for(int i=0; i<array->size; i++)
        free(array->array[i]);
    array->size=0;
    array->block_size=0;
    free(array->array);
    free(array);
}

void remove_static(struct static_array* array, int index) {
    for (int i = 0; i < array->block_size; i++)
        array->array[index][i]=0;
}

void remove_dynamic(struct dynamic_array* array, int index) {
    if(array==NULL)
        return;
    free(array->array[index]);
    array->array[index]=0;
}

void add_static(struct static_array* array, int index, char* block) {
    if(index<0 || index >=array->size) {
        printf("Wrong INDEX\n");
        return;
    }
    if(block == NULL || strlen(block) >= array->block_size) {
        printf("Wrong BLOCK SIZE\n");
        return;
    }

    if(array->array[index] != NULL)
        remove_static(array, index);
    strcpy(array ->array[index], block);
}

void add_dynamic(struct dynamic_array* array, int index, char* block)   {
    if(array == NULL) {
        printf("Array doesn't exist\n");
        return;
    }
    if(index < 0 || index >= array->size)   {
        printf("Wrong INDEX\n");
        return;
    }
    if(block == NULL || strlen(block) >= array->block_size) {
        printf("Wrong BLOCK SIZE\n");
        return;
    }

    if(array -> array[index] != NULL)
        remove_dynamic(array, index);

    array -> array[index] = realloc(array->array[index], array->block_size * sizeof(char));
    strcpy(array ->array[index], block);
}

int sum_block(const char* block, int block_size)    {
    if(block==NULL)
        return 0;

    int sum=0;

    for (int i = 0; i < block_size; i++)    {
            sum += (int) block[i];
    }
    return sum;
}

int find_block_by_sum_static(struct static_array* array, int sum) {
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


    for (int i = 1; i < array->size; i++) {
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

int find_block_by_sum_dynamic(struct dynamic_array* array, int sum) {
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


    for (int i = 1; i < array->size; i++) {
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
