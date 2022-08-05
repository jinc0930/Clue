#include <stdio.h>

const int POOL_SIZE = 7;

struct Pool {
  int taken[POOL_SIZE];
  int vec[POOL_SIZE];
  int length;
  int target_index;
};

struct Pool makePool(unsigned int target_index){
	struct Pool pool = { .taken = { 0 }, .length = POOL_SIZE, .target_index = target_index };
	int r = 0;
	assert(target_index < 9);
	for (size_t i = 0; i < POOL_SIZE; i++){
		if (i == target_index) continue;
		pool.vec[r++] = i;
		pool.length += 1;
	}
	return pool;
}

int takeTwo(struct Pool *pool){
	int take = rand() % pool->length;
  	int ref = pool->vec[take];
	for(int i = take; i < pool->length - 1; i++) pool->vec[i] = pool->vec[i + 1];
	pool->length -= 1;
	pool->taken[ref] += 2;
	return take;
}

int takeRandom(struct Pool *pool){
	int take = rand() % pool->length;
  	int ref = pool->vec[take];
	if (pool->taken[ref] > 1)  {
		for(int i = take; i < pool->length - 1; i++) pool->vec[i] = pool->vec[i + 1];
		pool->length -= 1;
	};
	pool->taken[ref] += 1;
	return take;
}