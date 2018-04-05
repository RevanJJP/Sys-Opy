#include <sys/time.h>
#include <sys/resource.h>
#include <string.h>
#include "iosys.h"

void print_time_result(struct timeval start, struct timeval end);
void time_of_copy_function(char* function_name, void (f)(char*, char*, int, int), char* source_path, char* destination_path, int rows, int row_size);
void time_of_sort_function(char* function_name, void (f)(char*, int, int), char* source_path, int rows, int row_size);
void time_tests(char* path, char* destination_path, int row, int row_size);

int main(int argc, char** argv)  {
    if(argc!=5 && argc!=6) error("main", "Wrong number of arguments", 1);
    
    if(argc==5) {
        switch (argv[1][0]) {
            case 'G':
            case 'g':
                if(strlen(argv[1])!=1) error("main", "Wrong arguments", 2);
                else generate(argv[2], atoi(argv[3]), atoi(argv[4]));
                break;
            case 'S':
            case 's':
                if(strlen(argv[1])!=2) error("main", "Wrong arguments", 3);
                else switch(argv[1][1])  {
                        case 'S':
                        case 's':
                            sort_sys(argv[2], atoi(argv[3]), atoi(argv[4]));
                            break;
                        case 'L':
                        case 'l':
                            sort_lib(argv[2], atoi(argv[3]), atoi(argv[4]));
                            break;
                        default:
                            error("main", "Wrong arguments", 4);
                }
            break;
            default:
                error("main", "Wrong arguments", 5);
        }
    }
        
    if(argc==6) {
        switch (argv[1][0]) {
            case 'C':
            case 'c':
                if(strlen(argv[1])!=2) error("main", "Wrong arguments", 6);
                else switch(argv[1][1])  {
                        case 'S':
                        case 's':
                            copy_sys(argv[2], argv[3], atoi(argv[4]), atoi(argv[5]));
                            break;
                        case 'L':
                        case 'l':
                            copy_lib(argv[2], argv[3], atoi(argv[4]), atoi(argv[5]));
                            break;
                        default:
                            error("main", "Wrong arguments", 7);
                    }
                break;
            case 'T':
            case 't':
                if(strlen(argv[1])!=1) error("main", "Wrong arguments", 8);
                else time_tests(argv[2], argv[3], atoi(argv[4]), atoi(argv[5])); 
                break;
            default:
                error("main", "Wrong arguments", 9);
        }
    }
    return 0;
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

void print_result(char* function_name, struct rusage susage, struct rusage eusage) {
    struct timeval user_start = susage.ru_utime;
    struct timeval user_end = eusage.ru_utime;
    struct timeval system_start = susage.ru_stime;
    struct timeval system_end = eusage.ru_stime;

    printf(" %s", function_name);
    printf("\nUser time:\t");
    print_time_result(user_start, user_end);
    printf("\nSystem time:\t");
    print_time_result(system_start, system_end);
    printf("\n\n");
}

void time_of_copy_function(char* function_name, void (f)(char*, char*, int, int), char* source_path, char* destination_path, int rows, int row_size) {
    struct rusage susage, eusage;
    getrusage(RUSAGE_SELF, &susage);

    f(source_path, destination_path, rows, row_size);
    
    getrusage(RUSAGE_SELF, &eusage);
    print_result(function_name, susage, eusage);
}

void time_of_sort_function(char* function_name, void (f)(char*, int, int), char* source_path, int rows, int row_size) {
    struct rusage susage, eusage;
    getrusage(RUSAGE_SELF, &susage);

    f(source_path, rows, row_size);
    
    getrusage(RUSAGE_SELF, &eusage);
    print_result(function_name, susage, eusage);
}

void time_tests(char* source_path, char* destination_path, int rows, int row_size) {
    generate(source_path, rows, row_size);
    time_of_copy_function("System copying", copy_sys, source_path, destination_path, rows, row_size);
    time_of_copy_function("Library copying", copy_lib, source_path, destination_path, rows, row_size);
    time_of_sort_function("System sort", sort_sys, source_path, rows, row_size);
    time_of_sort_function("Library sort", sort_lib, destination_path, rows, row_size);
}