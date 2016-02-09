#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

union semun {
 int val;
 struct semid_ds *buf;
 unsigned short *array;
};

int main(){
//sem_t s;
//int sem_init(sem_t *sem, int pshared, usigned value);
//int sem_destroy(sem_t *sem);
sem_t *sem = sem_open("/test.sem");
key_t key = ftok("tmp/sem.temp", 1);

int semid = semget(key, 16, 0666 | IPC_CREAT);
int i;

union semun arg;

struct sembuf sem_opt[16];
for(i = 0; i < 16; i++) {
  arg.val = i;
  semctl(semid, i, SETVAL, arg);
}

pause();

return 0;
}
