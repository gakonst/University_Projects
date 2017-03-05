#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <omp.h>
#include <mpi.h>
#include <game-of-life.h>

/* set everthing to zero */

void initialize_board (int *board, int M, int N){
    int   i, j;

    for (i=0; i<M; i++)
        for (j=0; j<N; j++) 
            Board(i,j) = 0;
}

/* generate random table */

void generate_table (int *board, int M, int N, float threshold,int tid) {

    long int seed;
    double random;
    struct drand48_data rand;
    #pragma omp parallel private(rand, random, seed) 
    {
        seed = time(NULL)+100*tid;
        srand48_r(seed, &rand);

        int   i, j;
        #pragma omp for private(j)
        for (i=0; i<M; i++) {
            for (j=0; j<N; j++) {
                // Cell = 1 if rand num < prob to be alive, otherwise = 0
                drand48_r(&rand, &random);
                Board(i,j) = random < threshold;
            }
        }
    }
}

