#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "float.h"
#include "omp.h"
#define DIM 3

inline unsigned int compute_code(float x, float low, float step){

  return floor((x - low) / step);

}


/* Function that does the quantization */
void quantize(unsigned int *codes, float *X, float *low, float step, int N, int T){
int i,j;
#pragma omp parallel for num_threads(T) private(i,j)
 for(i=0; i<N; i++){
    for(j=0; j<DIM; j++){
      codes[i*DIM + j] = compute_code(X[i*DIM + j], low[j], step); 
    }
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
float compute_range(float max, float min, float *range, int i){
    float x;
    x = fabs(max-min);
    x +=0.01*x;
    range[i] = x;
    return x;
}
void compute_hash_codes(unsigned int *codes, float *X, int N, 
			int nbins, float *min, 
			float *max, int T){
  
  float range[DIM];
  float qstep;

 //Not always worth to be parallelized
 //#pragma omp parallel for num_threads(T)
 for(int i=0; i<DIM; i++){
     compute_range(max[i],min[i],range,i);
    // range[i] = fabs(max[i] - min[i]); // The range of the data
    // range[i] += 0.01*range[i]; // Add somthing small to avoid having points exactly at the boundaries 
  }

  qstep = max_range(range) / nbins; // The quantization step 
  
  quantize(codes, X, min, qstep, N, T); // Function that does the quantization

}

