#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "character.h"
#ifndef ROOMS_H
#define ROOMS_H
#define MAX_CHARACTER 3

//room structure
struct Room {
    const char* name;
    struct Room * North;
    struct Room * South;
    struct Room * East;
    struct Room * West;
    struct Item * itemList;
    struct Character* chara[MAX_CHARACTER];
    bool visited;
    bool isLocked;
};

//makeroom
struct Room* makeroom(const char* roomname);

//getters
const char* getRoomName(struct Room* room);
struct Room* getNorth(struct Room* room);
struct Room* getSouth(struct Room* room);
struct Room* getEast(struct Room* room);
struct Room* getWest(struct Room* room);

//setters
void setname(struct Room* room, const char* name);
void setNorth(struct Room *room,struct Room* newroom);
void setSouth(struct Room *room,struct Room* newroom);
void setEast(struct Room* room,struct Room* newroom);
void setWest(struct Room* room,struct Room* newroom);

//freeroom
void freeRoom(struct Room* room);

//checks
bool isIdInside(struct Room* room, const char * id);
bool isCharInside(struct Room* room, const char * name);
bool isItemInside(struct Room* room, const char * name);

//adding
void additem(struct Room* room,struct Item* item);
int addChar(struct Room * room, struct Character* character);

// move item in the room to the inventory
// 0 if item doen't exist
// 1 if ok
// -1 if inventory is full
int moveToInventory(struct Character* chara, const char * item);

// move item from inventory to the room
// 0 if item doen't exist
// 1 if ok
int moveItemToRoom(struct Character* chara, const char * item);

//-1 if is full
//-2 if is locked
// >= 0 if is ok
int moveChar(struct Room * from, struct Room * to, struct Character* character);

//removing
void removeChar(struct Room* room, struct Character* character);
void removeItem(struct Room* room, struct Item *item);

int unlockRoom(struct Room* room);
int roomCharLength(struct Room* room);

#endif