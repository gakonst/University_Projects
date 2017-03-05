#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <game-of-life.h>
#include "mpi.h"
#include "omp.h"


enum neighbor {
    TOP_NEIGHBOR, BOT_NEIGHBOR
}neighbor;

void play (int *board, int *newboard, int M, int N, int tasks, int tid) {
    /*

       1.STASIS : If, for a given cell, the number of on neighbours is 
       exactly two, the cell maintains its status quo into the next 
       generation. If the cell is on, it stays on, if it is off, it stays off.

       2.GROWTH : If the number of on neighbours is exactly three, the cell 
       will be on in the next generation. This is regardless of the cell's
       current state.

       3.DEATH : If the number of on neighbours is 0, 1, 4-8, the cell will 
       be off in the next generation.

*/

    int   i, j;

    int *top_line = (int *) malloc(N * sizeof(int));
    int *bot_line = (int *) malloc(N * sizeof(int));
    MPI_Request request[4];
    MPI_Status status[2];

    //Destination is next rank process, if at max -> send to 0 (cyclic conditions)
    /*
       Hangs for some reason
       int NEXT = (tid+1) % tasks;
       int PREV = (tid-1) % tasks;
       */

    int PREV = tid-1 < 0 ? tasks-1 : tid-1;
    int NEXT  = tid+1 == tasks ? 0 : tid+1;

    if (tasks==1) {
        bot_line = &Board(0,0);
        top_line = &Board(N-1,0);
	#pragma omp parallel for private (j) 
        for (i=0; i<M; i++)
            for (j=0; j<N; j++) {
                int a = adjacent_to (board, i, j, M, N, top_line, bot_line);
                if (a == 2) NewBoard(i,j) = Board(i,j);
                if (a == 3) NewBoard(i,j) = 1;
                if (a < 2) NewBoard(i,j) = 0;
                if (a > 3) NewBoard(i,j) = 0;
            }

    }
    else{
        //Send & recv rows
        MPI_Isend(&Board(0,0), N, MPI_INT, PREV, BOT_NEIGHBOR, MPI_COMM_WORLD, &request[0]);
        MPI_Isend(&Board(M-1,0), N, MPI_INT, NEXT, TOP_NEIGHBOR, MPI_COMM_WORLD, &request[1]);

        MPI_Irecv(top_line, N, MPI_INT, PREV, TOP_NEIGHBOR, MPI_COMM_WORLD, &request[2]);
        MPI_Irecv(bot_line, N, MPI_INT, NEXT, BOT_NEIGHBOR, MPI_COMM_WORLD, &request[3]);

        //Play the game of life for all rows except for the first and the last.
	#pragma omp parallel for private (j)
        for (i=1; i<M-1; i++){
            for (j=0; j<N; j++) {
                int  a = adjacent_to (board, i, j, M, N, top_line, bot_line);
                if (a == 2) NewBoard(i,j) = Board(i,j);
                if (a == 3) NewBoard(i,j) = 1;
                if (a < 2) NewBoard(i,j) = 0;
                if (a > 3) NewBoard(i,j) = 0;
            }
        }
        //Wait until top-bot lines are received
        MPI_Waitall(2, &request[2], status); 

        //Same process, for first-last row
        for(int z=0; z<2; z++){
            i = (z == 0) ? 0 : M-1;    
#pragma omp parallel for 
            for (j=0; j<N; j++) {
                int a = adjacent_to (board, i, j, M, N, top_line, bot_line);
                if (a == 2) NewBoard(i,j) = Board(i,j);
                if (a == 3) NewBoard(i,j) = 1;
                if (a < 2) NewBoard(i,j) = 0;
                if (a > 3) NewBoard(i,j) = 0;
            }
        }
        //Wait until first-last lines are sent. If not -> Wrong of life.
        MPI_Waitall(2, &request[0], status);

    }

    /* copy the new board back into the old board */
    /*    #pragma omp parallel for private(i,j) 
          for (i=0; i<N; i++)
          for (j=0; j<M; j++) {
          Board(i,j) = NewBoard(i,j);
          }*/


}
