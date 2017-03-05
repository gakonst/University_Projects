#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include <cilk/cilk.h>
#define MAXBINS 8

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

void truncated_radix_sort(unsigned long int *morton_codes, 
			  unsigned long int *sorted_morton_codes, 
			  unsigned int *permutation_vector,
			  unsigned int *index,
			  unsigned int *level_record,
			  int N, 
			  int population_threshold,
			  int sft, int lv){

  int BinSizes[MAXBINS] = {0};
  int BinCursor[MAXBINS] = {0};
  unsigned int *tmp_ptr;
  unsigned long int *tmp_code;


  if(N<=0){

    return;
  }
  else if(N<=population_threshold || sft < 0) { // Base case. The node is a leaf

    level_record[0] = lv; // record the level of the node

    //Can make them go parallel as they copy diff data to diff memory. 
    cilk_spawn memcpy(permutation_vector, index, N*sizeof(unsigned int)); // Copy the pernutation vector
    /*cilk_spawn*/ memcpy(sorted_morton_codes, morton_codes, N*sizeof(unsigned long int)); // Copy the Morton codes	
	cilk_sync;		

    return;
  }
  else{

		  level_record[0] = lv;

		  // Cant parallelize the following 3 loops. By making it parallel we might lose on some values due
		  // to race conditions
		  // 
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
		  cilk_spawn swap(&index, &permutation_vector);

		  //swap the code pointers 
		  /*cilk_spawn*/ swap_long(&morton_codes, &sorted_morton_codes);
		  cilk_sync;
		  /* Call the function recursively to split the lower levels */
		  /* cilk_for(int i=0; i<MAXBINS; i++){
			 int offset = (i>0) ? BinSizes[i-1] : 0;
			 int size = BinSizes[i] - offset;

			 truncated_radix_sort(&morton_codes[offset], 
			 &sorted_morton_codes[offset], 
			 &permutation_vector[offset], 
			 &index[offset], &level_record[offset], 
			 size, 
			 population_threshold,
			 sft-3, lv+1);
			 }*/
		  for(int i=0; i<MAXBINS; i++){
				  int offset = (i>0) ? BinSizes[i-1] : 0;
				  int size = BinSizes[i] - offset; 

				  if (i<MAXBINS -1){
						  cilk_spawn truncated_radix_sort(&morton_codes[offset], 
										  &sorted_morton_codes[offset], 
										  &permutation_vector[offset], 
										  &index[offset], &level_record[offset], 
										  size, 
										  population_threshold,
										  sft-3, lv+1);
				  }else{

						  truncated_radix_sort(&morton_codes[offset], 
										  &sorted_morton_codes[offset], 
										  &permutation_vector[offset], 
										  &index[offset], &level_record[offset], 
										  size, 
										  population_threshold,
										  sft-3, lv+1);
				  }



		  }
		 cilk_sync; 
  }
}
