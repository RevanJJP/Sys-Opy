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
#include "jjp_lib.h"

int main(int argc, char** argv) {
    if(argc!=2) error("main", "Wrong arguments", 1);

    FILE* file=fopen(argv[1], "rb+");
    if(file==NULL)  error("main", "Can't open the file", 2);
    
    char*** args=malloc(max_lines*sizeof(char**));
    
    int lines=lineread(file, args);
    if(lines<0) error("main", "Can't read the file", 3);

    int PID, flag, close_code;
    // for(int i=0; i<lines; i++) {
    //     int j=0;
    //     if(args[i][0]==NULL) printf("NULL");
    //     while(args[i][j]!=NULL) {
    //         printf("%s ", args[i][j]);
    //         j++;
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    int fd[2][2];

    for(int i=0; i<lines; i++) {
        flag=0;
        while(args[i][0]!=NULL) {
            if(args[i+1][0]!=NULL) pipe(fd[1]);
            PID=fork();
            if(PID==0) { 
                if (flag!=0) {
                    dup2(fd[0][0], STDIN_FILENO);
                    close(fd[0][1]);
                }
                if (args[i+1][0]!=NULL) {
                    dup2(fd[1][1], STDOUT_FILENO);
                    close(fd[1][0]);
                }
                if(execvp(args[i][0], args[i])==-1) warning("main", "Couldn't execute a funtion", 4);
                exit(0);
            }
            if (flag!=0) close(fd[0][0]);
            if (args[i+1][0]!=NULL) close(fd[1][1]);
            flag=1;
            fd[0][0]=fd[1][0];
            fd[0][1]=fd[1][1];
            i++;
        }
        while(wait(&close_code)>0);
    }
    free(args);
    fclose(file);
    return 0;
}
