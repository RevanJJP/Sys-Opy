#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include "jjp_lib.h"

int N, M, counter, m_counter;
pid_t* children;
pid_t* process_awaiting;


void handle_sigrt(int sig_n, siginfo_t *act, void *context) {
    printf("SIGRT SIGMIN+%i from child PID: %d.\n", sig_n-SIGRTMIN, act->si_pid);
}

void handle_sigusr1(int sig_n, siginfo_t *act, void *context)   {
    printf("Child %d asked for permission.\n", act->si_pid);
    
    if (m_counter<M) {
        process_awaiting[m_counter] = act->si_pid;
        m_counter++;
        if (m_counter == M) {
            for (int i=0; i<m_counter; i++) {
                if (process_awaiting[i]>0) {
                    printf("Child %d granted permission to run.\n", process_awaiting[i]);
                    kill(process_awaiting[i], SIGUSR2);
                    waitpid(process_awaiting[i], NULL, 0);
                }
            }
        }
    } 
    else if (m_counter>=M) {
        printf("Child %d granted permission to run.\n", act->si_pid);
        kill(act->si_pid, SIGUSR2);
        waitpid(act->si_pid, NULL, 0);
    }
}

void handle_sigchld(int sig_n, siginfo_t *act, void *context) {
    if (act->si_code == CLD_EXITED) {
        printf("Child process %d finished. Exit: %d.\n", act->si_pid, act->si_status);
    } else {
        printf("Child process %d terminated.\n", act->si_pid);
    }

    for (int  i=0; i<N; i++) {
        if (children[i] == act->si_pid) {
            children[i] = 0;
        }
    }
    
    counter--;
    if(counter==0) printf("All childred processes finished working\n");
}

void handle_sigint(int sig_n, siginfo_t *info, void *context) {
    printf("SIGINT.\n");
    for (int i = 0; i<N; i++) if (children[i] !=0) kill(children[i], SIGKILL);
    free(children);
    exit(0);
}

int main(int argc, char** argv)  {
    if(argc!=3) error("Main", "Wrong number of arguments", 1);
    N = (int) strtol(argv[1], NULL, 10);
    M = (int) strtol(argv[2], NULL, 10);
    counter=0;
    m_counter=0;
    if(M>N) error("Main", "Wrong arguments", 2);

    children=calloc(N, sizeof(int));
    process_awaiting=calloc(N, sizeof(int));

    for (int i=0; i<N; i++) children[i]=0;
    for (int i=0; i<M; i++) process_awaiting[i]=0;

    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;

    act.sa_sigaction = handle_sigint;
    sigaction(SIGINT, &act, NULL);

    act.sa_sigaction = handle_sigusr1;
    sigaction(SIGUSR1, &act, NULL);

    act.sa_sigaction = handle_sigchld;
    sigaction(SIGCHLD, &act, NULL);

    act.sa_sigaction = handle_sigrt;
    for (int i=SIGRTMIN; i<=SIGRTMAX; i++) {
        sigaction(i, &act, NULL);
    }

   // printf("\nbr 1 %d\n", getpid());

    pid_t PID=0;
    for(int i=0; i<N; i++) {
        counter++;
        //printf("\nbr 2\n");
        PID=fork();
        if(PID==0) {
            if (execl("./child", "./child", NULL) == -1) error("Main", "Cannot start child process", 2);
            return 0;
        }
        else {
            //printf("\nbr 3\n");
            children[i]=PID;
        }
    }
    while(counter>0) {}
    free(children);
    return 0;
}