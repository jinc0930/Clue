#include "character.h"
#include "items.h"
#include "rooms.h"
#include<stdio.h>

//makeChar(name:char*): struct character *
//takes name as input, gives out a character as output
struct Character* makeChar(const char* name){
    struct Character * character = (struct Character*) malloc (sizeof(struct Character));
    character->name = name;
    character->location =NULL;
    character->inventory = NULL;
    character->inventoryItems = 0;
    character->id = NULL;
    character->coins_count = 0;
    // allocate on heap. remember to free!
    for (size_t i = 0; i < 3; i++) {
        character->hints[i] = malloc((128)*sizeof(char));
        character->prefix[i] = malloc((64)*sizeof(char));
    }
    return character;
}

//freeChar(charater:Character*):void
//frees the allocated space of a character
void freechar(struct Character* character){
    for (size_t i = 0; i < 3; i++) {
        free(character->hints[i]);
        free(character->prefix[i]);
    }
    free(character);
}

//getcharname(character:Character *):char*
//takes chracter as input and outputs the name of the given character
const char* getcharname(struct Character* character){
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

//updates amount of coins a character has
int updatecoins(struct Character* character, int purchase){
    if((character->coins_count += purchase)<0){
        return 1;
    }
    else{
        character->coins_count += purchase;
        return 0;
    }
}

//
int transaction(struct Character* sender, struct Character* receiver, int amount) {
    // ============
    // FIXME TODO
    // ============
    return 0;
}

//add(character:Character*,item:Item*):int
//takes character and item as input, add the item to the inventory of the character; returns 1 if item is added and 0 if inventory is full
int add(struct Character* character, struct Item*item) {
    if (character->inventoryItems >= 3) return 0;
    // somenthing
    if (character->inventory == NULL) {
        character->inventory = item;
        character->inventoryItems++;
        return 1;
    }
    struct Item *temp = character->inventory;
    item->next = temp;
    character->inventory = item;
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
        struct Item * prev = NULL;
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
        strcpy(character->prefix[0], "I'm not sure, but I think the murder weapon is");
        if (strcmp(itemHint->name, "butter knife") == 0) {
           strcpy(character->hints[0], "butter knife because it had blood on it");
        } 
        else if (strcmp(itemHint->name, "bat") == 0) {
           strcpy(character->hints[0], "bat, because I couldn't find it yesterday");
        }
        else if (strcmp(itemHint->name, "wrench") == 0) {
            strcpy(character->hints[0], "wrench because it was missing for a couple of days");
        }
        else if (strcmp(itemHint->name, "rope") == 0) {
            strcpy(character->hints[0], "rope because it has scratch marks on it");
        }
        else if (strcmp(itemHint->name, "dagger") == 0) {
            strcpy(character->hints[0], "dagger because it is bloody");
        }
        else if (strcmp(itemHint->name, "rifle") == 0) {
            strcpy(character->hints[0], "rifle because three bullets went missing");
        }
        else if (strcmp(itemHint->name, "hammer") == 0) {
            strcpy(character->hints[0], "hammer because it's sticky");
        }
        else if (strcmp(itemHint->name, "lead pipe") == 0) {
            strcpy(character->hints[0], "lead pipe because it looks like someone ripped it off the building");
        }
        else if (strcmp(itemHint->name, "poison bottle") == 0) {
            strcpy(character->hints[0], "poison bottle because the floor was corroded");
        }
    }
    else{
        // character->hints[0] = (itemHint->name);
        strcpy(character->prefix[0], "The murder weapon is not");
        if (strcmp(itemHint->name, "butter knife") == 0) {
           strcpy(character->hints[0], "butter knife because it had butter on it");
        } 
        else if (strcmp(itemHint->name, "bat") == 0) {
           strcpy(character->hints[0], "bat, because I took the only one to play baseball with some friends");
        }
        else if (strcmp(itemHint->name, "wrench") == 0) {
            strcpy(character->hints[0], "wrench because Michael took it to fix the car");
        }
        else if (strcmp(itemHint->name, "rope") == 0) {
            strcpy(character->hints[0], "rope because I borrowed the only one to my friend Kayla, who loves rock climbing");
        }
        else if (strcmp(itemHint->name, "dagger") == 0) {
            strcpy(character->hints[0], "dagger because it was stored and locked in the safe");
        }
        else if (strcmp(itemHint->name, "rifle") == 0) {
            strcpy(character->hints[0], "rifle because it was stored and locked in the safe");
        }
        else if (strcmp(itemHint->name, "hammer") == 0) {
            strcpy(character->hints[0], "hammer because I was using it to fix my cabinet");
        }
        else if (strcmp(itemHint->name, "lead pipe") == 0) {
            strcpy(character->hints[0], "lead pipe because we shipped it to a fixing company");
        }
        else if (strcmp(itemHint->name, "poison bottle") == 0) {
            strcpy(character->hints[0], "poison bottle because it was locked in the lab");
        }
    }
}

