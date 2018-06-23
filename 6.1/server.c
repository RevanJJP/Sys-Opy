#include "service.h"

void end_handler(Msg *msg);
void login_handler(Msg *msg);
void mirror_handler(Msg *msg);
void time_handler(Msg *msg);
void calc_handler(Msg *msg);
void handle_sigint(int sig_num, siginfo_t *info, void *context);
int create_queue(int id);
void close_queue();
int create_message(Msg *msg);
char* time_converter(const time_t *mtime);
int find_queue(pid_t pid);


void handle_sigint(int sig_num, siginfo_t *info, void *context) {
    printf("\nSERVER received SIGINT. Finishing...\n");
    exit(0);
}

int descriptor=-2;
int active=1;

int clients[MAX_CLIENTS][2];
int client_counter=0;

int main (int argc, char** argv) {
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = handle_sigint;
    sigaction(SIGINT, &act, NULL);

    if(atexit(close_queue)==-1) {
        perror("SERVER::Exit funtion load fail");
        exit(EXIT_FAILURE);
    }

    struct msqid_ds current;

    key_t public_key = ftok(getenv("HOME"), PROJECT_ID);
    if(public_key==-1) {
        perror("SERVER::Getting public key fail");
        exit(EXIT_FAILURE);
    }

    descriptor=msgget(public_key, IPC_CREAT | IPC_EXCL | S_IRWXU | S_IRWXG | S_IRWXO);
    if(descriptor==-1) {
        perror("SERVER::Creating queue fail");
        exit(EXIT_FAILURE);
    }

    Msg msg;
    while(1) {
        if (active==0) {
            if (msgctl(descriptor, IPC_STAT, &current) == -1) {
                perror("SERVER:: Getting state of queue fail");
                exit(EXIT_FAILURE);
            }
            if (current.msg_qnum==0) break;
        }

        if(msgrcv(descriptor, &msg, MSG_MAX_SIZE, 0, 0)<0) {
            perror("SERVER::Receiving message fail");
            exit(EXIT_FAILURE);
        }
        switch(msg.mtype) {
            case LOGIN:
                login_handler(&msg);
                break;
            case MIRROR:
                mirror_handler(&msg);
                break;
            case CALC:
                calc_handler(&msg);
                break;
            case TIME:
                time_handler(&msg);
                break;
            case END:
                end_handler(&msg);
                break;
            default:
                break;
        }
    }
    return 0;
}

void login_handler(Msg *msg) {
    key_t client_queue;
    if (sscanf(msg->mtext, "%d", &client_queue) < 0) {
        perror("SERVER::Eeading queue fail");
        exit(EXIT_FAILURE);
    }

    int client_queue_id = msgget(client_queue, 0);
    if (client_queue_id == -1)  {
        perror("SERVER: reading client_queue_id failed");
        exit(EXIT_FAILURE);
    }

    int client_pid = msg->pid;
    msg->mtype = LOGIN;
    msg->pid = getpid();

    if (client_counter > MAX_CLIENTS - 1) {
        printf("server: maximum number of clients reached\n");
        sprintf(msg->mtext, "%d", -1);
    } else {
        clients[client_counter][0] = client_pid;
        clients[client_counter++][1] = client_queue_id;
        sprintf(msg->mtext, "%d", client_counter-1);
    }

    if (msgsnd(client_queue_id, msg, MSG_MAX_SIZE, 0) == -1) {
        perror("SERVER: LOGIN response fail");
        exit(EXIT_FAILURE);
    }
}

void mirror_handler(Msg *msg)   {
    int client_queue=create_message(msg);
    if(client_queue==-1) return;
    int msg_length=strlen(msg->mtext);
    char buffer[msg_length];
    if(msg->mtext[msg_length-1]=='\n') msg_length--;
    for(int i=0; i<msg_length; i++) {
        buffer[i]=msg->mtext[msg_length-i-1];
    }
    int i=4;
    while(msg_length<i) {
        buffer[i-1]='\0';
        i--;
    }
    sprintf(msg->mtext, "%s", buffer);

    if(msgsnd(client_queue, msg, MSG_MAX_SIZE, 0)==-1) {
        perror("SERVER::MIRROR fail");
        exit(EXIT_FAILURE);
    }

}

void calc_handler(Msg *msg) {
    int client_queue=create_message(msg);
    if(client_queue==-1) return;

    char sign;
    int msg_length=strlen(msg->mtext);
    int j=0;
    int numbers[2];
    numbers[0]=0;
    numbers[1]=0;
    for(int i=0; i<msg_length && j<2; i++) {
        if(msg->mtext[i]>=48 && msg->mtext[i]<=57) {
            numbers[j]=numbers[j]*10+(int)(msg->mtext[i]-48);
        }
        else {
            j++;
            sign=msg->mtext[i];
        }
    }
    double result=0;

    switch(sign) {
        case '+':
            result=(double)numbers[0]+numbers[1];
            break;
        case '-':
            result=(double)numbers[0]-numbers[1];
            break;
        case '*':
            result=(double)numbers[0]*numbers[1];
            break;
        case '/':
            result=(double)numbers[0]/numbers[1];
            break;
        default:
            printf("SERVER::CALC fail");
    }
    snprintf(msg->mtext, MSG_MAX_SIZE, "%f", result);

    if(msgsnd(client_queue, msg, MSG_MAX_SIZE, 0)==-1) {
        perror("SERVER::CALC fail");
        exit(EXIT_FAILURE);
    }
}

void time_handler(Msg *msg) {
    int client_queue=create_message(msg);
    if(client_queue==-1) return;

    time_t timer;
    time(&timer);
    snprintf(msg->mtext, 30, "%s", time_converter(&timer));

    if(msgsnd(client_queue, msg, MSG_MAX_SIZE, 0)==-1) {
        perror("SERVER::TIME fail");
        exit(EXIT_FAILURE);
    }
}

int create_message(Msg *msg) {
    int client_queue=find_queue(msg->pid);
    if(client_queue==-1) {
        printf("SERCER::Couldn't find the client\n");
        return -1;
    }

    msg->mtype=msg->pid;
    msg->pid=getpid();

    return client_queue;
}

int find_queue(pid_t pid) {
    for(int i=0; i<MAX_CLIENTS; i++) {
        if(clients[i][0]==pid)
            return clients[i][1];
    }
    return -1;
}

void close_queue() {
    if(descriptor>-1) {
        int tmp = msgctl(descriptor, IPC_RMID, NULL);
        if(tmp==-1) printf("SERVER::Deleting queue fail");
    }
}

char* time_converter(const time_t *mtime) {
    char* buffer=malloc(sizeof(char)*30);
    struct tm *timeinfo;
    timeinfo=localtime (mtime);
    strftime(buffer, 20, "%b %d %H:%M", timeinfo);
    return buffer;
}

void end_handler(Msg *_) {active=0;}