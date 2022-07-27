#include "rooms.h"
#include "items.h"
#include <stdbool.h>

//makeroom(roomname:char*):Room*
//initializes room, takes a roomname as input and outputs a room
struct Room* makeroom(char* roomname){
  struct Room * room = (struct Room*) malloc (sizeof(struct Room));
  room -> name = roomname;
  room -> North = NULL;
  room -> South = NULL;
  room -> East = NULL;
  room -> West = NULL;
  room -> chara[0] = NULL;
  room -> chara[1] = NULL;
  room -> chara[2] = NULL;
  room -> chara[3] = NULL;
  room -> chara[4] = NULL;
  room -> chara[5] = NULL;
  room -> itemList = NULL;
  room -> num = 0;
  return room;
}

//getters
//getname(room:Room*):char*
//gets the name of the room, takes room as input and return the name as output
char* getname(struct Room* room){
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
void setname(struct Room* room, char* name){
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
    room -> itemList = item;
    item->next =NULL;
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
  else if((room->itemList==item) && (room->itemList->next == NULL)){
    room->itemList = NULL;
  }
  //not first item, and there are multiple items
  else{
    //start from second item
    struct Item * temp = room->itemList;
    struct Item * prev;
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

//addChar(room:Room*, character:Character*,num:int):void
//takes in room, character and integer num, adds given character to the given room
//num is used to keep track of total number of characters in the room
void addChar(struct Room * room, struct Character* character,int num){
  bool in = false;
  for (int n =0;n<6;n++){
    if(room -> chara[n] = character){
      in =true;
    }
  }
  if(in = false){
    room -> chara[num] = character;
    num++;
  }
}

//removeChar(room:Room*,character:Character*,num:int):void
//takes in room, character and integer num, removes given character to the given room
//num is used to keep track of total number of characters in the room
void removeChar(struct Room* room, struct Character* character, int num){
  for(int i=0;i<num;++i){
    if(room->chara[i] == character){
      room->chara[i]=NULL;
      num--;
      for(int j=i;j<5;j++){
        room->chara[j]=room->chara[j+1];
      }
    }
  }
}