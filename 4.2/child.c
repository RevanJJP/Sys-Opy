#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include "jjp_lib.h"

int exit_flag=0;

void send_rt_signal()   {
    //printf("\nbr 8 %d\n",(int) getpid());
    kill(getppid(), SIGRTMIN+(rand()%(SIGRTMAX-SIGRTMIN)));
    exit_flag=1;
}


int main(int argc, char** argv)  {
    signal(SIGUSR2, &send_rt_signal);



    srand(getpid());
    int seconds=rand()%10+1;
    //printf("\nbr 4 %d\n",(int) getpid());
    sleep(seconds);
   // printf("\nbr 5 %d\n",(int) getpid());
    kill(getppid(), SIGUSR1);
   // printf("\nbr 6 %d\n",(int) getpid());
   // printf("\nbr 7 %d\n",(int) getpid());
    while(exit_flag==0) {}
    return seconds;
}