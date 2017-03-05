#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include <pthread.h>
#define MAXBINS 8


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct radix_args
{
    unsigned long int *morton_codes;
	unsigned long int *sorted_morton_codes;
	unsigned int *permutation_vector;
	unsigned int *index;
	unsigned int *level_record;
	int N;
	int population_threshold;
	int sft;
	int lv;
	int totalThreads;
	int *currentThreads;
};


inline void swap_long(unsigned long int **x, unsigned long int **y){

		unsigned long int *tmp;
		tmp = x[0];
		x[0] = y[0];
		y[0] = tmp;

}

inline void swap(unsigned int **x, unsigned int **y){

		unsigned int *tmp;
		tmp = x[0];
		x[0] = y[0];
		y[0] = tmp;

}
void *truncated_radix_sort(/*unsigned long int *morton_codes, 
							 unsigned long int *sorted_morton_codes, 
							 unsigned int *permutation_vector,
							 unsigned int *index,
							 unsigned int *level_record,
							 int N, 
							 int population_threshold,
							 int sft, int lvi*/void* arg){

		struct radix_args *one_args = (struct radix_args*) arg;
		unsigned long int *morton_codes = one_args->morton_codes;
		unsigned long int *sorted_morton_codes = one_args->sorted_morton_codes;
		unsigned int *permutation_vector = one_args->permutation_vector;
		unsigned int *level_record = one_args->level_record;
		unsigned int *index = one_args->index;
		int N = one_args->N;
		int population_threshold = one_args->population_threshold;
		int sft = one_args->sft;
		int lv = one_args->lv;
		int totalThreads=one_args->totalThreads;
		int *currentThreads = one_args->currentThreads;

		int BinSizes[MAXBINS] = {0};
		int BinCursor[MAXBINS] = {0};
		unsigned int *tmp_ptr;
		unsigned long int *tmp_code;


		if(N<=0){

				return NULL;
		}
		else if(N<=population_threshold || sft < 0) { // Base case. The node is a leaf

				level_record[0] = lv; // record the level of the node
				memcpy(permutation_vector, index, N*sizeof(unsigned int)); // Copy the pernutation vector
				memcpy(sorted_morton_codes, morton_codes, N*sizeof(unsigned long int)); // Copy the Morton codes 

				return NULL;
		}
		else{

				level_record[0] = lv;
	// Find which child each point belongs to 
				for(int j=0; j<N; j++){
						unsigned int ii = (morton_codes[j]>>sft) & 0x07;
						BinSizes[ii]++;
				}

				// scan prefix (must change this code)  
				int offset = 0;
				for(int i=0; i<MAXBINS; i++){
						int ss = BinSizes[i];
						BinCursor[i] = offset;
						offset += ss;
						BinSizes[i] = offset;
				}

				for(int j=0; j<N; j++){
						unsigned int ii = (morton_codes[j]>>sft) & 0x07;
						permutation_vector[BinCursor[ii]] = index[j];
						sorted_morton_codes[BinCursor[ii]] = morton_codes[j];
						BinCursor[ii]++;
				}

				//swap the index pointers  
				swap(&index, &permutation_vector);

				//swap the code pointers 
				swap_long(&morton_codes, &sorted_morton_codes);
				
				pthread_attr_t attr;
				pthread_attr_init(&attr);
				pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
				int t=0;
				pthread_t *threads = (pthread_t*)malloc(totalThreads*sizeof(pthread_t));
				struct radix_args *args = (struct radix_args*)malloc(MAXBINS*sizeof(struct radix_args));
				/* Call the function recursively to split the lower levels */
				for(int i=0; i<MAXBINS; i++){
						int offset = (i>0) ? BinSizes[i-1] : 0;
						int size = BinSizes[i] - offset;

						args[i].morton_codes= &morton_codes[offset];//+8*offset;
						args[i].sorted_morton_codes= &sorted_morton_codes[offset];//+8*offset;
						args[i].permutation_vector=&permutation_vector[offset];//+4*offset;
						args[i].index = &index[offset];//+4*offset;
						args[i].level_record = &level_record[offset];//+4*offset;
						args[i].N = size;
						args[i].population_threshold = population_threshold;
						args[i].sft = sft - 3;
						args[i].lv= lv+1;
						args[i].totalThreads=totalThreads;
						args[i].currentThreads=currentThreads;
						pthread_mutex_lock(&mutex);
						//MUCH SLOWER WITH PTHREAD_CREATE?
						if (*currentThreads < totalThreads && i<MAXBINS-1){
								(*currentThreads)++;
								pthread_mutex_unlock(&mutex);
								pthread_create(&threads[t],&attr,truncated_radix_sort,(void*)& args[i]);
								t++;
						}else{
							pthread_mutex_unlock(&mutex);
							truncated_radix_sort((void*)&args[i]);
						}	

				}
				for (int j=0; j<t; j++){
						pthread_join(threads[j],NULL);
				}
				pthread_mutex_lock(&mutex);
				*currentThreads= *currentThreads-t;
				pthread_mutex_unlock(&mutex);
				free(threads);
				free(args);
			   	
				//    pthread_exit(NULL);  
		} 
		//  printf("%d\n",COUNT);
}