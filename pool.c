#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pool.h"

struct Pool makePoolExcluding(int pool_desired_size, int exclude_indexes[], int exclude_indexes_size) {
	struct Pool pool = {};
	int r = 0;
	// ensure it fits the array
	assert(pool_desired_size <= MAX_POOL_SIZE);
	for (int i = 0; i < pool_desired_size; i++){
		bool include = true;
		for(int j = 0; j < exclude_indexes_size; j++){
			if(exclude_indexes[j] == i) {
				include = false;
				break;
			};
		}
		if (include) {
			pool.vec[r++] = i;
			pool.length += 1;
		}
	}
	return pool;
}

struct Pool makePool(int pool_desired_size) {
	struct Pool pool = {};
	// ensure it fits the array
	assert(pool_desired_size <= MAX_POOL_SIZE);
	for (int i = 0; i < pool_desired_size; i++){
		pool.vec[i] = i;
		pool.length += 1;
	}
	return pool;
}

int poolTake(struct Pool *pool) {
	// this function requires pool to be length > 0;
	if (pool->length <= 0) return -1; 
	int take = rand() % pool->length;
  	int ref = pool->vec[take];
	for(int i = take; i < pool->length - 1; i++) pool->vec[i] = pool->vec[i + 1];
	pool->length -= 1;
	return ref;
}

int poolTakeExcluding(struct Pool *pool, int exlude) {
	// this function requires pool to be length > 0;
	if (pool->length <= 0) return -1; 
	int temp[MAX_POOL_SIZE];
	int size = 0;
	for(int j = 0; j < pool->length; j++){
		if(pool->vec[j] != exlude) {
			temp[j] = j;
			size++;
		};
	}

	int take = rand() % size;
  	int ref = pool->vec[temp[take]];
	for(int i = temp[take]; i < pool->length - 1; i++) pool->vec[i] = pool->vec[i + 1];
	pool->length -= 1;
	return ref;
}

int poolChoose(struct Pool *pool) {
	// this function requires pool to be length > 0;
	if (pool->length <= 0) return -1; 
	int take = rand() % pool->length;
  	int ref = pool->vec[take];
	return ref;
}