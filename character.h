#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#ifndef CHARACTER_H
#define CHARACTER_H
#define MAX_INVENTORY 3

//character structure
struct Character{
    const char* name;
    struct Item * inventory;
    struct Room * location;
    const char* id;
    int inventoryItems;
    char* hints[3];
    char* prefix[3];
    int coins_count;
};

//make character
struct Character* makeChar(const char* name);

//free character
void freechar(struct Character* character);

//getters
const char* getcharname(struct Character* character);
struct Room* getloc (struct Character * character);

//setters
void setloc(struct Character* character, struct Room* room);

//adding
int add(struct Character* character, struct Item*item);
//removing
void rmv(struct Character* character, struct Item*item);
//destroying and freeing
// 0 = fail, 1 = ok
int destroy(struct Character* character, const char * item);
//transferItem to another character
// 0 = fail, 1 = ok
int transferItem(struct Character* from, struct Character* to, const char * item);
bool constainsItem(struct Character* character, const char * item);
//updates new coin amount, purchase will be the adjustment of coins (pass in the correct signs),
//an integer is returned, 0 = purchase succesful and complete, 1= purchase unsucessful
int updatecoins(struct Character* character, int purchase);

// transaction between 2 characters
// returns 0 when it fails reamining unchanges
// returns 1 when it succeeds
int transaction(struct Character* sender, struct Character* receiver, int amount);

//setting hints the npcs will give out
void set_item_hint(struct Character* character, struct Item* itemHint);
void set_room_hint(struct Character* character, struct Room* roomHint);
void set_char_hint(struct Character* character, struct Character* accused);
#endif