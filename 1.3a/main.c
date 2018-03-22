#include <dlfcn.h>

#define MAX_ARRAY_SIZE 1000
#define MAX_BLOCK_SIZE 100

#ifndef DLL
#include "blockarray.h"
#endif

#ifdef DLL
typedef struct static_array  {
    char array[MAX_ARRAY_SIZE][MAX_BLOCK_SIZE];
    int size;
    int block_size;
} static_array;

typedef struct dynamic_array {
    char** array;
    int size;
    int block_size;
} dynamic_array;

#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

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

    #ifdef DLL
        void* handle = dlopen("./libblockarray.so", RTLD_LAZY);

        void (*time_of_create_static_array)(int, int) = dlsym(handle, "time_of_create_static_array");
        void (*time_of_create_dynamic_array)(int, int) = dlsym(handle, "time_of_create_dynamic_array");

        void (*time_of_add_static)(int) = dlsym(handle, "time_of_add_static");
        void (*time_of_add_dynamic)(int) = dlsym(handle, "time_of_add_dynamic");

        void (*time_of_remove_static)(int) = dlsym(handle, "time_of_remove_static");
        void (*time_of_remove_dynamic)(int) = dlsym(handle, "create_static_array");

        void (*time_of_remove_and_add_static)(int) = dlsym(handle, "time_of_remove_and_add_static");
        void (*time_of_remove_and_add_dynamic)(int) = dlsym(handle, "time_of_remove_and_add_dynamic");

        void (*time_of_find_block_by_sum_static)(int) = dlsym(handle, "time_of_find_block_by_sum_static");
        void (*time_of_find_block_by_sum_dynamic)(int) = dlsym(handle, "time_of_find_block_by_sum_dynamic");
    #endif

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
