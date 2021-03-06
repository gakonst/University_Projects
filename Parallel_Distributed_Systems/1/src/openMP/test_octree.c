#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sys/time.h"
#include "utils.h"
#include "math.h"
#include "omp.h"
#define DIM 3

int main(int argc, char** argv){

  // Time counting variables 
  struct timeval startwtime, endwtime;

  if (argc != 7) { // Check if the command line arguments are correct 
    printf("Usage: %s N thr dist\n"
      "where\n"
      "N    : number of points\n"
      "thr  : number of threads\n"
      "dist : distribution code (0-cube, 1-sphere)\n"
      "pop  : population threshold(128)\n"
      "rep  : repetitions(1)\n"
      "L    : maximum tree height.(18)\n", argv[0]);
    return (1);
  }
  char threads[10];
  // Input command line arguments
  int N = atoi(argv[1]); // Number of points
  int nthreads = atoi(argv[2]);
  int dist = atoi(argv[3]); // Distribution identifier 
  int population_threshold = atoi(argv[4]); // populatiton threshold
  int repeat = atoi(argv[5]); // number of independent runs
  int maxlev = atoi(argv[6]); // maximum tree height

  sprintf(threads,"%d",nthreads);
//  printf("Running for %d particles with distribution %d.\nThreads number: %s\n", N, dist,omp_get_num_threads());
     /* int x = omp_get_num_threads();
      printf("Max threads allowed: %d\n", omp_get_max_threads());
      printf("Threads number BEFORE openMP Set: %d\n",x);
      omp_set_num_threads(nthreads);
      x = omp_get_num_threads();   
      printf("OPENMP Threads: %d\n",x); */

  float *X = (float *) malloc(N*DIM*sizeof(float));
  float *Y = (float *) malloc(N*DIM*sizeof(float));
 omp_set_num_threads(nthreads);
  unsigned int *hash_codes = (unsigned int *) malloc(DIM*N*sizeof(unsigned int));
  unsigned long int *morton_codes = (unsigned long int *) malloc(N*sizeof(unsigned long int));
  unsigned long int *sorted_morton_codes = (unsigned long int *) malloc(N*sizeof(unsigned long int));
  unsigned int *permutation_vector = (unsigned int *) malloc(N*sizeof(unsigned int)); 
  unsigned int *index = (unsigned int *) malloc(N*sizeof(unsigned int));
  unsigned int *level_record = (unsigned int *) calloc(N,sizeof(unsigned int)); // record of the leaf of the tree and their level

  // initialize the index
  for(int i=0; i<N; i++){
    index[i] = i;
  }

  /* Generate a 3-dimensional data distribution */
  create_dataset(X, N, dist);

  /* Find the boundaries of the space */
  float max[DIM], min[DIM];
  find_max(max, X, N);
  find_min(min, X, N);

  int nbins = (1 << maxlev); // maximum number of boxes at the leaf level

  float sum_morton_time=0;
  float sum_hash_time	=0;
  float radix_sum_time=0;
  float s_time=0;
  // Independent runs
  for(int it = 0; it<repeat; it++){

    gettimeofday (&startwtime, NULL); 

    compute_hash_codes(hash_codes, X, N, nbins, min, max, nthreads); // compute the hash codes

    gettimeofday (&endwtime, NULL);

    double hash_time = (double)((endwtime.tv_usec - startwtime.tv_usec)
      /1.0e6 + endwtime.tv_sec - startwtime.tv_sec);
    


    gettimeofday (&startwtime, NULL); 

    morton_encoding(morton_codes, hash_codes, N, maxlev,nthreads); // computes the Morton codes of the particles

    gettimeofday (&endwtime, NULL);


    double morton_encoding_time = (double)((endwtime.tv_usec - startwtime.tv_usec)
      /1.0e6 + endwtime.tv_sec - startwtime.tv_sec);




    gettimeofday (&startwtime, NULL); 

    // Truncated msd radix sort
    truncated_radix_sort(morton_codes, sorted_morton_codes, 
      permutation_vector, 
      index, level_record, N, 
      population_threshold, 3*(maxlev-1), 0,nthreads);

    gettimeofday (&endwtime, NULL);

    double sort_time = (double)((endwtime.tv_usec - startwtime.tv_usec)
      /1.0e6 + endwtime.tv_sec - startwtime.tv_sec);


    gettimeofday (&startwtime, NULL); 

    // Data rearrangement
    data_rearrangement(Y, X, permutation_vector, N,nthreads);

    gettimeofday (&endwtime, NULL);


    double rearrange_time = (double)((endwtime.tv_usec - startwtime.tv_usec)
      /1.0e6 + endwtime.tv_sec - startwtime.tv_sec);
    

    if (it!=0){
    printf("Time to compute the hash codes   : %fs\n", hash_time);

    printf("Time to compute the morton encoding       : %fs\n", morton_encoding_time);

    printf("Time for the truncated radix sort         : %fs\n", sort_time);

    printf("Time to rearrange the particles in memory : %fs\n", rearrange_time);
    double total_time =  rearrange_time+sort_time+morton_encoding_time+hash_time;
    printf("Total time taken: %fs\n",total_time);
    }
    
    /* The following code is for verification */ 
    // Check if every point is assigned to one leaf of the tree
    int pass = check_index(permutation_vector, N); 

    if(pass){
      printf("Index test PASS\n");
    }
    else{
      printf("Index test FAIL\n");
    }

    // Check is all particles that are in the same box have the same encoding. 
    pass = check_codes(Y, sorted_morton_codes, 
     level_record, N, maxlev);

    if(pass){
      printf("Encoding test PASS\n");
    }
    else{
      printf("Encoding test FAIL\n");
    }

  }

  /* clear memory */
  free(X);
  free(Y);
  free(hash_codes);
  free(morton_codes);
  free(sorted_morton_codes);
  free(permutation_vector);
  free(index);
  free(level_record);
 /* printf("-------MEDIAN VALUES OF TIMES-------\n");
    printf("Time to compute the hash codes            : %fs\n", (sum_hash_time/(float)repeat));
    printf("Time to compute the morton encoding       : %fs\n", (sum_morton_time/(float)repeat));
	printf("Time for the truncated radix sort         : %fs\n", (radix_sum_time/(float)repeat));
	printf("Time to rearrange the particles in memory : %fs\n", (s_time/(float)repeat));

*/

}





