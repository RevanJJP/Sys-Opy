#ifndef Z6_SERVICE_H
#define Z6_SERVICE_H
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h> 
#include <sys/ipc.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>
#include <assert.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include "jjp_lib.h"

#define MSG_MAX_SIZE 32
#define PROJECT_ID 0x099
#define MAX_CLIENTS 20

typedef enum service_mtype  {
    LOGIN=300,
    END=301,
    
    MIRROR=101,
    CALC=102,
    TIME=103
} mtype;

typedef struct Msg {
    long mtype;
    pid_t pid;
    char mtext[MSG_MAX_SIZE];
} Msg;

#endif