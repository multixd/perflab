#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "stencil.h"
#include "main.h"

char hostname[HOST_NAME_MAX];

void normalize(float* Out, float S) {
  double total_elements = S * S * S;
  *Out /= total_elements;
}

void macc_element(const float* In, float* Out, const float* Stencil) {
  *Out += (*In) * (*Stencil);
}


static struct option long_options[] = {
//    {"direct-path",        no_argument,       nullptr, 'd',},
    {"input",         required_argument, 0, 'i'},
    {"trials",        required_argument, 0, 't'},
    {0, 0, 0, 0,},
};


typedef struct {
  int Ni,Nj,Nk;
  int S;
  float orig_msec;
  float best_msec; 
} TestParams;

void clear3d(int Ni, int Nj, int Nk, float a[Ni][Nj][Nk]) {
  for(int i = 0; i < Ni; ++i) {
    for(int j = 0; j < Nj; ++j) {
      for(int k = 0; k < Nk; ++k) {
        a[i][j][k] = 0;
      }
    }
  }
}

void gen_3d(int Ni, int Nj, int Nk, float a[Ni][Nj][Nk]) {
  for(int i = 0; i < Ni; ++i) {
    for(int j = 0; j < Nj; ++j) {
      for(int k = 0; k < Nk; ++k) {
        a[i][j][k] = ((float)rand())/(RAND_MAX/8);
      }
    }
  }
}

static int max_errors_to_print = 5;

char check_3d(int Ni, int Nj, int Nk,
              float a[Ni][Nj][Nk], float a_check[Ni][Nj][Nk]) {
  int errors_printed=0;
  char has_errors = 0;
  for(int i = 0; i < Ni; ++i) {
    for(int j = 0; j < Nj; ++j) {
      for(int k = 0; k < Nk; ++k) {
        if( (a[i][j][k] < (a_check[i][j][k] - 0.005)) ||
            (a[i][j][k] > (a_check[i][j][k] + 0.005))    ) {
          has_errors = 1;
          if(errors_printed < max_errors_to_print) {
            if(errors_printed==0) printf("\n");
            printf("Error on index: [%d][%d][%d].",i,j,k);
            printf("Your output: %f, Correct output %f\n",a[i][j][k], a_check[i][j][k]);
            errors_printed++;
          } else {
            //printed too many errors already, just stop
            if(max_errors_to_print !=0) {
              printf("and many more errors likely exist...\n");
            }
            return 1;
          }
        }
      }
    }
  }
  return has_errors;
}

float run(TestParams* p, char check_func, char* is_broken) {
  uint64_t start_time, total_time;

  int Ni = p->Ni, Nj = p->Nj, Nk=p->Nk;
  int S = p->S;
  void* in   = malloc(sizeof(float) * (Ni+S) * (Nj+S) * (Nk+S));
  void* out  = malloc(sizeof(float) * Ni * Nj * Nk);
  void* kern = malloc(sizeof(float) * S * S * S);
  void* out_check  = 0;

  void (*func)(int Ni, int Nj, int Nk, int S, 
            const float In[Ni][Nj][Nk], float Out[Ni][Nj][Nk], 
            const float Stencil[S][S][S]) = compute_stencil;

  // Generate the inputs 
  gen_3d((Ni+S),(Nj+S),(Nk+S),in);
  gen_3d(S,S,S,kern);
  clear3d(Ni,Nj,Nk,out);

  if(check_func) {
    out_check  = calloc(sizeof(float), Ni * Nj * Nk);
    //clear3d(Ni,Nj,Nk,out_check);
    stencil_check(Ni,Nj,Nk,S,in,out_check,kern);
  }

  start_time = read_usec();
  (*func)(Ni,Nj,Nk,S,in,out,kern);
  total_time = read_usec() - start_time;

  if(check_func) {
    if(check_3d(Ni,Nj,Nk,out,out_check)) {
      *is_broken=1;
    } else {
      printf("no problems detected\n");
    }
  }

  double total_msec = total_time / 1000.0;
  float speedup = p->orig_msec / total_msec ;
  p->best_msec = fmin(p->best_msec,total_msec);

  double computations = ((double)Ni)*Nj*Nk*S*S*S;
  float ghz=2.0;
  double clocks = total_time*1000.0 * ghz;
  printf("  Runtime (msec): %0.1f, CPE: %0.3f, Speedup: %0.3f\n", 
      total_msec, clocks/computations, speedup);

  free(in);
  free(out);
  free(kern);
  if(out_check) free(out_check);

  return total_time;
}


