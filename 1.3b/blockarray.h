#ifndef Z1_BLOCKARRAY_H
#define Z1_BLOCKARRAY_H

#define MAX_ARRAY_SIZE 1000
#define MAX_BLOCK_SIZE 100

#include <sys/time.h>
#include <sys/resource.h>

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


static_array* create_static_array (int size, int block_size);
dynamic_array* create_dynamic_array (int size, int block_size);

void delete_static_array(static_array* array);
void delete_dynamic_array(dynamic_array* array);

void remove_static(static_array* array, int index);
void remove_dynamic(dynamic_array* array, int index);

void add_static(static_array* array, int index, char* block);
void add_dynamic(dynamic_array* array, int index, char* block);

int find_block_by_sum_static(static_array* array, int sum);
int find_block_by_sum_dynamic(dynamic_array* array, int sum);


void print_static_array(static_array* array);
void print_dynamic_array(dynamic_array* array);

void print_time_result(struct timeval start, struct timeval end);
void print_result(char* function_name, struct rusage susage, struct rusage eusage, struct timeval real_start, struct timeval real_end);

char* create_block(int block_size);
void remove_block(char* block);

void time_of_create_static_array (int size, int block_size);
void time_of_create_dynamic_array(int size, int block_size);

void time_of_add_static(int amount);
void time_of_add_dynamic(int amount);

void time_of_remove_static(int amount);
void time_of_remove_dynamic(int amount);

void time_of_remove_and_add_static(int amount);
void time_of_remove_and_add_dynamic(int amount);

void time_of_find_block_by_sum_static(int sum);
void time_of_find_block_by_sum_dynamic(int sum);


#endif //Z1_BLOCKARRAY_H
