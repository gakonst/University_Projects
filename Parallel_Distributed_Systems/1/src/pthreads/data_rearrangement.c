#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <pthread.h>

#define DIM 3
//#define Threads 4


typedef struct {
    int *permutation_vector;
    float *X;
    float *Y;
    int start;
    int finish;
}rearrange;



inline void fun_copy(int *permutation_vector,float *X,float *Y,int i){
       memcpy(&Y[i*DIM], &X[permutation_vector[i]*DIM], DIM*sizeof(float));
        }

void *data(void *arg){
    
   rearrange *vars = (rearrange*) arg;
   float *X = vars->X;
   float *Y = vars->Y;
   int start  = vars->start;
   int finish = vars->finish;
  int *permutation_vector=vars->permutation_vector;

   for (int i=start; i<finish;i++){
       fun_copy(permutation_vector,X,Y,i);
   }
}





void data_rearrangement(float *Y, float *X, 
			unsigned int *permutation_vector, 
			int N, int Threads){
  rearrange array[Threads];
   pthread_t threads[Threads];
   int start = 0;
   int finish = N/Threads;
  for(int i=0; i<Threads; i++){
      array[i].X=X;
      array[i].Y=Y;
      array[i].start = start;
      array[i].finish = finish;
      array[i].permutation_vector=permutation_vector;
      pthread_create(&threads[i], NULL, data, (void *)&array[i]);
    start+=N/Threads;
    finish+=N/Threads;
  }
  for (int i=0;i<Threads;i++){
      pthread_join(threads[i],NULL);
  }
}