#define NUM_TESTS (4)
TestParams Tests[NUM_TESTS] = 
 { 
   {T1I,T1J,T1K,T1S,    782,   10000000.00},
   {T2I,T2J,T2K,T2S,    816,   10000000.00},
   {T3I,T3J,T3K,T3S,   3950,   10000000.00},
   {T4I,T4J,T4K,T4S,   7800,   10000000.00} };

char run_test(int i, char check_func) {
  if(i<0 || i >= NUM_TESTS) {
    printf("Bad Test Case: %d, exiting\n",i+1);
    exit(0);
  }
  char is_broken=0;
  printf("Test %d with Ni,Nj,Nk=%d,%d,%d; S=%d -- ",i+1,
          Tests[i].Ni,Tests[i].Nj,Tests[i].Nk,Tests[i].S);
  fflush(stdout);
  run(&(Tests[i]),check_func,&is_broken);
  return is_broken;
}

float interp(float s, float l, float lgrade, 
                      float h, float hgrade) {
  return (s - l) * (hgrade - lgrade) / (h -l) + lgrade;
}

float grade(float s) {
  if(s<1) return 0;
  if(s<2.5) return interp(s,   1,  0, 2.5,  40);
  if(s<7)   return interp(s, 2.5, 40,   7,  70);
  if(s<13)  return interp(s,   7, 70,  13,  90);
  if(s<20)  return interp(s,  13, 90,  20, 100);
  return 100;
}

int main(int argc, char** argv) {
  char do_all=0;
  char check_func=1;
  int test_case=1;

  srand (time(NULL));

  int opt;
  int num_trials=1;

  int result = gethostname(hostname, HOST_NAME_MAX);
  if (result) {
    perror("gethostname");
    return EXIT_FAILURE;
  }
 
  char on_right_machine=1; 
  if(strcmp(hostname,"lnxsrv07.seas.ucla.edu") && 
    strcmp(hostname,"lnxsrv09.seas.ucla.edu")) {
    printf("WARNING! You are not on lnxsrv07 or lnxsrv09, so results may not be valid!\n");
    printf("Your machine is %s.\n",hostname);
    on_right_machine=0;
  }

  while ((opt = getopt_long(argc, argv, "i:t:", long_options, 0)) != -1) {
    switch (opt) {
      case 'i':
        if(*optarg == 'a')
          do_all=1;
        else {
          test_case=atoi(optarg);
        }
        break;
      case 't': 
        num_trials=atoi(optarg);
    }
  }

  printf("Num Trials: %d (change with --trials)\n", num_trials);

  int benchmarks_failed = 0;
  
  if(do_all) {
    printf("Running all %d test cases!\n",NUM_TESTS);

    for(int t = 0; t < num_trials; ++t) {
      if(t!=0) printf("\n");
      for(int i = 0; i < NUM_TESTS; i++) {
        benchmarks_failed+=run_test(i,check_func);
      }
    }

    float gmean_speedup=1;
    for(int i = 0; i < NUM_TESTS; i++) {
      double speedup = ((double)Tests[i].orig_msec / (double)Tests[i].best_msec);
      gmean_speedup *= speedup;
    }
    gmean_speedup = pow(gmean_speedup,1.0/NUM_TESTS);

    printf("Geomean Speedup: %0.2f\n", gmean_speedup);

    if(benchmarks_failed) {
      printf("Number of Benchmarks FAILED: %d\n",benchmarks_failed);
      printf("No grade given, because of incorrect execution.\n");
    } else {
      if(on_right_machine) { 
        printf("Grade: %0.1f\n",grade(gmean_speedup));
      } else {
        printf("No grade given, because you're not on the right machine.\n");
      }
    }
  } else {
    for(int t = 0; t < num_trials; ++t) {
      run_test(test_case-1,check_func);
    }
    double speedup = ((double)Tests[test_case-1].orig_msec / (double)Tests[test_case-1].best_msec);
    printf("Best Speedup: %0.2f\n\n", speedup);

    printf("No grade given, because only one test is run.\n");
    printf(" ... To see your grade, run all tests with \"-i a\"\n");

  }
}


