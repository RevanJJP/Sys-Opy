#include "jjp_lib.h"

void print_time_result(struct timeval start, struct timeval end) {
    long sec = end.tv_sec - start.tv_sec;
    long microsec = end.tv_usec - start.tv_usec;

    if(microsec<0) {
        sec-=1;
        microsec+=1000000;
    }

    printf("%ld.%06lds", sec, microsec);
}

void print_result(struct rusage susage, struct rusage eusage) {
    struct timeval user_start = susage.ru_utime;
    struct timeval user_end = eusage.ru_utime;
    struct timeval system_start = susage.ru_stime;
    struct timeval system_end = eusage.ru_stime;

    printf("User time:\t");
    print_time_result(user_start, user_end);
    printf("\nSystem time:\t");
    print_time_result(system_start, system_end);
    printf("\n");
}

void error(char* function_name, char* error_meesage, int error_code) {
    printf("\nERROR %d in function '%s':\n%s.\n", error_code, function_name, error_meesage);
    exit(error_code);
}

void warning(char* function_name, char* warning_meesage, int warning_code) {
    printf("\nWARNING %d in function '%s':\n%s.\n", warning_code, function_name, warning_meesage);
}