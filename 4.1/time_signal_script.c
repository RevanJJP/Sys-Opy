#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include "jjp_lib.h"


int process_id;

void fSIGSTP(int signum)   {
    if(process_id!=0) {
        kill(process_id, SIGKILL);
        printf("\n\nWaiting for:\n\tCtrl+Z - start new process\n\tCtrl+C - exit\n");
    }
    else {
        process_id=-1;
    }

}

void fSIGINT()  {
    if(process_id!=0) kill(process_id, SIGKILL);
    printf("\n\nProgram received signal SIGINT(2). Exiting...\n\n");
    exit(0);
}


int main()  {
    int status;
    printf("\nPID:\t%d\n", getpid());
    process_id=0;
    struct sigaction sigint_action;
    sigint_action.sa_handler = fSIGINT;
    sigemptyset(&sigint_action.sa_mask);
    sigint_action.sa_flags = 0;
    signal(SIGTSTP, fSIGSTP);
    sigaction(2, &sigint_action, NULL);
    
    while(1) { 
        process_id=fork();
        if(process_id==0) {
            printf("\nStart child process -> PID:\t%d\n", getpid());
            if (execl("./script.sh", "./script.sh", NULL) == -1) error("Main", "Cannot run script", 2);
        }
        else    {
            wait(&status);
            process_id=0;
            while(process_id==0){}
        }
    }
}