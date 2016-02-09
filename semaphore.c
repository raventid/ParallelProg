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
key_t key = ftok("/tmp/sem.temp", 1);                                            
                                                                                 
int semid = semget(key, 16, 0666 | IPC_CREAT);                                   
int i;                                                                           
                                                                                 
union semun arg;                                                                 
                                                                                 
struct sembuf sem_opt[16];                                                       
                                                                                 
for (i = 0; i < 16; i++) {                                                       
    sem_opt[i].sem_num = i;                                                      
    sem_opt[i].sem_op = i;                                                       
    sem_opt[i].sem_flg = 0;                                                      
}                                                                                
semop(semid, sem_opt, 16);                                                       
//for(i = 0; i < 16; i++) {                                                      
//  arg.val = i;                                                                 
//  semctl(semid, i, SETVAL, arg);                                               
//}                                                                              
                                                                                 
pause();                                                                         
                                                                                 
return 0;                                                                        
}                                                                                
