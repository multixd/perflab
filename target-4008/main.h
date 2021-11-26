#ifndef MAIN_H
#define MAIN_H

#include <sys/time.h>

void stencil_check(int Ni, int Nj, int Nk, int S, 
            const float In[Ni+S][Nj+S][Nk+S], float Out[Ni][Nj][Nk], 
            const float Stencil[S][S][S]);

static __inline__ uint64_t read_usec(void) {
  struct timeval tv;
  gettimeofday(&tv, 0);
  return (((uint64_t)tv.tv_sec) * 1000000 + ((uint64_t)tv.tv_usec));
}


#endif  
