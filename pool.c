#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "pool.h"

struct Pool makePool(int exclude_index, int pool_size, int max_takes) {
	struct Pool pool = { .taken = { 0 }, .length = pool_size, .max_get = max_takes, .exclude_index = exclude_index };
	int r = 0;
	// ensure exclude_index is within the pool size;
	assert(exclude_index < MAX_POOL_SIZE);
	for (int i = 0; i < pool_size; i++){
		if (i == exclude_index) continue;
		pool.vec[r++] = i;
		pool.length += 1;
	}
	return pool;
}

int poolGet(struct Pool *pool, int quantity) {
	// this function requires pool to be length > 0;
	if (pool->length <= 0) return -1; 
	int take = rand() % pool->length;
  	int ref = pool->vec[take];
	if (pool->taken[ref] += quantity >= pool->max_get)  {
		// shift pool elements to the left
		for(int i = take; i < pool->length - 1; i++) pool->vec[i] = pool->vec[i + 1];
	}
	pool->length -= 1;
	return ref;
}