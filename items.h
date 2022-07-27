#include <stdio.h>
#include <stdlib.h>
#ifndef ITEMS_H
#define ITEMS_H

//item structure
struct Item{
    char* name;
    struct Item* next;
};

//initiator
struct Item * makeitem(char* name);

//getter
char*  getN(struct Item* item);
struct Item * getNext(struct Item* item);

//setter
void setNext(struct Item * item, struct Item * newitem);

#endif