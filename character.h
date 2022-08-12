#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef CHARACTER_H
#define CHARACTER_H

//character structure
struct Character{
    char* name;
    struct Item * inventory;
    struct Room * location;
    char* id;
    int inventoryItems;
    char* hints[3];
    char* prefix[3];
};

//make character
struct Character* makeChar(char* name);

//free character
void freechar(struct Character* character);

//getters
char* getcharname(struct Character* character);
struct Room* getloc (struct Character * character);

//setters
void setloc(struct Character* character, struct Room* room);

//adding
int add(struct Character* character, struct Item*item);
//removing
void rmv(struct Character* character, struct Item*item);



void set_item_hint(struct Character* character, struct Item* itemHint);
void set_room_hint(struct Character* character, struct Room* roomHint);
void set_char_hint(struct Character* character, struct Character* accused);
#endif