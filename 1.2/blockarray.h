#ifndef Z1_BLOCKARRAY_H
#define Z1_BLOCKARRAY_H

#define MAX_ARRAY_SIZE 1000
#define MAX_BLOCK_SIZE 100


typedef struct static_array  {
    char array[MAX_ARRAY_SIZE][MAX_BLOCK_SIZE];
    int size;
    int block_size; //same for whole array
} static_array;

typedef struct dynamic_array {
    char** array;
    int size;
    int block_size; //same for whole array
} dynamic_array;


static_array* create_static_array (int size, int block_size);                           //+
dynamic_array* create_dynamic_array (int size, int block_size);                         //+

void delete_static_array(static_array* array);                                   //+
void delete_dynamic_array(dynamic_array* array);                                 //+

void remove_static(static_array* array, int index);                              //+
void remove_dynamic(dynamic_array* array, int index);                            //+

void add_static(static_array* array, int index, char* block);                    //+
void add_dynamic(dynamic_array* array, int index, char* block);                  //+

int find_block_by_sum_static(static_array* array, int sum);                      //+
int find_block_by_sum_dynamic(dynamic_array* array, int sum);                    //+


#endif //Z1_BLOCKARRAY_H
