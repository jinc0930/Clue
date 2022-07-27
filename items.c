#include "items.h"

//makeitem(name:char*):Item*
//takes an item and a "string" as input, set given string as given item's name
struct Item * makeitem(char*name){
    struct Item * item = (struct Item*) malloc (sizeof(struct Item));
    item -> name = name;
    item -> next = NULL;
    return item;
}

//getter
//getNext(item:Item*):Item*
//takes item as input, get and returns the next item of given item
struct Item * getNext(struct Item* item){
    return item -> next;
}
//getN(item:Item*):char*
//takes item as input, gets the name of given item
char*  getN(struct Item* item){
    return item->name;
}

//setter
//setNext(item:Item*,newitem:Item*):void
//takes in item and a newitem, sets item's next pointer to the given new item.
void setNext(struct Item * item, struct Item * newitem){
    item -> next = newitem;
}