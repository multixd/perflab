#include "stencil.h"

// Ni,Nj,Nk -- Dimensions of the output matrix
// S -- width/length/height of the stencil
inline void compute_stencil(int Ni, int Nj, int Nk, int S, 
            const float In[Ni+S][Nj+S][Nk+S], float Out[Ni][Nj][Nk], 
            const float Stencil[S][S][S]) 
{

/*if (S = 2) {
	
	for (int k = 0; k < Nk; ++k) {
		for (int z = 0; z < S; ++z) {
			 macc_element(&In[0][0][k+z],&Out[0][0][k],&Stencil[0][0][z]);
		} 
	}
}
*/



 
for(int i = 0; i < Ni; ++i) { 

	for(int x = 0; x < S; ++x) {

		for(int j = 0; j < Nj; ++j) {
			for(int y = 0; y < S; ++y) {
				for(int k = 0; k < Nk; ++k) {
        				for(int z = 0; z < S; ++z) {
						 Out[i][j][k] += In[i+x][j+y][k+z] * Stencil[x][y][z];
					}
				}	
			}				
	
		
		}
	}
}
double total = S*S*S;
for (int i = 0; i < Ni; ++i) {
	for (int j = 0; j < Nj; ++j) {
		for (int k = 0; k < Nk; ++k) {
			Out[i][j][k] /= total;
		}
	}
}
}




