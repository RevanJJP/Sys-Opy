#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include "jjp_lib.h"

int received;


void handle_sigusr1(int sig_num, siginfo_t *info, void *context) {
    received++;
    kill(getppid(), SIGUSR1);
}

void handle_sigusr2(int sig_num, siginfo_t *info, void *context) {
    printf("Child received SIGUSR2. Finishing...\nSignals received by child: %d\n", received);
    exit(received);
}

void handle_sigint(int sig_num, siginfo_t *info, void *context) {
    printf("Child received SIGINT. Finishing...\nSignals received by child: %d\n", received);
    exit(received);
}

int main(int argc, char** argv) {
    received=0;
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;
    
    act.sa_sigaction = handle_sigint;
    sigaction(SIGINT, &act, NULL);
    act.sa_sigaction = handle_sigusr1;
    sigaction(SIGUSR1, &act, NULL);
    act.sa_sigaction = handle_sigusr2;
    sigaction(SIGUSR2, &act, NULL);

    sigset_t mask;
    sigfillset(&mask);

    sigdelset(&mask, SIGUSR1);
    sigdelset(&mask, SIGUSR2);

    while(1) { pause();}
}