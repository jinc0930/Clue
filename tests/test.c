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

#define TEST(t) ({ puts("-> " #t); t(); })
#define SUBTEST(t) ({ puts("  -> " t); })

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
  // must be 0 since pool excluded 2 elements and consumed the remainder
  assert(pool.length == 0);
}

static void test_pool_excluding_caller() {
  struct Pool pool = makePool(9);
  int taken[9] = { [0 ... 8] = -1 };
  for (size_t i = 0; i < 8; i++) {
    taken[i] = poolTakeExcluding(&pool, 3); //exclude 3
    assert(taken[i] != 3);
    for (size_t j = 0; j < 9; j++) {
      if (i != j) { assert(taken[j] != taken[i]); } // check for duplicates
    }
  }
  assert(poolTakeExcluding(&pool, 3) == -1); // must return -1 since there's no element dinstinct from 3
  assert(pool.length == 1); // in this case the pool didn't consume the excluded
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
  slice("talk foo", direction, strlen("talk "), 10);
  assert(strcmp(direction, "foo") == 0);
}

// ITEMS
static void test_item() {
  struct Item * item = makeitem("sword");
  assert(strcmp(item->name, "sword") == 0);
  struct Item * item2 = makeitem("rope");
  setNext(item, item2);
  assert(strcmp(item->name, "rope"));
  assert(strcmp(item->next->name, "sword"));
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
  struct Item *one = makeitem("rope");
  struct Item *two = makeitem("bat");
  struct Item *three = makeitem("dagger");
  add(character, one);
  add(character, two);
  add(character, three);
  assert(character->inventoryItems == 3);
  assert(character->inventory != NULL);
  struct Item * inv = character->inventory;
  assert(strcmp(inv->name, "dagger") == 0);
  inv = inv->next;
  assert(strcmp(inv->name, "bat") == 0);
  inv = inv->next;
  assert(strcmp(inv->name, "rope") == 0);
  assert(inv->next == NULL);
  assert(character->inventoryItems == 3);
  assert(character->inventory != NULL);

  SUBTEST("test_multi_rmv");
  rmv(character, two);
  rmv(character, three);
  rmv(character, one);
  assert(character->inventoryItems == 0);
  assert(character->inventory == NULL);
  rmv(character, two);
  assert(character->inventoryItems == 0);
  assert(character->inventory == NULL);
}

