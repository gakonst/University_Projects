#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "float.h"
#include "pthread.h"
#define DIM 3

//#define Threads 8

struct vars
{
    int N;
    float *X;
    float *low;
    float step;
    int *codes;
    int start;
    int finish;
 //  int offset;
};


inline unsigned int compute_code(float x, float low, float step){

  return floor((x - low) / step);

}


/* Function that does the quantization */
void *quantize(void *arg){

   struct vars *qu = (struct vars*) arg;
   int N = qu->N;
   float *X = qu->X;
   float *low = qu->low;
   float step= qu-> step;
   int *codes = qu->codes;
   int start = qu->start;
   int finish = qu->finish;
//   int offset = qu->offset;

  for(int i=start; i<finish; i++){
    for(int j=0; j<DIM; j++){
      codes[i*DIM + j] = compute_code(X[i*DIM + j], low[j], step); 
    }
  }
  pthread_exit(NULL);

}

void quantize_struct(unsigned int *codes, float *X, float *low, float step, int N, int Threads){
    struct vars arr[Threads];
    pthread_t threads[Threads];
    int start=0;
    int finish=N/Threads;
    for (int i=0;i<Threads;i++){
        arr[i].N=N;
        arr[i].step=step;
        arr[i].low=low;
        arr[i].X=X;
        arr[i].codes=codes;
        arr[i].start=start;
        arr[i].finish=finish;
//        arr[i].offset=i;

      pthread_create(&threads[i],NULL,quantize,(void *)&arr[i]);  
      start+=N/Threads;
      finish+=N/Threads;
    }
    for (int i=0;i<Threads-1;i++){
        pthread_join(threads[i],NULL);
    }

}

float max_range(float *x){

  float max = -FLT_MAX;
  for(int i=0; i<DIM; i++){
    if(max<x[i]){
      max = x[i];
    }
  }

  return max;

}

void compute_hash_codes(unsigned int *codes, float *X, int N, 
			int nbins, float *min, 
			float *max, int Threads){
  
  float range[DIM];
  float qstep;

  for(int i=0; i<DIM; i++){
    range[i] = fabs(max[i] - min[i]); // The range of the data
    range[i] += 0.01*range[i]; // Add somthing small to avoid having points exactly at the boundaries 
  }

  qstep = max_range(range) / nbins; // The quantization step 
  
  quantize_struct(codes, X, min, qstep, N, Threads); // Function that does the quantization

}



