#ifndef POOL_H
#define POOL_H
#define POOL_SIZE 8

/**
 * Used to track hint indexes (item/chars/rooms)
 */
struct Pool {
  size_t taken[POOL_SIZE];
  size_t vec[POOL_SIZE];
  size_t length;
  size_t exclude_index;
};

/**
 * Create a pool of 8 indexes excluding exclude_index.
 * Used for tracking hints. 
 * An index that was taken 2 times will be removed from the pool.
 */
struct Pool makePool(size_t exclude_index);
 
/**
 * MUST BE CALLED BEFORE takeFromPool();
 * Pick one index randomly and takes 2 times from the pool,
 * that will be removed from the pool.
 * IMPORTANT: Assumes is never called when pool length is 0 otherwise an error is given
 */
size_t takeTwoFromPool(struct Pool *pool);

/**
 * Pick one index randomly
 * and remove it from the pool if the index was taken before
 * IMPORTANT: Assumes is never called when pool length is 0 otherwise an error is given
 */
size_t takeFromPool(struct Pool *pool);

#endif