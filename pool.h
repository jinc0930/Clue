#ifndef POOL_H
#define POOL_H
#define MAX_POOL_SIZE 9

/**
 * Used for tracking hints / randomize indexes / etc.
 */
struct Pool {
  int taken[MAX_POOL_SIZE];
  int vec[MAX_POOL_SIZE];
  int length;
  int exclude_index;
  int max_get;
};

/**
 * Create a pool of pool_size indexes excluding exclude_index.
 * Used for tracking hints / randomize indexes / etc.
 * An index that was taken max_get times will be removed from the pool.
 */
struct Pool makePool(int exclude_index, int pool_size, int max_get);
 
/**
 * Pick one index randomly
 * and remove the index if the index was taken >= max_get.
 * quantity arg increment the counter 
 * If pool is empty it returns -1
 */
int poolGet(struct Pool *pool, int quantity);

#endif