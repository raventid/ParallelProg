#include <iostream>
#include <stdio.h>
#include <stdint.h>

int main() 
{

  uint32_t *i = 1024;
  i++;

  printf("i=%d", i);
  
  return 0;
}
