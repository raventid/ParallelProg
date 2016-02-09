#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int main(){
 char buf[100];
 mkfifo("in.fifo", 0666);
 mkfifo("out.fifo", 0777);

 int pipe_in = open("in.fifo", O_RDONLY | O_NONBLOCK);

 int len=0;
 while(1){
   len = read(pipe_in, &buf, sizeof(buf));
   if (len == 0) 
   {
       continue;
   } else {
       int pipe_out = open("out.fifo", O_WRONLY);
       write(pipe_out, &buf, len);
       close(pipe_out);
       exit(0);
   }
 }

 close(pipe_in);
 return 0;
}
