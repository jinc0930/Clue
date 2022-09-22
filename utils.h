#include <string.h>
#include <stdbool.h>
#ifndef UTILS_H
#define UTILS_H
#define MAX_POOL_SIZE 9
// check if string starts with prefix and remove the trailing \n
bool startsWith(const char *prefix, const char *str);

// get a slice of a string
void slice(const char *str, char *result, size_t start, size_t end);

// swap ints
void swap(int *a, int *b);

// shuffle array
void shuffle(int arr[], int n);

/**
 * Used for tracking hints / randomize indexes / etc.
 */
struct Pool {
  int vec[MAX_POOL_SIZE];
  int length;
};

/**
 * Create a pool of pool_desired_size indexes excluding exclude_indexes[].
 * Final length will be pool_desired_size - lenght of exclude_indexes[].
 * Used for tracking hints / randomize indexes / etc.
 */
struct Pool makePoolExcluding(int pool_desired_size, int exclude_indexes[], int exclude_indexes_size);

// Convenient method for makePoolExcluding() to create a pool without excluding
struct Pool makePool(int pool_desired_size);

/**
 * Pick one index randomly and remove the index if the index
 * If pool is empty it returns -1
 */
int poolTake(struct Pool *pool);


/**
 * Pick one index randomly exluding exclude_idx and remove the index if the index
 * If pool is empty it returns -1
 */
int poolTakeExcluding(struct Pool *pool, int exclude_idx); 

// Choose one index randomly, but it won't remove from the pool and it won't count as take;
int poolChoose(struct Pool *pool);

#endif