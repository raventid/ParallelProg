#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>

int main(int argc, char* argv)
{
  void *p1 = malloc(sizeof(int));
  printf("Value of p1 before reallocation is %p\n", p1);
  printf("Size of allocated block of memory is %zu\n", malloc_usable_size(p1));

  size_t mult = 2;
  while(1){
    //realloc(p1, sizeof(int)*mult);
    free(p1); 
    p1 = malloc(sizeof(int)*mult);

    printf("---------------------------------------------\n\n");
    printf("Value of p1 after reallocation is %p\n", p1);
    printf("Size for reallocation is %lu\n", sizeof(int)*mult);
    printf("Size of allocated block of memory is %zu\n", malloc_usable_size(p1));
    printf("---------------------------------------------\n");

    mult = mult + 1000000;
    sleep(5);
  }

  free(p1);
  return 0;
}
