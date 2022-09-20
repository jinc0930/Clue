#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#ifndef ITEMS_H
#define ITEMS_H

//item structure
struct Item{
    const char* name;
    struct Item* next;
    bool isSpecial;
};

//initiator
struct Item * makeitem(const char* name);

//initiator n2
struct Item * makeSpecialItem(const char* name);

//getter
const char* getItemName(struct Item* item);
struct Item * getNext(struct Item* item);

//setter
void setNext(struct Item * item, struct Item * newitem);

#endif