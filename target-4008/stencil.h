#ifndef STENCIL_H
#define STENCIL_H

#include <stdint.h>
#include <stdio.h>

//Define all the test cases
#define T1I 192
#define T1J 192
#define T1K 192
#define T1S   3

#define T2I       4
#define T2J       4
#define T2K 1048576
#define T2S       2

#define T3I  128
#define T3J  128
#define T3K  128
#define T3S    8

#define T4I   64
#define T4J   64
#define T4K   64
#define T4S   20


void normalize(float* Out, float S);

void macc_element(const float* In, float* Out, const float* stencil);

void compute_stencil(int Ni, int Nj, int Nk, int S, 
            const float In[Ni+S][Nj+S][Nk+S], float Out[Ni][Nj][Nk], 
            const float Stencil[S][S][S]);

#endif
