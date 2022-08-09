#ifndef POOL_H
#define POOL_H
#define MAX_POOL_SIZE 9

/**
 * Used for tracking hints / randomize indexes / etc.
 */
struct Pool {
  int take_counter[MAX_POOL_SIZE];
  int vec[MAX_POOL_SIZE];
  int length;
  int max_take;
};

/**
 * Create a pool of pool_desired_size indexes excluding exclude_indexes[].
 * Final length will be pool_desired_size - lenght of exclude_indexes[].
 * Used for tracking hints / randomize indexes / etc.
 * An index that was taken max_take times will be removed from the pool.
 */
struct Pool makePoolExcluding(int pool_desired_size, int max_take, int exclude_indexes[], int exclude_indexes_size);

// Convenient method for makePoolExcluding() to create a pool without excluding
struct Pool makePool(int pool_desired_size, int max_take);

/**
 * Pick one index randomly and remove the index if the index was taken >= max_take.
 * Increment counter by quantity
 * If pool is empty it returns -1
 */
int poolTake(struct Pool *pool, int quantity_take);


// Choose one index randomly, but it won't remove from the pool and it won't count as take;
int poolChoose(struct Pool *pool);
#endif