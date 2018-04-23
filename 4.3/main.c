#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include "jjp_lib.h"

pid_t PID;
int signals, sent, signals_received, child_signals_received;

void print_results() {
    printf("Signals sent to child: %d\n", sent);
    printf("Signals received by parent: %d\n", signals_received);
}

void handle_sigusr1(int sig_num, siginfo_t *info, void *context) {
    signals_received++;
}

void handle_sigint() {
    printf("Received SIGINT\n");
    kill(PID, SIGUSR2);
    
    waitpid(PID, &child_signals_received, 0);
    print_results();
    exit(0);
}


int main(int argc, char** argv) {
    if (argc != 3) error("main", "wrong number of arguments", 1);
    signals = (int) strtol(argv[1], NULL, 10);
    int mode = (int) strtol(argv[2], NULL, 10);

    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;

    act.sa_sigaction = handle_sigint;
    sigaction(SIGINT, &act, NULL);
 
    signals_received=0; 
    child_signals_received=0;

    PID = fork();
    if (PID==0) {
        switch(mode) {
            case 1:
            execl("./child", "./child", "1", NULL);
            break;
            case 2:
            execl("./child", "./child", "2", NULL);
            break;
            default:
            error("main", "wrong arguments", 2);
        }
    } 
    else {
        sleep(1);
        sigset_t mask;
        sigfillset(&mask);
        
        act.sa_sigaction = handle_sigusr1;
        sigaction(SIGUSR1, &act, NULL);
        sigdelset(&mask, SIGUSR1);
        sigdelset(&mask, SIGUSR2); 
       

        if (mode==1) {
            printf("Sending SIGUSR1 to child...\n");
            for (sent=0; sent<signals; sent++) {
                kill(PID, SIGUSR1);
            }
        }
        else if (mode==2) {
            printf("Sending SIGUSR1 to child...\n");
            for (sent=0; sent<signals; sent++) {
                kill(PID, SIGUSR1);
                sigsuspend(&mask);
            }
        }

        printf("Sending final signal (SIGUSR2)...\n");
        kill(PID, SIGUSR2);
    }
    
    waitpid(PID, &child_signals_received, 0);
    print_results();
    exit(0);
}
