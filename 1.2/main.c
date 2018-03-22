#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "blockarray.h"

//printf("\n\n\nTu break\n\n\n");
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

int main(int argc, char** argv) {
    int arg=0, array_size=0, block_size=0;
    if(argc==4) {
        arg=atoi(argv[3]);
    }
    else if(argc==5)    {
        array_size=atoi(argv[3]);
        block_size=atoi(argv[4]);
    }
    else {
        printf("\nWrong arguments amount.\n");
        exit(0);
    }

    if(strlen(argv[1])==1 && strlen(argv[2])==1) {
        if (argv[2][0] == 'S' || argv[2][0] == 's')
            switch (argv[1][0]) {
                case 'C':
                case 'c':
                    time_of_create_static_array(array_size, block_size);
                    break;
                case 'S':
                case 's':
                    time_of_find_block_by_sum_static(arg);
                    break;
                case 'R':
                case 'r':
                    time_of_remove_static(arg);
                    break;
                case 'A':
                case 'a':
                    time_of_add_static(arg);
                    break;
                case 'N':
                case 'n':
                    time_of_remove_and_add_static(arg);
                    break;
                default:
                    printf("Wrong command");
                    exit(0);
            }
        else if (argv[2][0] == 'D' || argv[2][0] == 'd')
            switch (argv[1][0]) {
                case 'C':
                case 'c':
                    time_of_create_dynamic_array(array_size, block_size);
                    break;
                case 'S':
                case 's':
                    time_of_find_block_by_sum_dynamic(arg);
                    break;
                case 'R':
                case 'r':
                    time_of_remove_dynamic(arg);
                    break;
                case 'A':
                case 'a':
                    time_of_add_dynamic(arg);
                    break;
                case 'N':
                case 'n':
                    time_of_remove_and_add_dynamic(arg);
                    break;
                default:
                    printf("Wrong command");
                    exit(0);
            }
        else {
            printf("Wrong command");
            exit(0);
        }
    }
    else {
        printf("Wrong command");
        exit(0);
    }

    return 0;
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
        block[i]=(char)32 + arc4random()%95;

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
    int block_size=arc4random()%(MAX_BLOCK_SIZE-1)+1;

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
    int block_size = arc4random() % (MAX_BLOCK_SIZE - 1) + 1;

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
    int block_size=arc4random()%(MAX_BLOCK_SIZE-1)+1;

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
    int block_size=arc4random()%(MAX_BLOCK_SIZE-1)+1;

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
    int block_size=arc4random()%(MAX_BLOCK_SIZE-1)+1;

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
        block_size=arc4random()%(MAX_BLOCK_SIZE-1)+1;

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

    int block_size=arc4random()%(MAX_BLOCK_SIZE-1)+1;
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
    int  block_size = arc4random() % (MAX_BLOCK_SIZE - 1) + 1;

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
