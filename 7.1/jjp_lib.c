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

void print_time_msec(){
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    t.tv_nsec/=1000;
    
    if((t.tv_sec/3600)%24<10) printf("0");
    printf("%ld:", (t.tv_sec/3600)%24);
    if((t.tv_sec/60)%60<10) printf("0");
    printf("%ld:", (t.tv_sec/60)%60);
    if((t.tv_sec)%60<10) printf("0");
    printf("%ld.", (t.tv_sec)%60);
    int i=100000;
    while(t.tv_nsec<i) {
        printf("0");
        i/=10;
    }
    printf("%ld", t.tv_nsec);
}

void ERROR(int error_code, char* error_meesage) {
    perror(error_meesage);
    exit(error_code);
}