void set_room_hint(struct Character* character, struct Room* roomHint){
    if((strcmp(character -> id,"accuser")==0)||(strcmp(character -> id,"murderer")==0)){
        strcpy(character->prefix[1], "I'm not sure, but I think the murder happened in the");
        if (strcmp(roomHint->name, "kitchen") == 0) {
           strcpy(character->hints[1], "kitchen because I heard weird noises last night");
        } 
        else if (strcmp(roomHint->name, "hall") == 0) {
           strcpy(character->hints[1], "hall because the sensor lights were triggered");
        }
        else if (strcmp(roomHint->name, "study") == 0) {
            strcpy(character->hints[1], "study because the lights was on");
        }
        else if (strcmp(roomHint->name, "bathroom") == 0) {
            strcpy(character->hints[1], "bathroom because someone was using it for a couple of hours straight");
        }
        else if (strcmp(roomHint->name, "ballroom") == 0) {
            strcpy(character->hints[1], "ballroom because creepy music was blasting for the whole night");
        }
        else if (strcmp(roomHint->name, "library") == 0) {
            strcpy(character->hints[1], "library because the shelves were flipped on the floor");
        }
        else if (strcmp(roomHint->name, "lounge") == 0) {
            strcpy(character->hints[1], "lounge because the sofa had blood stains");
        }
        else if (strcmp(roomHint->name, "office") == 0) {
            strcpy(character->hints[1], "office because noone ever goes there");
        }
        else if (strcmp(roomHint->name, "bedroom") == 0) {
            strcpy(character->hints[1], "bedroom because the pillow is missing");
        }
    }
    else{
        // character->hints[1] = (getRoomName(roomHint));
        strcpy(character->prefix[1], "The murder did not happen in the");
        if (strcmp(roomHint->name, "kitchen") == 0) {
           strcpy(character->hints[1], "kitchen because I was making a midnight snack there");
        } 
        else if (strcmp(roomHint->name, "hall") == 0) {
           strcpy(character->hints[1], "hall because the hall has sensor lights that would have been triggered");
        }
        else if (strcmp(roomHint->name, "study") == 0) {
            strcpy(character->hints[1], "study because I was writting letters in the study");
        }
        else if (strcmp(roomHint->name, "bathroom") == 0) {
            strcpy(character->hints[1], "bathroom because I had to use it for a couple of hours");
        }
        else if (strcmp(roomHint->name, "ballroom") == 0) {
            strcpy(character->hints[1], "ballroom because there was a party hosted there");
        }
        else if (strcmp(roomHint->name, "library") == 0) {
            strcpy(character->hints[1], "library because it is locked and closed after 4pm");
        }
        else if (strcmp(roomHint->name, "lounge") == 0) {
            strcpy(character->hints[1], "lounge because a couple of people were watching TV in the lounge");
        }
        else if (strcmp(roomHint->name, "office") == 0) {
            strcpy(character->hints[1], "office because I took a nap in there");
        }
        else if (strcmp(roomHint->name, "bedroom") == 0) {
            strcpy(character->hints[1], "bedroom because Ivan was sleeping there");
        }
    }
}

// //working on this
void set_char_hint(struct Character* character, struct Character* accused){
    if((strcmp(character -> id,"accuser")==0)||(strcmp(character -> id,"murderer")==0)){
        strcpy(character->prefix[2], ("I'm not sure, but I think the murderer is"));
        if (strcmp(accused->name, "chang") == 0) {
           strcpy(character->hints[2], "Chang because she is always passive-aggressive");
        } 
        else if (strcmp(accused ->name, "kevin") == 0) {
           strcpy(character->hints[2], "Kevin because he was acting suspicious");
        }
        else if (strcmp(accused ->name, "james") == 0) {
            strcpy(character->hints[2], "James because he used the bathroom for a couple of hours");
        }
        else if (strcmp(accused ->name, "tenzin") == 0) {
            strcpy(character->hints[2], "Tenzin because he loves to play violent videogames.");
        }
        else if (strcmp(accused ->name, "minh") == 0) {
            strcpy(character->hints[2], "Minh because he is so fake, he acts nice but bad mouths you behind your back");
        }
        else if (strcmp(accused ->name, "michael") == 0) {
            strcpy(character->hints[2], "Michael because he and the victim's are enemies");
        }
        else if (strcmp(accused ->name, "ivan") == 0) {
            strcpy(character->hints[2], "Ivan because he was off for the whole day and stayed in");
        }
        else if (strcmp(accused ->name, "edrick") == 0) {
            strcpy(character->hints[2], "Edrick because he was acting weird and didn't study that night");
        }
        else if (strcmp(accused ->name, "joey") == 0) {
            strcpy(character->hints[2], "Joey because he did not go work out at the gym that day");
        }
    }
    else{
        strcpy(character->prefix[2], "I don't know who the muderer is, but I know it is not");
        if (strcmp(accused->name, "chang") == 0) {
           strcpy(character->hints[2], "Chang because she was with me when the murder happened");
        } 
        else if (strcmp(accused ->name, "kevin") == 0) {
           strcpy(character->hints[2], "Kevin because he is always coding");
        }
        else if (strcmp(accused ->name, "james") == 0) {
            strcpy(character->hints[2], "James because he is the nicest man I have ever seen");
        }
        else if (strcmp(accused ->name, "tenzin") == 0) {
            strcpy(character->hints[2], "Tenzin because he was playing videogames the time of the murder.");
        }
        else if (strcmp(accused ->name, "minh") == 0) {
            strcpy(character->hints[2], "Minh because he is such a nice person");
        }
        else if (strcmp(accused ->name, "michael") == 0) {
            strcpy(character->hints[2], "Michael because he is the victim's bestfriend");
        }
        else if (strcmp(accused ->name, "ivan") == 0) {
            strcpy(character->hints[2], "Ivan because he was working the whole day");
        }
        else if (strcmp(accused ->name, "edrick") == 0) {
            strcpy(character->hints[2], "Edrick because he was busy studying");
        }
        else if (strcmp(accused ->name, "joey") == 0) {
            strcpy(character->hints[2], "Joey because he was working out at the gym");
        }
    }
}