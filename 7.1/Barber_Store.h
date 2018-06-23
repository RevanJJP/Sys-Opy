#ifndef Z7_ZAKLADFRYZJERSKI_H
#define Z7_ZAKLADFRYZJERSKI_H
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h> 
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>
#include <errno.h>
#include <assert.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

#define SHARED_MEMORY_ID 5
#define SEMAPHORE_ID 3
#define FIFO_ID 2

#define CLIENTS_MAX_SIZE 1000
#define MSG_MAX_SIZE 1
#define PROJECT_ID 777


enum Semaphores {
    QUEUE_SEM=0,
    ASLEEP_SEM=1,
    SEATS_LIMIT_SEM=2,
    CURRENT_CLIENT_SEM=3,
    CLIENT_COUNTER_SEM=4,
    WAKE_UP_SEM=5,
    SHAVING_SEM=6,
    SIT_SEM=7,
    FINISH_SHAVING_SEM=8
};

typedef enum service_mtype  {
    CLIENT=100
} mtype;


typedef struct barber_state {
    int seats_limit;
    int current_client;
    int client_counter;
    int asleep;
} barber_state;

typedef struct Msg {
    long mtype;
    pid_t pid;
    //char mtext[MSG_MAX_SIZE];
} client_msg;

const char* semaphore_path = "./";
const char* shared_mem_path="/";

#endif