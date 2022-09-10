#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../game.h"
#include "../items.h"
#include "../character.h"
#include "../pool.h"
#include "../utils.h"
#include "../rooms.h"

#define TEST(t) ({ puts("-> " #t " testing "); t(); })
#define SUBTEST(t) ({ puts("  -> " t " testing"); })

// POOL
static void test_pool_basic() {
  struct Pool pool = makePool(9);
  assert(pool.length == 9);
  for (size_t i = 0; i < 9; i++) {
    assert(pool.vec[i] == i);
  }
  int taken[9] = { [0 ... 8] = -1 };
  for (size_t i = 0; i < 9; i++) {
    taken[i] = poolTake(&pool);
    assert(taken[i] >= 0 && taken[i] < 9); // in range
    for (size_t j = 0; j < 9; j++) {
      if (i != j) { assert(taken[j] != taken[i]); } // check for duplicates
    }
  }
  assert(pool.length == 0); // consumed the pool?
}

static void test_pool_excluding() {
  int excluding[] = {5, 1};
  struct Pool pool = makePoolExcluding(9, excluding, 2);
  assert(pool.length == 7);
  int taken[9] = { [0 ... 8] = -1 };
  int len = pool.length;
  for (size_t i = 0; i < len; i++) {
    taken[i] = poolTake(&pool);
    assert(taken[i] >= 0 && taken[i] < 9); // in range
    assert(taken[i] != 5 && taken[i] != 1); // must exclude
    for (size_t j = 0; j < len; j++) {
      if (i != j) { assert(taken[j] != taken[i]); } // check for duplicates
    }
  }
  // must be 0 since pool is excluded 2 elements and consumed the remainder
  assert(pool.length == 0);
}

static void test_pool_excluding_caller() {
  struct Pool pool = makePool(9);
  int taken[9] = { [0 ... 8] = -1 };
  for (size_t i = 0; i < 8; i++) {
    taken[i] = poolTakeExcluding(&pool, 3);
    assert(taken[i] != 3);
    for (size_t j = 0; j < 9; j++) {
      if (i != j) { assert(taken[j] != taken[i]); } // check for duplicates
    }
  }
  assert(poolTakeExcluding(&pool, 3) == -1); // must return -1 since there's no element dinstinct from 3
  assert(pool.length == 1); // the exclude still exists
  assert(poolTake(&pool) == 3); // must be the number 3 (that was excluded earlier)
}

// UTILS
static void test_utils() {
  SUBTEST("test_starts_with");
  assert(startsWith("abcd", "abcd"));
  assert(startsWith("a", "abcd"));
  assert(startsWith("ab", "ab"));
  assert(startsWith("a", "aaaa"));
  assert(startsWith("aa", "aa\n"));
  assert(startsWith("\n", "\n"));
  assert(startsWith("", ""));
  assert(!startsWith("abcd", "a"));
  assert(!startsWith("aaaaa", "a"));
  assert(!startsWith("xyz", "xy\n"));
  assert(!startsWith("xy\n", "xy"));
  
  SUBTEST("test_slice");
  char direction[10] = { 0 };
  slice("go north", direction, strlen("go "), 10);
  assert(strcmp(direction, "north") == 0);
}

// ITEMS
static void test_item() {
  struct Item * item = makeitem("sword");
  assert(strcmp(item->name, "sword") == 0);
  struct Item * item2 = makeitem("rope");
  setNext(item, item2);
  assert(strcmp(item->name, "rope"));
}

// CHARACTERS
static void test_character() {
  struct Character * character = makeChar("chang");
  assert(strcmp(character->name, "chang") == 0);

  // add
  struct Item * item = makeitem("hammer");
  SUBTEST("test_item_add");
  add(character, item);
  assert(character->inventoryItems == 1);
  assert(strcmp(item->name, "hammer") == 0);
  assert(character->inventory->next == NULL);

  // rmv
  SUBTEST("test_character_rmv");
  rmv(character, item);
  assert(character->inventoryItems == 0);
  assert(character->inventory == NULL);

  // add multiple
  SUBTEST("test_multi_add");
  add(character, makeitem("scissors"));
  add(character, makeitem("hammer"));
  add(character, makeitem("marker"));
  assert(character->inventoryItems == 3);
  assert(character->inventory != NULL);
  struct Item * inv = character->inventory;
  assert(strcmp(inv->name, "scissors") == 0);
  inv = inv->next;
  assert(strcmp(inv->name, "hammer") == 0);
  inv = inv->next;
  assert(strcmp(inv->name, "marker") == 0);
  inv = inv->next;
  assert(inv == NULL);

  // check again
  assert(character->inventoryItems == 3);
  assert(character->inventory != NULL);
}

static void test_rooms() {
  // FIXME todo rooms
}

// GAME
static void test_game() {
  struct Game game = makeGame();
  assert(game.attempts == 0);
  assert(game.okChar == false);
  assert(game.okItem == false);
  assert(game.okRoom == false);
}

// GAMEPLAY
static void test_gameplay() {
  struct Game game = makeGame();
  int avatar_idx = initGame(&game, "test");
  assert(game.attempts == 0);
  
  // map
  SUBTEST("map");
  assert(game.attempts == 0);
  assert(game.avatar != NULL);
  assert(game.targetChar != NULL);
  assert(game.targetItem != NULL);
  assert(game.targetRoom != NULL);
  assert(game.map != NULL);
  assert(game.items != NULL);
  assert(game.avatar == game.characters[avatar_idx]);
  
  int player_position = 0; 
  for (size_t i = 0; i < 9; i++) {
    assert(game.map[i] != NULL);
    assert(game.map[i]->chara != NULL);
    if (game.map[i]->visited) {
      assert(strcmp(game.map[i]->chara[0]->name, "test") == 0);
      player_position = i;
    }
    assert(game.map[i]->itemList != NULL);
    assert(game.map[i]->chara[0] != NULL);
  }

  // from middle
  SUBTEST("movements");
  for(int i=North; i<=South; i++) {
    // swap player to the middle if necessary
    void*temp = game.map[4]->chara[0];
    game.map[4]->chara[0] = game.map[player_position]->chara[0];
    game.map[player_position]->chara[0] = temp;
    game.avatar->location = game.map[4];
    assert(move(&game, i) > -1);
    assert(move(&game, i) == -1);
    if (i == North || i == South) {
      assert(move(&game, West) > -1);
      assert(move(&game, West) == -1);
      assert(move(&game, East) > -1);
      assert(move(&game, East) > -1);
      assert(move(&game, East) == -1);
    } else if (i == West || i == East) {
      assert(move(&game, North) > -1);
      assert(move(&game, North) == -1);
      assert(move(&game, South) > -1);
      assert(move(&game, South) > -1);
      assert(move(&game, South) == -1);
    }
  }

  SUBTEST("take");
  // FIXME todo take

  SUBTEST("drop");
  // FIXME todo take

  SUBTEST("clue");
  // FIXME todo clue
}

// TESTS
int main(void) {
  srand(20);
  TEST(test_utils);
  TEST(test_pool_basic);
  TEST(test_pool_excluding);
  TEST(test_pool_excluding_caller);
  TEST(test_item);
  TEST(test_character);
  TEST(test_rooms);
  TEST(test_game);
  TEST(test_gameplay);
  return 0;
}