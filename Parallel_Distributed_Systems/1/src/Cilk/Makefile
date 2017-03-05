# GCC=gcc -O3 -std=gnu99
ICC = icc -O3 
VERSION=0.04

all:
	$(ICC) test_octree.c datasets.c general_functions.c hash_codes.c morton_encoding.c radix_sort.c data_rearrangement.c verification.c -o cilk_octree

clean:
	rm -f *~ *.o *.bin cilk_octree 

release:
	tar -cvf octree-code-$(VERSION).tar *.c *.h *m *sh Makefile

