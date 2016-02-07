#include <stdio.h>
#include <fcntl.h>

int main(int argc, char** argv){
 char *line = "hello, world!";
 FILE* fp = fopen("/tmp/myFIFO","rw");
 fprintf(fp, line);
 fclose(fp);
 return 0;
}
