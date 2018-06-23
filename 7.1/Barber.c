#include "Barber_Store.h"
#include "jjp_lib.h"

int shared_memory;
int semaphore;
int fifo=-1;
static struct sembuf buffer;
barber_state* barber;

void printc(const char* communique);
void barber_store();
void exit_barber();
void semaphore_inc(int semaphore_id, int sem_num);
void semaphore_dec(int semaphore_id, int sem_num);
void sighandler(int signo) {exit(0);}


int main(int argc, char** argv) {
    if(argc!=2) ERROR(1, "BARBER: Pass number of seats in waiting room.\n");
    int seats=atoi(argv[1]);
    if(seats<0) ERROR(1, "Wrong seats number passed");
    if(atexit(exit_barber)<0) ERROR(1, "BARBER: atexit() error");

    signal(SIGTERM, sighandler);
    signal(SIGINT, sighandler);

    key_t fifo_key=ftok(getenv("HOME"), FIFO_ID);
    fifo=msgget(fifo_key, IPC_CREAT | IPC_EXCL | S_IRWXU | S_IRWXG | S_IRWXO);
    if(fifo<0) ERROR(1, "BARBER: Couldn't create fifo");
    
    key_t semaphore_key=ftok(getenv("HOME"), SEMAPHORE_ID);
    semaphore=semget(semaphore_key, 9, IPC_CREAT | IPC_EXCL | S_IRWXU | S_IRWXG | S_IRWXO);
    if(semaphore<0) ERROR(1, "BARBER: Couldn't create semaphore");

    if(semctl(semaphore, QUEUE_SEM, SETVAL, 1)<0) ERROR(1, "BARBER: Couldn't create QUEUE semaphor");
    if(semctl(semaphore, ASLEEP_SEM, SETVAL, 1)<0) ERROR(1, "BARBER: Couldn't create ASLEEP semaphor");
    if(semctl(semaphore, SEATS_LIMIT_SEM, SETVAL, 1)<0) ERROR(1, "BARBER: Couldn't create SEATS LIMIT semaphor");
    if(semctl(semaphore, CURRENT_CLIENT_SEM, SETVAL, 1)<0) ERROR(1, "BARBER: Couldn't create CURRENT CLIENT semaphor");
    if(semctl(semaphore, CLIENT_COUNTER_SEM, SETVAL, 1)<0) ERROR(1, "BARBER: Couldn't create CLIENT COUNTER semaphore");
    if(semctl(semaphore, SHAVING_SEM, SETVAL, 1)<0) ERROR(1, "BARBER: Couldn't create SIT semaphore");
    if(semctl(semaphore, WAKE_UP_SEM, SETVAL, 0)<0) ERROR(1, "BARBER: Couldn't create WAKE UP semaphore");
    if(semctl(semaphore, FINISH_SHAVING_SEM, SETVAL, 0)<0) ERROR(1, "BARBER: Couldn't create FINISHING SHAVING semaphore");
    if(semctl(semaphore, SIT_SEM, SETVAL, 0)<0) ERROR(1, "BARBER: Couldn't create SIT semaphore");

    key_t shared_memory_key=ftok(getenv("HOME"), SHARED_MEMORY_ID); //printf("\nShared mem key: %d", shared_memory_key);
    shared_memory=shmget(shared_memory_key, sizeof(barber_state),  IPC_CREAT | IPC_EXCL | S_IRWXU | S_IRWXG | S_IRWXO);
    if(shared_memory<0) ERROR(1, "BARBER: Couldn't create shared memory");
    
    barber=(barber_state*) shmat(shared_memory, NULL, 0);
    if(barber==(void*)-1) ERROR(1, "BARBER: Couldn't add shared memory");
    
    barber->asleep=0;
    barber->current_client=-1;
    barber->seats_limit=seats;
    barber->client_counter=0;

    printc("Opening barber store");
    while(1){barber_store(seats);}

    return 0;  
}

void barber_store (int seats) {
    semaphore_dec(semaphore, CURRENT_CLIENT_SEM);
    if(barber->current_client!=-1) {
        char temp[100];
        semaphore_dec(semaphore, SHAVING_SEM);
        sprintf(temp, "Shaving client no. ID%d", barber->current_client);
        printc(temp);
        semaphore_inc(semaphore, SHAVING_SEM);
        sprintf(temp, "Finishing shaving of client no. ID%d", barber->current_client);
        printc(temp);
        barber->current_client=-1;
        semaphore_inc(semaphore, FINISH_SHAVING_SEM);

    }
    semaphore_inc(semaphore, CURRENT_CLIENT_SEM);

    semaphore_dec(semaphore, ASLEEP_SEM);
    semaphore_dec(semaphore, CLIENT_COUNTER_SEM);
    if(barber->client_counter==0 ) {
        if(barber->asleep==FALSE) {
            barber->asleep=TRUE;
            printc("Falling asleep");
        }
        semaphore_inc(semaphore, ASLEEP_SEM);
        semaphore_inc(semaphore, CLIENT_COUNTER_SEM);

        //blocking this process, until client wakes barber up (uses semaphore)
        semaphore_dec(semaphore, WAKE_UP_SEM);
        
        semaphore_dec(semaphore, ASLEEP_SEM);
        barber->asleep=FALSE;
        printc("Waking up");
        semaphore_inc(semaphore, ASLEEP_SEM);
        
    }
    else {
        semaphore_inc(semaphore, ASLEEP_SEM);
        semaphore_inc(semaphore, CLIENT_COUNTER_SEM);
        client_msg  msg_buffer;
        if(msgrcv(fifo, &msg_buffer, sizeof(msg_buffer), 0, 0)<0) ERROR(1, "BARBER: Couldn't receive message");
        
        semaphore_dec(semaphore, QUEUE_SEM);
        semaphore_dec(semaphore, CURRENT_CLIENT_SEM);
        barber->current_client=msg_buffer.pid;
        char temp[100];
        sprintf(temp, "Inviting client no. ID%d", barber->current_client);
        printc(temp);
        semaphore_inc(semaphore, SIT_SEM);
        semaphore_inc(semaphore, CURRENT_CLIENT_SEM);
        semaphore_inc(semaphore, QUEUE_SEM);

        semaphore_dec(semaphore, CLIENT_COUNTER_SEM);
        barber->client_counter--;
        semaphore_inc(semaphore, CLIENT_COUNTER_SEM);
    }
}

void printc(const char* communique) {
    print_time_msec();
    printf(" BARBER: %s", communique);
    printf("\n");
}

void exit_barber() {
    if(semctl(semaphore, 0, IPC_RMID, NULL)<0) perror("BARBER: semctl() semaphore error");
    if(shmdt(barber)<0) perror("BARBER: Shared memory unlinking error");
    if(shmctl(shared_memory, IPC_RMID, NULL)<0) perror("BARBER: shmctl() memory error");
    if(msgctl(fifo, IPC_RMID, NULL)<0) perror("BARBER: msgctl() fifo error");
    printf("\n"); 
    exit(0);
}

void semaphore_inc(int semaphore_id, int sem_num) {
    buffer.sem_num=sem_num;
    buffer.sem_op=1;
    buffer.sem_flg=0;
    if(semop(semaphore_id, &buffer, 1)<0) ERROR(1, "BARBER: Couldn't increase semaphore");
}
void semaphore_dec(int semaphore_id, int sem_num) {
    buffer.sem_num=sem_num;
    buffer.sem_op=-1;
    buffer.sem_flg=0;
    if(semop(semaphore_id, &buffer, 1)<0) ERROR(1, "BARBER: Couldn't decrease semaphore");
}
