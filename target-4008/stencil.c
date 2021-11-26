#include "stencil.h"

// Ni,Nj,Nk -- Dimensions of the output matrix
// S -- width/length/height of the stencil
inline void compute_stencil(int Ni, int Nj, int Nk, int S, 
            const float In[Ni+S][Nj+S][Nk+S], float Out[Ni][Nj][Nk], 
            const float Stencil[S][S][S]) 
{
/*
if (S == 2) {
	int a = 0;
	for (int i = 0; i < Ni; ++i) {
		for (int x = 0; x < S; ++x) {

			for (int j = 0; j < Nj; ++j) {
				for (int y = 0; y < S; ++y) {
					
					int k = 0;
					Out[i+x][j+y][k] += In[i+x][j+y][k] * Stencil[x][y][a];

					for (k=0; k+1 < Nk; ++k) {
             					a = 1-a;
						Out[i][j][k+1] += In[i+x][j+y][k+1] * Stencil[x][y][a];
             					Out[i][j][k] += Out[i][j][k+1];

	
					}	
		
					if ( k < Nk) {
						a = 1-a;
						Out[i][j][k]+= In[i][j][k+1] * Stencil[0][0][a];
          
        				}
					a = 0;		
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
return;
}*/

if( S==3) {
for(int i = 0; i < Ni; ++i) {

        for(int x = 0; x < S; ++x) {
                for(int j = 0; j < Nj; ++j) {
                        for(int y = 0; y < S; ++y) {

                                for(int k = 0; k < Nk; ++k) {
                                        for(int z = 0; z <3; ++z) {       
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
return;
}
if( S==8) {
for(int i = 0; i < Ni; ++i) {

        for(int x = 0; x < S; ++x) {
                for(int j = 0; j < Nj; ++j) {
                        for(int y = 0; y < S; ++y) {
                        
                                for(int k = 0; k < Nk; ++k) {
                                        for(int z = 0; z <S; ++z) {
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
return;
}
if( S==20) {

for(int i = 0; i < Ni; ++i) {
for(int xx = 0; xx < S; xx+=4) {

        for(int x = xx; x <xx+4; ++x) {
                for(int j = 0; j < Nj; ++j) {
			for(int yy = 0; yy < S; yy+=4){
                        for(int y =yy; y <yy+4; ++y) {

                                for(int k = 0; k < Nk; ++k) {
				for(int zz = 0; zz < S; zz+=4) {
                                        for(int z = zz; z <zz+4; ++z) {
		
                                                Out[i][j][k] += In[i+x][j+y][k+z] * Stencil[x][y][z];
                                        }
                                }
                        }
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
return;
}

if( S==2) {
for(int i = 0; i < Ni; ++i) {

        for(int x = 0; x < S; ++x) {
                for(int j = 0; j < Nj; ++j) {
                        for(int y = 0; y < S; ++y) {

                                for(int k = 0; k < Nk; ++k) {
                                	for(int z = 0; z <2; ++z) {       
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
return;
}


 
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




