#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "utils.h"

bool startsWith(const char *prefix, const char *str) {
    bool starts = false;
    if (strncmp(prefix, str, strlen(prefix)) == 0) starts = true;
    return starts;
}

void slice(const char *str, char *result, size_t start, size_t end) {
    strncpy(result, str + start, end - start);
}

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void shuffle(int arr[], int n ){
    for (int i = n-1; i > 0; i--) {
        int j = rand() % (i+1);
        swap(&arr[i], &arr[j]);
    }
}

// POOL

struct Pool makePoolExcluding(int pool_desired_size, int exclude_indexes[], int exclude_indexes_size) {
	struct Pool pool = {.length = 0, .vec = { 0 }};
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
		if (include == true) {
			pool.vec[r++] = i;
			pool.length += 1;
		}
	}
	return pool;
}

struct Pool makePool(int pool_desired_size) {
	struct Pool pool = {.length = 0, .vec = { 0 }};
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

int poolTakeExcluding(struct Pool *pool, int exclude) {
	// this function requires pool to be length > 0;
	if (pool->length <= 0) return -1; 
	int temp[MAX_POOL_SIZE];
	int size = 0;
	for(int j = 0; j < pool->length; j++){
		if(pool->vec[j] != exclude) {
			temp[size++] = j;
		};
	}
	if (size <= 0) return -1; // no elements left

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