#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<sys/wait.h>

void save_pid(char * str){
  FILE *f = fopen(str, "w");
  if (f == NULL)
  {
      printf("Error opening file!\n");
      exit(1);
  }
  
  fprintf(f, "%d", getpid());
  fclose(f);
}

int main(void)
{
    pid_t pid;
    if(!(pid=fork())){
     save_pid("pid_child");
     puts("Hello from child process");
     while(1){
      sleep(4);
      puts("Hello from child process"); 
     } 
    }else{
    save_pid("pid_parent");
    waitpid(pid, NULL, 0);
    pause();
    }
    return 0;
}
