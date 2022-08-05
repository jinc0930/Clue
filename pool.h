const int POOL_SIZE = 7;

/**
 * Used to track item/chars/rooms indexes
 * for the purpose of generating, removing and tracking hints.
 */
struct Pool {
  int taken[POOL_SIZE];
  int vec[POOL_SIZE];
  int length;
  int target_index;
};

// Create a pool of size 8. Skipping target_index that will be the target answer.
struct Pool makePool(unsigned int target_index);

// MUST BE CALLED BEFORE takeRandom(); Take 2 exact same index from the pool randomly.
int takeTwo(struct Pool *pool);

// Take one index from the pool randomly
int takeRandom(struct Pool *pool);