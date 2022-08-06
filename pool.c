#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "pool.h"

struct Pool makePool(size_t exclude_index) {
	struct Pool pool = { .taken = { 0 }, .length = POOL_SIZE, .exclude_index = exclude_index };
	size_t r = 0;
	assert(exclude_index < 9);
	for (size_t i = 0; i < POOL_SIZE; i++){
		if (i == exclude_index) continue;
		pool.vec[r++] = i;
		pool.length += 1;
	}
	return pool;
}

size_t takeTwoFromPool(struct Pool *pool) {
	// this function requires pool length > 0;
	assert(pool->length > 0);
	size_t take = rand() % pool->length;
  size_t ref = pool->vec[take];
	// assumes takeTwoFromPool() is called before takeFromPool();
	assert(pool->taken[ref] == 0);
	// shift pool elements to the left
	for(size_t i = take; i < pool->length - 1; i++) pool->vec[i] = pool->vec[i + 1];
	pool->length -= 1;
	pool->taken[ref] += 2;
	return ref;
}

size_t takeFromPool(struct Pool *pool) {
	// this function require pool length > 0;
	assert(pool->length > 0);
	size_t take = rand() % pool->length;
  size_t ref = pool->vec[take];
	if (pool->taken[ref] > 1)  {
		// shift pool elements to the left
		for(size_t i = take; i < pool->length - 1; i++) pool->vec[i] = pool->vec[i + 1];
		pool->length -= 1;
	};
	pool->taken[ref] += 1;
	return ref;
}