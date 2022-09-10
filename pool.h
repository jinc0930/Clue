#ifndef POOL_H
#define POOL_H
#define MAX_POOL_SIZE 9

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