#include <stdio.h>
#include <iostream>
#include <stdlib.h>

typedef unsigned int MEMTYPE;

MEMTYPE *mem;
MEMTYPE memSize;
MEMTYPE avail;  //1st index of the 1st free range

//return size of block
MEMTYPE blockSize(MEMTYPE x) {
  return mem[x];
}

//return next free block
MEMTYPE next(MEMTYPE x) {
  return mem[x + mem[x]];
}

//return index of pointer to next free block
MEMTYPE linkToNext(MEMTYPE x) {
  return x + mem[x];
}

//initialize memory
void my_init(void *ptr, unsigned size) {

  mem = (MEMTYPE *) ptr;
  memSize = size / sizeof(MEMTYPE);
  mem[0] = memSize - 1;
  mem[memSize - 1] = memSize;
  avail = 0;
}

//allocate memory
void *my_alloc(unsigned size) {

  if (size == 0) {  //return NULL pointer after attempt to allocate 0-length memory
    return NULL;
  }

  MEMTYPE num = size / sizeof(MEMTYPE);
  if (size % sizeof(MEMTYPE) > 0) num++;
  MEMTYPE cur, prev;  //pointer to (actually index of) current block, previous block
  MEMTYPE isFirstFreeBeingAllocated = 1;  //whether the first free block is being allocated

  prev = cur = avail;

  //testing, whether we have enough free space for allocation
  test:

  if (avail == memSize) {  //if we are on the end of the memory
    return NULL;
  }

  if (blockSize(cur) < num) {  //if the size of free block is lower than requested
    isFirstFreeBeingAllocated = 0;
    prev = cur;

    if (next(cur) == memSize) {  //if not enough memory
      return NULL;
    }
    else
      cur = next(cur);
    goto test;
  }

  if (blockSize(cur) == num) {  //if the size of free block is equal to requested

    if (isFirstFreeBeingAllocated)
      avail = next(cur);
    else
      mem[linkToNext(prev)] = next(cur);
  }

  else {  //if the size of free block is greater than requested

    if (isFirstFreeBeingAllocated) {
      if ((blockSize(cur) - num) == 1)  //if there is only 1 free item left from this (previously) free block
        avail = next(cur);
      else
        avail = cur + num + 1;
    }
    else {
      if ((blockSize(cur) - num) == 1)  //if there is only 1 free item left from this (previously) free block
        mem[linkToNext(prev)] = next(cur);
      else
        mem[linkToNext(prev)] = cur + num + 1;
    }

    if ((blockSize(cur) - num) == 1)  //if there is only 1 free item left from this (previously) free block
      mem[cur] = num + 1;
    else {
      mem[cur + num + 1] = blockSize(cur) - num - 1;
      mem[cur] = num;
    }
  }

  return (void *) &(mem[cur+1]);
}

//free memory
void my_free(void *ptr) {

  MEMTYPE toFree;  //pointer to block (to free)
  MEMTYPE cur, prev;

  toFree = ((MEMTYPE *)ptr - (mem + 1));


  if (toFree < avail) {  //if block, that is being freed is before the first free block

    if (((linkToNext(toFree) + 1) == avail) && avail < memSize)  //if next free block is immediately after block that is being freed
      mem[toFree] += (mem[avail] + 1);  //defragmentation of free space
    else
      mem[linkToNext(toFree)] = avail;

    avail = toFree;
  }

  else {  //if block, that is being freed isn't before the first free block

    prev = cur = avail;

    while (cur < toFree) {
      prev = cur;
      cur = next(cur);
    }

    if ((linkToNext(prev) + 1) == toFree) { //if previous free block is immediately before block that is being freed

      mem[prev] += (mem[toFree] + 1);  //defragmentation of free space

      if (((linkToNext(toFree) + 1) == cur) && cur < memSize)  //if next free block is immediately after block that is being freed
        mem[prev] += (mem[cur] + 1);  //defragmentation of free space
      else
        mem[linkToNext(toFree)] = cur;
    }
    else {
      mem[linkToNext(prev)] = toFree;
      mem[linkToNext(toFree)] = cur;
    }

  }
}
class SmallAllocator {
private:
        char Memory[1048576];
public:
        void *Alloc(unsigned int Size) {
                return my_alloc(Size);
        };
        void *ReAlloc(void *Pointer, unsigned int Size) {
                //return realloc(Pointer, Size);
                my_free(Pointer);
                return my_alloc(Size);
        };
        void Free(void *Pointer) {
                my_free((void *)Pointer);
        };
};
//class SmallAllocator {
//private:
//        char Memory[1048576];
//public:
//        void *Alloc(unsigned int Size) {};
//        void *ReAlloc(void *Pointer, unsigned int Size) {};
//        void Free(void *Pointer) {};
//};
int main(int argc, char **argv){

SmallAllocator A1;
std::cout << "first stetps" << std::endl;
int * A1_P1 = (int *) A1.Alloc(sizeof(int));
std::cout << "first allocation" << std::endl;
//A1_P1 = (int *) A1.ReAlloc(A1_P1, 2 * sizeof(int));
A1.Free(A1_P1);
std::cout << "first free" << std::endl;
SmallAllocator A2;
int * A2_P1 = (int *) A2.Alloc(10 * sizeof(int));
for(unsigned int i = 0; i < 10; i++) A2_P1[i] = i;
for(unsigned int i = 0; i < 10; i++) if(A2_P1[i] != i) std::cout << "ERROR 1" << std::endl;
int * A2_P2 = (int *) A2.Alloc(10 * sizeof(int));
for(unsigned int i = 0; i < 10; i++) A2_P2[i] = -1;
for(unsigned int i = 0; i < 10; i++) if(A2_P1[i] != i) std::cout << "ERROR 2" << std::endl;
for(unsigned int i = 0; i < 10; i++) if(A2_P2[i] != -1) std::cout << "ERROR 3" << std::endl;
A2_P1 = (int *) A2.ReAlloc(A2_P1, 20 * sizeof(int));
for(unsigned int i = 10; i < 20; i++) A2_P1[i] = i;
for(unsigned int i = 0; i < 20; i++) if(A2_P1[i] != i) std::cout << "ERROR 4" << std::endl;
for(unsigned int i = 0; i < 10; i++) if(A2_P2[i] != -1) std::cout << "ERROR 5" << std::endl;
A2_P1 = (int *) A2.ReAlloc(A2_P1, 5 * sizeof(int));
for(unsigned int i = 0; i < 5; i++) if(A2_P1[i] != i) std::cout << "ERROR 6" << std::endl;
for(unsigned int i = 0; i < 10; i++) if(A2_P2[i] != -1) std::cout << "ERROR 7" << std::endl;
A2.Free(A2_P1);
A2.Free(A2_P2);
return 0;

}
