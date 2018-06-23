#include "service.h"

void login(key_t private_queue_key);
void mirror_handler(Msg *msg, char* const chain);
void time_handler(Msg *msg);
void calc_handler(Msg *msg, char* const chain);
void handle_sigint(int sig_num, siginfo_t *info, void *context);
int create_queue(int id);
void close_queue();
void end_handler(Msg *msg);

int private_id=-1;
int session_id=-2;
int descriptor=-1;

int main (int argc, char** argv) {
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = handle_sigint;
    sigaction(SIGINT, &act, NULL);

    if(atexit(close_queue)==-1) {
        perror("CLIENT::Exit funtion load fail");
        exit(EXIT_FAILURE);
    }

    descriptor=create_queue(PROJECT_ID);

    key_t private_key = ftok(getenv("HOME"), getpid());
    if(private_key==-1) {
        perror("CLIENT::Getting private key failed");
        exit(EXIT_FAILURE);
    }

    private_id= msgget(private_key, IPC_CREAT | IPC_EXCL | S_IRWXU | S_IRWXG | S_IRWXO);
    if(private_id==-1) {
        perror("CLIENT::Creating private queue failed");
        exit(EXIT_FAILURE);
    }

    login(private_key);

    char command[8];
    char chain[MSG_MAX_SIZE];
    Msg msg;
    while(1) {
        msg.pid=getpid();
        scanf("%s", command);
        if (strcmp(command, "END") == 0) {
            printf("\nEnding...\n");
            end_handler(&msg);
            break;
        }
        else if (strcmp(command, "MIRROR") == 0) {
            scanf("%s", chain);
            mirror_handler(&msg, chain);
        }
        else if(strcmp(command, "CALC") == 0) {
            scanf("%s", chain);
            calc_handler(&msg, chain);
        }
        else if(strcmp(command, "TIME") == 0) {
            time_handler(&msg);
        }
        else {
            printf("\nWrong command!\n");
            continue;
        }
    }
    return 0;
}

void handle_sigint(int sig_num, siginfo_t *info, void *context) {
    printf("\nClient received SIGINT. Finishing...\n");
    exit(0);
}

void login(key_t private_key) {
    Msg msg;
    msg.mtype=LOGIN;
    msg.pid=getpid();
    sprintf(msg.mtext, "%d", private_key);

    if(msgsnd(descriptor, &msg, MSG_MAX_SIZE, 0) == -1) { 
        perror("CLIENT::Login fail");
        exit(EXIT_FAILURE);
    }
    if(msgrcv(private_id, &msg, MSG_MAX_SIZE, 0, 0) == -1) { 
        perror("CLIENT::Couldn't catch server login response");
        exit(EXIT_FAILURE);
    }
    if(sscanf(msg.mtext, "%d", &session_id) < 1) { 
        perror("CLIENT::Login message corrupted");
        exit(EXIT_FAILURE);
    }
    if(session_id<0) { 
        perror("CLIENT: Server limit reached");
        exit(EXIT_FAILURE);
    }
    printf("\nClient logged in. Session no %d\n", session_id);
}

void mirror_handler(Msg *msg, char* chain) {
    if(strlen(chain)>MSG_MAX_SIZE) {
        printf("CLIENT::String for MIRROR is too big\n");
        return;
    }
    
    msg->mtype=MIRROR;
    strcpy(msg->mtext, chain);

    if(msgsnd(descriptor, msg, MSG_MAX_SIZE, 0) == -1)  {
        perror("CLIENT: MIRROR request fail");
        exit(EXIT_FAILURE);
    }
    if(msgrcv(private_id, msg, MSG_MAX_SIZE, 0, 0) == -1)  {
        perror("CLIENT: MIRROR response fail");
        exit(EXIT_FAILURE);
    }

    printf("%s\n", msg->mtext);
}

void time_handler(Msg *msg) {
    msg->mtype=TIME;

    if(msgsnd(descriptor, msg, MSG_MAX_SIZE, 0) == -1)  {
        perror("CLIENT: TIME request fail");
        exit(EXIT_FAILURE);
    }
    if(msgrcv(private_id, msg, MSG_MAX_SIZE, 0, 0) == -1)  {
        perror("CLIENT: TIME response fail");
        exit(EXIT_FAILURE);
    }

    printf("%s\n", msg->mtext);
}

void calc_handler(Msg *msg, char* const chain) {
    if(strlen(chain)>MSG_MAX_SIZE) {
        printf("CLIENT::CALC message is too big\n");
        return;
    }
    
    msg->mtype=CALC;
    strcpy(msg->mtext, chain);

    if(msgsnd(descriptor, msg, MSG_MAX_SIZE, 0) == -1)  {
        perror("CLIENT: CALC request fail");
        exit(EXIT_FAILURE);
    }
    if(msgrcv(private_id, msg, MSG_MAX_SIZE, 0, 0) == -1)  {
        perror("CLIENT: CALC response fail");
        exit(EXIT_FAILURE);
    }

    printf("%s\n", msg->mtext);
}

int create_queue(int id) {
    int key=ftok(getenv("HOME"), id);
    if(key==-1) {
        perror("CLIENT::Queue key generation fail");
        exit(EXIT_FAILURE);
    }

    int queue_id=msgget(key,0);
    if(queue_id==-1) {
        perror("CLIENT::Queue opening fail");
        exit(EXIT_FAILURE);
    }

    return queue_id;
}

void close_queue() {
    if(private_id > -1) {
        if(msgctl(private_id, IPC_RMID, NULL)==-1) {
            perror("CLIENT::Deleting queue fail");
            exit(EXIT_FAILURE);
        }
    }
}

void end_handler(Msg *msg) {
    msg->mtype=END;

    if(msgsnd(descriptor, msg, MSG_MAX_SIZE, 0)==-1){
        perror("CLIENT::Ending queue fail");
        exit(EXIT_FAILURE);
    }
}