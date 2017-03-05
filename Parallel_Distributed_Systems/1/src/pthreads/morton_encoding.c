#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "pthread.h"
#define DIM 3

//#define Threads 8
typedef struct{
    unsigned long int *mcodes;
    unsigned int *codes;
    int N;
    int i;
    int start;
    int finish;

}morton;


inline unsigned long int splitBy3(unsigned int a){
    unsigned long int x = a & 0x1fffff; // we only look at the first 21 bits
    x = (x | x << 32) & 0x1f00000000ffff;  // shift left 32 bits, OR with self, and 00011111000000000000000000000000000000001111111111111111
    x = (x | x << 16) & 0x1f0000ff0000ff;  // shift left 32 bits, OR with self, and 00011111000000000000000011111111000000000000000011111111
    x = (x | x << 8) & 0x100f00f00f00f00f; // shift left 32 bits, OR with self, and 0001000000001111000000001111000000001111000000001111000000000000
    x = (x | x << 4) & 0x10c30c30c30c30c3; // shift left 32 bits, OR with self, and 0001000011000011000011000011000011000011000011000011000100000000
    x = (x | x << 2) & 0x1249249249249249;
    return x;
}


void *mortonEncode_magicbits(void *arg){
    unsigned long int answer;
    morton *enc = (morton *) arg;
    int start = enc->start;
    int finish = enc->finish;
    
    for (int i=start;i<finish;i++){
        unsigned int *codes = enc->codes;
        unsigned long int *mcodes = enc->mcodes; 
        answer = splitBy3(codes[i*DIM]) | splitBy3(codes[i*DIM+1]) << 1 | splitBy3(codes[i*DIM+2]) << 2;
        mcodes[i] = answer;

    }   
}


/* The function that transform the morton codes into hash codes */ 
void morton_encoding(unsigned long int *mcodes, unsigned int *codes, int N, int max_level, int Threads){

    morton arr[Threads];
    pthread_t threads[Threads];
    int start=0;
    int finish=N/Threads;
    for(int i=0; i<Threads; i++){
        arr[i].codes=codes;
        arr[i].mcodes=mcodes;
        arr[i].N=N;
        arr[i].start = start;
        arr[i].finish = finish;
        pthread_create(&threads[i],  NULL,mortonEncode_magicbits, (void*)&arr[i]);
        start+=N/Threads;
        finish+=N/Threads;
        // Compute the morton codes from the hash codes using the magicbits mathod
        //    mcodes[i] = mortonEncode_magicbits(codes[i*DIM], codes[i*DIM + 1], codes[i*DIM + 2]);
    }
    for (int i=0;i<Threads;i++){
        pthread_join(threads[i],NULL);
    }

}


