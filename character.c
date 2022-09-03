#include "character.h"
#include "items.h"
#include "rooms.h"
#include "prettyprint.h"
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
        strcpy(character->prefix[0], "I'm not sure, but I think the murder weapon is");
        if (strcmp(itemHint->name, "butter knife") == 0) {
           strcpy(character->hints[0], RED"butter knife"RESET" because it had blood on it");
        } 
        else if (strcmp(itemHint->name, "bat") == 0) {
           strcpy(character->hints[0], RED"bat,"RESET" because I couldn't find it yesterday");
        }
        else if (strcmp(itemHint->name, "wrench") == 0) {
            strcpy(character->hints[0], RED"wrench"RESET" because it was missing for a couple of days");
        }
        else if (strcmp(itemHint->name, "rope") == 0) {
            strcpy(character->hints[0], RED"rope"RESET" because it has scratch marks on it");
        }
        else if (strcmp(itemHint->name, "dagger") == 0) {
            strcpy(character->hints[0], RED"dagger"RESET" because it is bloody");
        }
        else if (strcmp(itemHint->name, "rifle") == 0) {
            strcpy(character->hints[0], RED"rifle"RESET" because three bullets went missing");
        }
        else if (strcmp(itemHint->name, "hammer") == 0) {
            strcpy(character->hints[0], RED"hammer"RESET" because it's sticky");
        }
        else if (strcmp(itemHint->name, "lead pipe") == 0) {
            strcpy(character->hints[0], RED"lead pipe"RESET" because it looks like someone ripped it off the building");
        }
        else if (strcmp(itemHint->name, "poison bottle") == 0) {
            strcpy(character->hints[0], RED"poison bottle"RESET" because the floor was corroded");
        }
    }
    else{
        // character->hints[0] = (itemHint->name);
        strcpy(character->prefix[0], "The murder weapon is not");
        if (strcmp(itemHint->name, "butter knife") == 0) {
           strcpy(character->hints[0], BLU"butter knife"RESET" because it had butter on it");
        } 
        else if (strcmp(itemHint->name, "bat") == 0) {
           strcpy(character->hints[0], BLU"bat,"RESET" because I took the only one to play baseball with some friends");
        }
        else if (strcmp(itemHint->name, "wrench") == 0) {
            strcpy(character->hints[0], BLU"wrench"RESET" because Michael took it to fix the car");
        }
        else if (strcmp(itemHint->name, "rope") == 0) {
            strcpy(character->hints[0], BLU"rope"RESET" because I borrowed the only one to my friend Kayla, who loves rock climbing");
        }
        else if (strcmp(itemHint->name, "dagger") == 0) {
            strcpy(character->hints[0], BLU"dagger"RESET" because it was stored and locked in the safe");
        }
        else if (strcmp(itemHint->name, "rifle") == 0) {
            strcpy(character->hints[0], BLU"rifle"RESET" because it was stored and locked in the safe");
        }
        else if (strcmp(itemHint->name, "hammer") == 0) {
            strcpy(character->hints[0], BLU"hammer"RESET" because I was using it to fix my cabinet");
        }
        else if (strcmp(itemHint->name, "lead pipe") == 0) {
            strcpy(character->hints[0], BLU"lead pipe"RESET" because we shipped it to a fixing company");
        }
        else if (strcmp(itemHint->name, "poison bottle") == 0) {
            strcpy(character->hints[0], BLU"poison bottle"RESET" because it was locked in the lab");
        }
    }
}

