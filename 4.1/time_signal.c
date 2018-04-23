#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include "jjp_lib.h"

int flag_18;

void fSIGSTP(int signum)   {
    if(flag_18==0) {
        flag_18=1;
        sigset_t mask;
        sigfillset(&mask);
        sigdelset(&mask, SIGTSTP);
        printf("\n\nWaiting for:\n\tCtrl+Z - resume\n\tCtrl+C - exit\n");
        sigsuspend(&mask);
        
    }
    else { 
        flag_18=0;
        printf("\n\n");
    }
}

void fSIGINT()  {
    printf("\n\nProgram received signal SIGINT(2). Exiting...\n\n");
    exit(0);
}

int main(int argc, char** argv)  {
    flag_18=0;
    struct sigaction sigint_action;
    sigint_action.sa_handler = fSIGINT;
    sigemptyset(&sigint_action.sa_mask);
    sigint_action.sa_flags = 0;

    signal(SIGTSTP, fSIGSTP);
    sigaction(SIGINT, &sigint_action, NULL);

    time_t timer;
    char buffer[26];
    struct tm* tm_info;
    printf("\nPID:\t%d\n", getpid());
    while(1) {
        sleep(1);
        time(&timer);
        tm_info = localtime(&timer);

        strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
        puts(buffer);
    }

    return 0;
}