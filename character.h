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
    int inventoryItems;
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

#endif