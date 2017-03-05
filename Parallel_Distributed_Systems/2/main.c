/*
 * Game of Life implementation based on
 * http://www.cs.utexas.edu/users/djimenez/utsa/cs1713-3/c/life.txt
 * 
 */

#include "sys/time.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "omp.h"
#include <game-of-life.h>
#include "mpi.h"
int main (int argc, char *argv[]) {
    int   *board, *newboard, i;
    double totalTime;
    struct timeval startwtime, endwtime;

    if (argc !=7) { // Check if the command line arguments are correct 
        printf("Usage: %s N M T thres disp\n"
                "where\n"
                "  M     : rows of table\n"
                "  N   : columns of table\n" 
                "  T     : the number of threads\n"
                "  thres : propability of a cell to be alive \n"
                "  t     : number of generations\n"
                "  disp  : {1: display output, 0: hide output}\n"
                , argv[0]);
        return (1);
    }

    // Input command line arguments
    int M = atoi(argv[1]);        // Array rows
    int N = atoi(argv[2]);        // Array columns
    int threads = atoi(argv[3]);
    double thres = atof(argv[4]); // Probability of life cell
    int t = atoi(argv[5]);        // Number of generations 
    int disp = atoi(argv[6]);     // Display output?

    omp_set_dynamic(0);
    omp_set_num_threads(threads);
    
    int tid, tasks;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &tid); // Curent thread id
    MPI_Comm_size(MPI_COMM_WORLD, &tasks); // Total num of tasks | Set by -np flag @mpiexec


    //SYNC
    MPI_Barrier(MPI_COMM_WORLD);
    gettimeofday(&startwtime, NULL);   //TIC

    board = NULL;
    newboard = NULL;

    M = M / tasks ; // Split by rows per task

    board = (int *)malloc(N*M*sizeof(int));
    if (board == NULL){
        printf("\nERROR: Memory allocation did not complete successfully!\n");
        return (1);
    }
    newboard = (int *)malloc(N*M*sizeof(int));
    if (newboard == NULL){
        printf("\nERROR: Memory allocation did not complete successfully!\n");
        return (1);
    }

    //Send task id as argument so that the seed is different each time.
    generate_table (board, M,N, thres, tid);

    /* play game of life t times */

    for (i=0; i<t; i++) {
        //printf("\nIn %d generation | Sending %d task", i, tid);
        if (disp) display_table (board, M, N);
        play (board, newboard, M, N, tasks, tid);    

        //printf("\nSwapping\n");

        //Faster to swap via pointers instead of value by value
        int *temp = newboard;
        newboard = board;
        board = temp;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

    gettimeofday(&endwtime ,NULL); //TOC
    totalTime = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6 + endwtime.tv_sec - startwtime.tv_sec);

    printf("Time elapsed playing for %d generations in task %d = %fs\n\n", t, tid,  totalTime);
    return 0;
}
