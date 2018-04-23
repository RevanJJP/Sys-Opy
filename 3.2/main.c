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

int main(int argc, char** argv) {
    if(argc!=2) error("main", "Wrong arguments", 1);

    FILE* file = fopen(argv[1], "r+");
    if(file==NULL)  error("main", "Can't open the file", 2);
    
    char*** args=malloc(max_lines*sizeof(char**));
    
    int lines=lineread(file, args);
    if(lines<0) error("main", "Can't read the file", 3);

    int PID, close_code;

    for(int i=0; i<lines; i++) {
        PID=fork();
        if(PID==0) { 
            if(execvp(args[i][0], args[i])==-1) warning("main", "Couldn't execute a funtion", 4);
            exit(0);
        }
        wait(&close_code);
    }
    free(args);
    return 0;
}

