#include "Barber_Store.h"
#include "jjp_lib.h"

int shared_memory;
int semaphore;
int fifo=-1;
static struct sembuf buffer;
barber_state* barber;

void printc(const char* communique);
void sighandler(int signo) { exit(0);}
void visit_barber_store(int shavings);

void semaphore_inc(int semaphore_id, int sem_num);
void semaphore_dec(int semaphore_id, int sem_num);
pid_t clients[CLIENTS_MAX_SIZE];

void unlink_memory() { if(shmdt(barber)<0) ERROR(1, "CLIENT: Couldn't unlink shared memory");}

int main(int argc, char** argv) {
    if(argc!=3) ERROR(1, "Pass amount of customers and shavings");
    signal(SIGTERM, sighandler);
    signal(SIGINT, sighandler);
    
    int clients_number=atoi(argv[1]);
    int shavings=atoi(argv[2]);

    if(clients_number<=0 || shavings<=0) ERROR(1, "CLIENT: Wrong arguments");
    if(atexit(unlink_memory)!=0) ERROR(1, "CLIENT: atexit() error");

    for(int i=0; i<CLIENTS_MAX_SIZE; i++) {
        clients[i]=-1;
    }

    key_t fifo_key=ftok(getenv("HOME"), FIFO_ID);
    fifo=msgget(fifo_key, 0);
    if(fifo<0) ERROR(1, "CLIENT: Couldn't open fifo");

    key_t sem_key=ftok(getenv("HOME"), SEMAPHORE_ID);
    semaphore=semget(sem_key, 0, 0);
    if(semaphore<0) ERROR(1, "CLIENT: Couldn't obtain semaphore");

    key_t shared_memory_key=ftok(getenv("HOME"), SHARED_MEMORY_ID);
    shared_memory=shmget(shared_memory_key, 0, 0);
    if(shared_memory<0) ERROR(1, "CLIENT: Couldn't open shared memory");

    barber=(barber_state*) shmat(shared_memory, NULL, 0);
    if(barber<0) ERROR (1, "CLIENT: Couldn't set shared memory");

    for(int i=0; i<clients_number; i++) {
        pid_t pid=fork();
        if(pid==0) { 
            visit_barber_store(shavings);
            exit(EXIT_SUCCESS);
        }
        else clients[i]=pid;
    }

    for(int i=0; i<clients_number; i++) {
        if(clients[i]>0) waitpid(clients[i], 0, 0);
    }

    return 0;
}

void visit_barber_store(int shavings) {
    while(shavings>0) {
        semaphore_dec(semaphore, CLIENT_COUNTER_SEM);
        semaphore_dec(semaphore, SEATS_LIMIT_SEM);
        if(barber->client_counter==barber->seats_limit) {
            printc("No free seats left, I'll come back later");
            semaphore_inc(semaphore, CLIENT_COUNTER_SEM);
            semaphore_inc(semaphore, SEATS_LIMIT_SEM);
            continue;
        }
        printc("Entering the barber store");
        semaphore_inc(semaphore, SEATS_LIMIT_SEM);
        if(barber->client_counter==0) {
            semaphore_dec(semaphore, ASLEEP_SEM);
            if(barber->asleep==TRUE) { 
                semaphore_inc(semaphore, WAKE_UP_SEM);
                printc("Waking barber up");
            }
            semaphore_inc(semaphore, ASLEEP_SEM);
        }
        client_msg msg;
        msg.mtype=CLIENT;
        msg.pid=getpid();
        semaphore_dec(semaphore, QUEUE_SEM);
        if(msgsnd(fifo, &msg, sizeof(client_msg), 0)<0) {
        char temp[100];
        sprintf(temp, "CLIENT ID%d: msgsnd() failure", getpid());
        ERROR(1, temp);
        }
        barber->client_counter++;
        printc("Taking seat in the waiting room");
        semaphore_inc(semaphore, QUEUE_SEM);
        semaphore_inc(semaphore, CLIENT_COUNTER_SEM);

        semaphore_dec(semaphore, SIT_SEM);
        semaphore_dec(semaphore, CLIENT_COUNTER_SEM);
        barber->client_counter--;
        semaphore_inc(semaphore, CLIENT_COUNTER_SEM);
        printc("Taking seat in the barber chair");
        semaphore_dec(semaphore, FINISH_SHAVING_SEM);
        printc("Exiting barber store after shaving");
        shavings--;
    }
}

void printc(const char* communique) {
    print_time_msec();
    printf(" Client ID%d: %s", getpid(), communique);
    printf("\n");
}

void semaphore_inc(int semaphore_id, int sem_num) {
    buffer.sem_num=sem_num;
    buffer.sem_op=1;
    buffer.sem_flg=0;
    if(semop(semaphore_id, &buffer, 1)<0) { 
        char temp[100];
        sprintf(temp, "CLIENT ID%d: Couldn't decrease semaphore", getpid());
        ERROR(1, temp);
    }
}
void semaphore_dec(int semaphore_id, int sem_num) {
    buffer.sem_num=sem_num;
    buffer.sem_op=-1;
    buffer.sem_flg=0;
    if(semop(semaphore_id, &buffer, 1)<0) {
        char temp[100];
        sprintf(temp, "CLIENT ID%d: Couldn't decrease semaphore", getpid());
        ERROR(1, temp);
    } 
}
