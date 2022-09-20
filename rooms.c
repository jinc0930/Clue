#include "rooms.h"
#include "items.h"
#include "character.h"
#include <stdbool.h>
#include <string.h>

//makeroom(roomname:char*):Room*
//initializes room, takes a roomname as input and outputs a room
struct Room* makeroom(const char* roomname){
  struct Room * room = (struct Room*) malloc (sizeof(struct Room));
  room -> name = roomname;
  room -> North = NULL;
  room -> South = NULL;
  room -> East = NULL;
  room -> West = NULL;
  room -> chara[0] = NULL;
  room -> chara[1] = NULL;
  room -> chara[2] = NULL;
  room -> itemList = NULL;
  room -> visited = false;
  room -> isLocked = false;
  return room;
}

//getters
//getRoomName(room:Room*):char*
//gets the name of the room, takes room as input and return the name as output
const char* getRoomName(struct Room* room){
  return room->name;
}

//getNorth(room:Room*):Room*
//gets the room north of given room, takes room as input and returns room as output
struct Room* getNorth(struct Room* room){
  return room->North;
}
//getSouth(room:Room*):Room*
//gets the room south of given room, takes room as input and returns room as output
struct Room* getSouth(struct Room* room){
  return room->South;
}

//getEast(room:Room*):Room*
//gets the room east of given room, takes room as input and returns room as output
struct Room* getEast(struct Room* room){
  return room->East;
}

//getWest(room:Room*):Room*
//gets the room west of given room, takes room as input and returns room as output
struct Room* getWest(struct Room* room){
  return room->West;
}

//setters
//setName(room:Room*,name:char*):void
//takes room and name as input, sets the given name as the room's name
void setname(struct Room* room, const char* name){
  room->name = name;
}

//setNorth(room:Room*,newRoom:Room*):void
//takes room and target room as input, set the north pointer of room to target room
void setNorth(struct Room* room, struct Room* newRoom){
  room->North = newRoom;
}

//setSouth(room:Room*,newRoom:Room*):void
//takes room and target room as input, set the south pointer of room to target room
void setSouth(struct Room* room, struct Room* newRoom){
  room->South = newRoom;
}

//setEast(room:Room*,newRoom:Room*):void
//takes room and target room as input, set the east pointer of room to target room
void setEast(struct Room* room, struct Room* newRoom){
  room -> East = newRoom;
}

//setWest(room:Room*,newRoom:Room*):void
//takes room and target room as input, set the west pointer of room to target room
void setWest(struct Room* room, struct Room* newRoom){
  room -> West = newRoom;
}

//freeRoom(room:Room*):void
//takes room as input, frees the allocated memory spcae for given room
void freeRoom(struct Room* room){
  free(room);
}

//additem(room:Room*,item:Item*):void
//takes room and item as input, adds given item to given room's item list
void additem(struct Room* room, struct Item* item){
  //if list is empty
  if(room -> itemList == NULL){
    room->itemList = item;
    item->next = NULL;
  }
  //if list is not empty
  else{
    struct Item *i =room->itemList;
    setNext(item, i);
    room->itemList = item;
  }
}

//removeItem(room:Room*,item:Item*):void
//takes room and item as input, removes give item from the itemlist of the given room
//if item is not in the item list of the room, do nothing
void removeItem(struct Room* room, struct Item* item){
  if(room -> itemList == NULL){
   //do nothing
  }
  else if(strcmp(room->itemList->name, item->name) == 0){
    if (room->itemList->next == NULL) {
      room->itemList = NULL;
    } else {
      room->itemList = room->itemList->next;
    }
  }
  //not first item, and there are multiple items
  else{
    //start from second item
    struct Item * temp = room->itemList;
    struct Item * prev = NULL;
    while(temp -> next != NULL && strcmp(temp -> name, item->name) != 0){ 
      //iterate until find the place that holds the item
      prev = temp;
      temp = temp -> next;
    }
    if(temp != NULL && strcmp(temp -> name, item->name) == 0){
      //points the prev item to the one after current item
      prev -> next = temp -> next; 
    }
  }
}

//addChar(room:Room*, character:Character*):int
//takes in room, character, adds given character to the given room
//return -1 if there's no available slots in the room otherwise return the index slot
int addChar(struct Room * room, struct Character* character){
  for (size_t i = 0; i < MAX_CHARACTER; i++){
    struct Character *c = room->chara[i];
     if (c == NULL) {
      room->chara[i] = character;
      character->location = room;
      return i;
    }
  }
  // no slots available
  return -1;
}

//move character from one slot to another room
//return -1 if there's no available slots in the room otherwise return the index slot
//return -2 if is locked
//return the index if ok
int moveChar(struct Room * from, struct Room * to, struct Character* character){
  if (to->isLocked) return -2;
  int slot = addChar(to, character);
  if (slot > -1) {
    for (size_t i = 0; i < MAX_CHARACTER; i++){
      struct Character *c = from->chara[i];
      if(c != NULL && c->name == character->name){
        from->chara[i] = NULL;
        character->location = to;
        break;
      }
    }
    return slot;
  }
  // no slots available
  return -1;
}

//removeChar(room:Room*,character:Character*):void
//takes in room, character and integer num, removes given character to the given room
void removeChar(struct Room* room, struct Character* character) {
  for (size_t i = 0; i < MAX_CHARACTER; i++){
    struct Character *c = room->chara[i];
    if(c != NULL && c->name == character->name){
      room->chara[i] = NULL;
      character->location = NULL;
      // for(int j=i; j<MAX_CHARACTER; j++){
      //   room->chara[j]=room->chara[j+1];
      // }
      break;
    }
  }
}

// how many chars in the room
// no optimization since room size is 3
int roomCharLength(struct Room* room) {
  int len = 0;
  for (size_t i = 0; i < MAX_CHARACTER; i++){
    struct Character *c = room->chara[i];
     if (c != NULL) len += 1;
  }
  return len;
}

bool isIdInside(struct Room* room, const char * id) {
  bool x = false;
  for (size_t i = 0; i < MAX_CHARACTER; i++){
    struct Character *c = room->chara[i];
     if (c != NULL && c->id != NULL && strcmp(c->id, id) == 0) x = true;
  }
  return x;
}

bool isCharInside(struct Room* room, const char * name) {
  bool x = false;
  for (size_t i = 0; i < MAX_CHARACTER; i++){
    struct Character *c = room->chara[i];
    if (c != NULL && c->name != NULL && strcmp(c->name, name) == 0) x = true;
  }
  return x;
}

bool isItemInside(struct Room* room, const char * name) {
  struct Item * roominv = room->itemList;
  while(roominv!=NULL){
      if(strcmp(roominv->name, name)==0){
        return true;
      }
      roominv = roominv->next;
  }
  return false;
}

int unlockRoom(struct Room* room) {
  // TODO: maybe add more stuff
  room->isLocked = false;
  return 1;
}