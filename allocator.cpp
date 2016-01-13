#include <stdio.h>
#include <iostream>
#include <map>
#include <iterator>
#include <stdlib.h>
#include <string.h>

class SmallAllocator {
private:
        char Memory[1048576];
	std::map <int, int> memory_manager;

	void *create_memory_block(unsigned int Size) {
	  int position = get_available_gap(Size);
	  memory_manager[position] = position + Size - 1;
	  for( std::map<int, int>::iterator ii=memory_manager.begin(); ii!=memory_manager.end(); ++ii)
          {
		  std::cout << (*ii).first << ": " << (*ii).second << std::endl;
          } 
	  return (void *)&Memory[position];
	}

	void free_memory_block(void *Pointer) {
		std::cout << (char *)Pointer - Memory << std::endl;
          memory_manager.erase( (char *)Pointer - Memory );	
	}

	int get_available_gap(unsigned int Size) {
	  if(0 == memory_manager.size()){
	         return 0;	
	  }
	  if(1 == memory_manager.size()){
		 return memory_manager.begin()->second+1; 
	  }
          std::map<int, int>::iterator prev = memory_manager.begin(); 
  	  for( std::map<int, int>::iterator ii=memory_manager.begin(); ii!=memory_manager.end(); ++ii)
            { 
		    std::cout << (*ii).first << std::endl;
		    std::cout << (*prev).second << std::endl;

		  if(memory_manager.begin() != ii) {
		    if ( (*ii).first - (*prev).second >= Size ) {
		     return (*prev).second + 1;  
		    }
		    prev = ii;
		  }
            }
	  std::cout << "Yes, I will return the last one which is " << memory_manager.rbegin()->second+1  << std::endl;
	  return memory_manager.rbegin()->second+1;
	}

        int reallocate(void *Pointer, unsigned int Size) {
		    int position = (char *)Pointer - Memory;
		    std::map<int, int>::iterator next = memory_manager.upper_bound(position);  
                    if(memory_manager.end() != next) {
		      if ( (*next).first - position >= Size ) {
		       memory_manager[position] = memory_manager[position] + Size - 1;
 		       return position;  
		      }
		      else {
		        return -1;
		      }
		    }
		    else{
	             memory_manager[position] = memory_manager[position] + Size - 1;
		     return position;
                    }
	}

public:
        void *Alloc(unsigned int Size) {
                return create_memory_block(Size); 
         //       return malloc(Size);
	};
        void *ReAlloc(void *Pointer, unsigned int Size) {
		int pointer = 0;
		if (-1 != reallocate(Pointer, Size)){
                  return Pointer;
		}
		else
		{
	          void *new_block = create_memory_block(Size);
                  memmove( new_block, Pointer, Size);
	          free_memory_block(Pointer);
		  return new_block; 
		}
        };
        void Free(void *Pointer) {
                free_memory_block(Pointer);
        };
};

int main(int argc, char **argv)
{
SmallAllocator A1;
std::cout << "first stetps" << std::endl;
int * A1_P1 = (int *) A1.Alloc(sizeof(int));
int * b = (int *) A1.Alloc(5*sizeof(int));
std::cout << "------------" << std::endl;
int * c = (int *) A1.Alloc(5*sizeof(int));
std::cout << "------------" << std::endl;
int * m = (int *) A1.Alloc(5*sizeof(int));
std::cout << "------------" << std::endl;

A1_P1 = (int *) A1.ReAlloc(A1_P1, 2 * sizeof(int));
m = (int *) A1.ReAlloc(m, 6 * sizeof(int));
A1.Free(A1_P1);
A1.Free(c);

int * x = (int *) A1.Alloc(5*sizeof(int));

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
