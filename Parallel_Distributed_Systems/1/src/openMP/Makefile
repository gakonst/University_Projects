# GCC=gcc -O3 -std=gnu99
ICC = gcc -O3 -fopenmp  
VERSION=0.04

all:
	$(ICC) test_octree.c datasets.c general_functions.c hash_codes.c morton_encoding.c radix_sort.c data_rearrangement.c verification.c -o openmp_tree -lm

clean:
	rm -f *~ *.o *.bin openmp_tree 

release:
	tar -cvf octree-code-$(VERSION).tar *.c *.h *m *sh Makefile