static void test_rooms() {
  struct Room * room = makeroom("ballroom");
  assert(strcmp(room->name, "ballroom") == 0);
  SUBTEST("add_char");
  char* chars[] = {"minh","james","ivan"};
  for (int i = 0; i < 3; i++) {
    struct Character * character = makeChar(chars[i]);
    assert(addChar(room, character) != -1);
    assert(isCharInside(room, chars[i]));
  }

  SUBTEST("add_item");
  char* items[] = {"butter knife","bat","wrench"};
  for (int i = 0; i < 3; i++) {
    struct Item * item = makeitem(items[i]);
    additem(room, item);
    assert(room->itemList != NULL);
    assert(strcmp(room->itemList->name, items[i]) == 0);
  }

  SUBTEST("check items");
  struct Item * temp = room->itemList;
  for (int i = 3; i-- > 0; ) {
      assert(temp != NULL);
      assert(strcmp(temp->name, items[i]) == 0);
      temp = temp->next;
  }

  SUBTEST("room_char_length");
  assert(roomCharLength(room) == 3);
  removeChar(room, room->chara[1]);
  assert(roomCharLength(room) == 2);
  assert(room->chara[1] == NULL);
  assert(room->chara[0] != NULL);
  assert(room->chara[2] != NULL);

  SUBTEST("is_id_inside");
  assert(!isIdInside(room, "nope"));

  SUBTEST("move_char");
  struct Room * room2 = makeroom("kitchen");
  // assert(room->chara[0] != NULL);
  moveChar(room, room2, room->chara[0]);
  assert(!isCharInside(room, chars[0]));
  assert(isCharInside(room2, chars[0]));

  setNorth(room, room2);
  assert(room->North != NULL);
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
  assert(game.characters[avatar_idx] == game.avatar);
  
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
  
  // int player_position = 0;
  for (size_t i = 0; i < 9; i++) {
    assert(game.map[i] != NULL);
    assert(game.map[i]->chara != NULL);
    if (game.map[i]->visited) {
      assert(strcmp(game.map[i]->chara[0]->name, "test") == 0);
      // player_position = i;
    }
    assert(game.map[i]->itemList != NULL);
    assert(game.map[i]->chara[0] != NULL);
  }

  // from middle
  SUBTEST("movements");
  for(int i=North; i<=South; i++) {
    teleport(&game, 4); // to the middle
    assert(strcmp(game.avatar->location->name, game.map[4]->name) == 0);
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
}

static void test_gameplay_take_drop() {
  struct Game game = makeGame();
  initGame(&game, "test");
  assert(game.avatar->location->itemList != NULL);
  
  void test_take(const char * item_name) {
    assert(take(&game, item_name) == Ok);
    assert(take(&game, item_name) == NotFound);
  }
  void test_drop(const char * item_name) {
    assert(drop(&game, item_name) == Ok);
    assert(drop(&game, item_name) == NotFound);
  }
  const char * get_item_name() {
    return game.avatar->location->itemList->name;
  }
  const char * item_name = get_item_name();
  SUBTEST("take");
  test_take(item_name);
  assert(game.avatar->location->itemList == NULL);
  assert(strcmp(game.avatar->inventory->name, item_name) == 0);
  assert(take(&game, item_name) == NotFound);

  SUBTEST("drop");
  test_drop(item_name);
  assert(game.avatar->inventory == NULL);
  assert(strcmp(game.avatar->location->itemList->name, item_name) == 0);

  SUBTEST("take2");
  test_take(item_name);
  assert(strcmp(game.avatar->inventory->name, item_name) == 0);

  SUBTEST("multi");
  for(int i=North; i<=South; i++) {
    if (move(&game, i) != -1) {
      break;
    }
  }
  const char * item_name_2 = get_item_name();
  SUBTEST("m_take");
  test_take(item_name_2);

  SUBTEST("m_drop");
  test_drop(item_name_2);

  SUBTEST("checks");
  assert(game.avatar->inventoryItems == 1);
  assert(strcmp(game.avatar->inventory->name, item_name) == 0);
  assert(game.avatar->inventory->next == NULL);

  SUBTEST("invalid cases");
  assert(take(&game, "invalid item") == Invalid);
  assert(drop(&game, "invalid item") == Invalid);
}


static void test_gameplay_clue_chars() {
  struct Game game = makeGame();
  initGame(&game, "test");
  assert(game.attempts == 0);
  assert(clue(&game, "invalid") == Invalid);
  assert(game.attempts == 0);

  SUBTEST("clue_target_char");
  assert(clue(&game, game.targetChar) == Ok);
  assert(game.attempts == 1);
  assert(game.okChar == true);

  SUBTEST("clue_all_chars");
  int len = sizeof(game.characters) / sizeof(game.characters[0]);
  int corrects = 0;
  for (size_t i = 0; i < len; i++) {
    // since seed is deterministic, here we are testing clue against all characters
    struct Game game = makeGame();
    initGame(&game, "test");
    if (strcmp(game.characters[0]->name, "test") != 0) {
      assert(clue(&game, game.characters[i]->name) == Ok);
      assert(game.attempts == 1); 
      if (game.okChar == true) {
        corrects++;
        assert(strcmp(game.characters[i]->name, game.targetChar) == 0);
      }
    }
  }
  // only one is the correct
  assert(corrects == 1);
}

static void test_gameplay_clue_rooms() {
  struct Game game = makeGame();
  initGame(&game, "test");
  int len = sizeof(game.map) / sizeof(game.map[0]);
  int corrects = 0;
  game.attempts = len; // add more attemps for this test
  assert(strcmp(game.avatar->location->chara[0]->name, "test") == 0);
  for (size_t i = 0; i < len; i++) {
    // teleport player
    teleport(&game, i);
    clue(&game, "chang");
    if (game.okRoom == true) {
      corrects++;
      assert(strcmp(game.map[i]->name, game.targetRoom) == 0);
    }
  }
  assert(corrects == 1);
}

// TESTS
int main(void) {
  srand(20); // rand seed must be deterministic
  TEST(test_utils);
  TEST(test_pool_basic);
  TEST(test_pool_excluding);
  TEST(test_pool_excluding_caller);
  TEST(test_item);
  TEST(test_character);
  TEST(test_rooms);
  TEST(test_game);
  TEST(test_gameplay);
  TEST(test_gameplay_take_drop);
  TEST(test_gameplay_clue_chars);
  TEST(test_gameplay_clue_rooms);
  return 0;
}