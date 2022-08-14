#include "character.h"
#include "items.h"
#include "rooms.h"
#include<stdio.h>

//makeChar(name:char*): struct character *
//takes name as input, gives out a character as output
struct Character* makeChar(char* name){
    struct Character * character = (struct Character*) malloc (sizeof(struct Character));
    character->name = name;
    character->location =NULL;
    character->inventory = NULL;
    character->inventoryItems = 0;
    character->id = NULL;
    character->hints[3];
    character->prefix[3];
    return character;
}

//freeChar(charater:Character*):void
//frees the allocated space of a character
void freechar(struct Character* character){
    free(character);
}

//getcharname(character:Character *):char*
//takes chracter as input and outputs the name of the given character
char* getcharname(struct Character* character){
    return character -> name;
}

//getloc(character:Character *):Room*
//gets location of a character, takes character as input, returns a room the character is located at
struct Room* getloc (struct Character * character){
    return character -> location;
}

//setloc(character:Character*, room:Room*):void
//takes character and room as input, sets the location of the character to the room given
void setloc(struct Character* character, struct Room* room){
    character -> location = room;
}

//add(character:Character*,item:Item*):int
//takes character and item as input, add the item to the inventory of the character; returns 1 if item is added and 0 if inventory is full
int add(struct Character* character, struct Item*item) {
    if (character->inventoryItems >= 3) return 0;
    // somenthing
    if (character->inventory == NULL) {
        character->inventory = item;
    } else {
        struct Item *temp = character->inventory;
        for(;;) {
            if (temp->next == NULL) {
                temp->next = item;
                break;
            }
            temp = temp->next;
        }
    }
    character->inventoryItems++;
    return 1;
}

//rmv(character:Character*,item:Item*):void
//takes character and item as input, removes the item from the inventory of the chracter
void rmv(struct Character* character, struct Item*item){
    if(character->inventory == NULL){
        //do nothing
    }
    //if there is only one item in the inventory
    else if((character->inventory ==item) && (character->inventory->next == NULL)){
        character->inventory = NULL;
        character->inventoryItems = 0;
    } 
    //if there is more than one and the target item is the first item on the inventory list
    else if((character->inventory ==item) && (character->inventory->next != NULL)){
        character->inventory = character->inventory->next;
        character->inventoryItems--;
    }
    //not first item, and there are multiple items
    else{
    //start from second item
        struct Item * temp = character->inventory;
        struct Item * prev;
        while((temp->next != NULL) && (strcmp(temp->name, item->name) != 0)) { 
            //iterate until find the place that holds the item
            prev = temp;
            temp = temp->next;
        }
        if(temp != NULL && strcmp(temp->name, item->name) == 0) {
            //points the prev item to the next item (after current)
            prev->next = temp->next; 
        }
        character->inventoryItems--;
    }
}

//takes character and item, gives the character that hint
void set_item_hint(struct Character* character, struct Item* itemHint){
    
    
    if((strcmp(character -> id,"accuser")==0)||(strcmp(character -> id,"murderer")==0)){
        character->hints[0] = (itemHint->name);
        character->prefix[0] = ("I'm not sure, but I think the murderer weapon is");
    }
    else{
        character->hints[0] = (itemHint->name);
        character->prefix[0] = ("The murderer weapon is not");
    }
}

void set_room_hint(struct Character* character, struct Room* roomHint){
    if((strcmp(character -> id,"accuser")==0)||(strcmp(character -> id,"murderer")==0)){
        character->hints[1] = (getRoomName(roomHint));
        character->prefix[1] = ("I'm not sure, but I think the murderer happened in the");
    }
    else{
        character->hints[1] = (getRoomName(roomHint));
        character->prefix[1] = ("The murderer did not happen in the");
    }
}

//old way
// void set_char_hint(struct Character* character, struct Character* accused){
//     if((strcmp(character -> id,"accuser")==0)||(strcmp(character -> id,"murderer")==0)){
//         character->hints[2] = (accused->name);
//         character->prefix[2] = ("I'm not sure, but I think the murderer is");
//     }
//     else{
//         character->hints[2] = (accused->name);
//         character->prefix[2] =("The murderer is not");
//     }
// }

// //working on this
void set_char_hint(struct Character* character, struct Character* accused){
    if((strcmp(character -> id,"accuser")==0)||(strcmp(character -> id,"murderer")==0)){
        character->hints[2] = (accused->name);
        character->prefix[2] = ("I'm not sure, but I think the murderer is");
    }
    else{
        //standard
        // character->hints[2] = (accused->name);
        // character->prefix[2] =("The murderer is not");
        character->prefix[2] = "I don't know who the muderer is, but I know it is not";
        if (strcmp(accused->name, "chang") == 0) {
           character->hints[2] = ("Chang because she was with me when the murder happened");
        } 
        else if (strcmp(accused ->name, "kevin") == 0) {
           character->hints[2] = ("Kevin because he is always coding");
        }
        else if (strcmp(accused ->name, "james") == 0) {
            character->hints[2] = ("James because he is the nicest man I have ever seen");
        }
        else if (strcmp(accused ->name, "tenzin") == 0) {
            character->hints[2] = ("Tenzin because he was playing videogames the time of the murder.");
        }
        else if (strcmp(accused ->name, "minh") == 0) {
            character->hints[2] = ("Minh because he is such a nice person");
        }
        else if (strcmp(accused ->name, "michael") == 0) {
            character->hints[2] = ("Michael because he is the victim's bestfriend");
        }
        else if (strcmp(accused ->name, "ivan") == 0) {
            character->hints[2] = ("Ivan because he was working the whole day");
        }
        else if (strcmp(accused ->name, "edrick") == 0) {
            character->hints[2] = ("Edrick because he was busy studying");
        }
        else if (strcmp(accused ->name, "joey") == 0) {
            character->hints[2] = ("Joey because he was working out at the gym");
        }
    }
}