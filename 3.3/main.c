#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <string.h>
#include <dirent.h>
#include <ftw.h>
#include <sys/wait.h>
#include "interpreter.h"
#include "jjp_time.h"

int main(int argc, char** argv) {
    if(argc!=4) error("main", "Wrong arguments", 1);
    
    long time_lim = strtol(argv[2], NULL, 10);
    long size_lim = strtol(argv[3], NULL, 10) * 1024 * 1024;
    
    FILE* file = fopen(argv[1], "r+");
    if(file==NULL)  error("main", "Can't open the file", 2);
    
    char*** args=malloc(max_lines*sizeof(char**));
    
    int lines=lineread(file, args);
    if(lines<0) error("main", "Can't read the file", 3);

    int PID, close_code;
    struct rusage susage, eusage, dusage;

    for(int i=0; i<lines; i++) {
        getrusage(RUSAGE_CHILDREN, &susage);
        PID=fork(); 
        if(PID==0) { 
            struct rlimit cpu_limit;
            cpu_limit.rlim_max=(rlim_t)time_lim;
            cpu_limit.rlim_cur=(rlim_t)time_lim;
            if (setrlimit(RLIMIT_CPU, &cpu_limit)!=0)  error("Main", "Couldn't set this time limit", 4);

            struct rlimit mem_limit;
            mem_limit.rlim_max = (rlim_t) size_lim;
            mem_limit.rlim_cur = (rlim_t) size_lim;
            if (setrlimit(RLIMIT_AS, &mem_limit) != 0) error("Main", "Couldn't set this memory limit", 5);

            if(execvp(args[i][0], args[i])==-1) warning("main", "Couldn't execute a funtion", 6);

            exit(0);
        }
        else {

            wait(&close_code);
            getrusage(RUSAGE_CHILDREN, &dusage);
            getrusage(RUSAGE_CHILDREN, &eusage);
            printf("\nFunction %s in line %d:\n", args[i][0], i+1);
            print_result(susage, eusage);
        }
    }
    free(args);
    return 0;
}