void set_room_hint(struct Character* character, struct Room* roomHint){
    if((strcmp(character -> id,"accuser")==0)||(strcmp(character -> id,"murderer")==0)){
        strcpy(character->prefix[1], "I'm not sure, but I think the murder happened in the");
        if (strcmp(roomHint->name, "kitchen") == 0) {
           strcpy(character->hints[1], RED"kitchen"RESET" because I heard weird noises last night");
        } 
        else if (strcmp(roomHint->name, "hall") == 0) {
           strcpy(character->hints[1], RED"hall"RESET" because the sensor lights were triggered");
        }
        else if (strcmp(roomHint->name, "study") == 0) {
            strcpy(character->hints[1], RED"study"RESET" because the lights was on");
        }
        else if (strcmp(roomHint->name, "bathroom") == 0) {
            strcpy(character->hints[1], RED"bathroom"RESET" because someone was using it for a couple of hours straight");
        }
        else if (strcmp(roomHint->name, "ballroom") == 0) {
            strcpy(character->hints[1], RED"ballroom"RESET" because creepy music was blasting for the whole night");
        }
        else if (strcmp(roomHint->name, "library") == 0) {
            strcpy(character->hints[1], RED"library"RESET" because the shelves were flipped on the floor");
        }
        else if (strcmp(roomHint->name, "lounge") == 0) {
            strcpy(character->hints[1], RED"lounge"RESET" because the sofa had blood stains");
        }
        else if (strcmp(roomHint->name, "office") == 0) {
            strcpy(character->hints[1], RED"office"RESET" because noone ever goes there");
        }
        else if (strcmp(roomHint->name, "bedroom") == 0) {
            strcpy(character->hints[1], RED"bedroom"RESET" because the pillow is missing");
        }
    }
    else{
        // character->hints[1] = (getRoomName(roomHint));
        strcpy(character->prefix[1], "The murder did not happen in the");
        if (strcmp(roomHint->name, "kitchen") == 0) {
           strcpy(character->hints[1], BLU"kitchen"RESET" because I was making a midnight snack there");
        } 
        else if (strcmp(roomHint->name, "hall") == 0) {
           strcpy(character->hints[1], BLU"hall"RESET" because the hall has sensor lights that would have been triggered");
        }
        else if (strcmp(roomHint->name, "study") == 0) {
            strcpy(character->hints[1], BLU"study"RESET" because I was writting letters in the study");
        }
        else if (strcmp(roomHint->name, "bathroom") == 0) {
            strcpy(character->hints[1], BLU"bathroom"RESET" because I had to use it for a couple of hours");
        }
        else if (strcmp(roomHint->name, "ballroom") == 0) {
            strcpy(character->hints[1], BLU"ballroom"RESET" because there was a party hosted there");
        }
        else if (strcmp(roomHint->name, "library") == 0) {
            strcpy(character->hints[1], BLU"library"RESET" because it is locked and closed after 4pm");
        }
        else if (strcmp(roomHint->name, "lounge") == 0) {
            strcpy(character->hints[1], BLU"lounge"RESET" because a couple of people were watching TV in the lounge");
        }
        else if (strcmp(roomHint->name, "office") == 0) {
            strcpy(character->hints[1], BLU"office"RESET" because I took a nap in there");
        }
        else if (strcmp(roomHint->name, "bedroom") == 0) {
            strcpy(character->hints[1], BLU"bedroom"RESET" because Ivan was sleeping there");
        }
    }
}

// //working on this
void set_char_hint(struct Character* character, struct Character* accused){
    if((strcmp(character -> id,"accuser")==0)||(strcmp(character -> id,"murderer")==0)){
        strcpy(character->prefix[2], ("I'm not sure, but I think the murderer is"));
        if (strcmp(accused->name, "chang") == 0) {
           strcpy(character->hints[2], RED"Chang"RESET" because she is always passive-aggressive");
        } 
        else if (strcmp(accused ->name, "kevin") == 0) {
           strcpy(character->hints[2], RED"Kevin"RESET" because he was acting suspicious");
        }
        else if (strcmp(accused ->name, "james") == 0) {
            strcpy(character->hints[2], RED"James"RESET" because he used the bathroom for a couple of hours");
        }
        else if (strcmp(accused ->name, "tenzin") == 0) {
            strcpy(character->hints[2], RED"Tenzin"RESET" because he loves to play violent videogames.");
        }
        else if (strcmp(accused ->name, "minh") == 0) {
            strcpy(character->hints[2], RED"Minh"RESET" because he is so fake, he acts nice but bad mouths you behind your back");
        }
        else if (strcmp(accused ->name, "michael") == 0) {
            strcpy(character->hints[2], RED"Michael"RESET" because he and the victim's are enemies");
        }
        else if (strcmp(accused ->name, "ivan") == 0) {
            strcpy(character->hints[2], RED"Ivan"RESET" because he was off for the whole day and stayed in");
        }
        else if (strcmp(accused ->name, "edrick") == 0) {
            strcpy(character->hints[2], RED"Edrick"RESET" because he was acting weird and didn't study that night");
        }
        else if (strcmp(accused ->name, "joey") == 0) {
            strcpy(character->hints[2], RED"Joey"RESET" because he did not go work out at the gym that day");
        }
    }
    else{
        strcpy(character->prefix[2], "I don't know who the muderer is, but I know it is not");
        if (strcmp(accused->name, "chang") == 0) {
           strcpy(character->hints[2], BLU"Chang"RESET" because she was with me when the murder happened");
        } 
        else if (strcmp(accused ->name, "kevin") == 0) {
           strcpy(character->hints[2], BLU"Kevin"RESET" because he is always coding");
        }
        else if (strcmp(accused ->name, "james") == 0) {
            strcpy(character->hints[2], BLU"James"RESET" because he is the nicest man I have ever seen");
        }
        else if (strcmp(accused ->name, "tenzin") == 0) {
            strcpy(character->hints[2], BLU"Tenzin"RESET" because he was playing videogames the time of the murder.");
        }
        else if (strcmp(accused ->name, "minh") == 0) {
            strcpy(character->hints[2], BLU"Minh"RESET" because he is such a nice person");
        }
        else if (strcmp(accused ->name, "michael") == 0) {
            strcpy(character->hints[2], BLU"Michael"RESET" because he is the victim's bestfriend");
        }
        else if (strcmp(accused ->name, "ivan") == 0) {
            strcpy(character->hints[2], BLU"Ivan"RESET" because he was working the whole day");
        }
        else if (strcmp(accused ->name, "edrick") == 0) {
            strcpy(character->hints[2], BLU"Edrick"RESET" because he was busy studying");
        }
        else if (strcmp(accused ->name, "joey") == 0) {
            strcpy(character->hints[2], BLU"Joey"RESET" because he was working out at the gym");
        }
    }
}