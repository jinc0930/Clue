#include <stdio.h>
#include <stdlib.h>
#ifndef ROOMS_H
#define ROOMS_H
#define MAX_CHARACTER 6

//room structure
struct Room {
    char* name;
    struct Room * North;
    struct Room * South;
    struct Room * East;
    struct Room * West;
    struct Item * itemList;
    int num;
    struct Character* chara[MAX_CHARACTER];
};

//makeroom
struct Room* makeroom(char* roomname);

//getters
char* getRoomName(struct Room* room);
struct Room* getNorth(struct Room* room);
struct Room* getSouth(struct Room* room);
struct Room* getEast(struct Room* room);
struct Room* getWest(struct Room* room);

//setters
void setname(struct Room* room, char* name);
void setNorth(struct Room *room,struct Room* newroom);
void setSouth(struct Room *room,struct Room* newroom);
void setEast(struct Room* room,struct Room* newroom);
void setWest(struct Room* room,struct Room* newroom);

//freeroom
void freeRoom(struct Room* room);

//adding
void additem(struct Room* room,struct Item* item);
void addChar(struct Room * room, struct Character* character, int num);

//removing
void removeChar(struct Room* room, struct Character* character, int num);
void removeItem(struct Room* room, struct Item *item);

#endif