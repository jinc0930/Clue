#include "rooms.h"
#include "prettyprint.h"
#include "utils.h"
#include "character.h"
#include "items.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_LINE 100

int main() {
    //make nine rooms
    struct Room * room1 = makeroom("kitchen");
    struct Room * room2 = makeroom("hall");
    struct Room * room3 = makeroom("study");
    struct Room * room4 = makeroom("bathroom");
    struct Room * room5 = makeroom("ballroom");
    struct Room * room6 = makeroom("library");
    struct Room * room7 = makeroom("lounge");
    struct Room * room8 = makeroom("office");
    struct Room * room9 = makeroom("bedroom");
    //making arrays for easier manipulation in randomnizing them
    struct Room* roomarr[9] = {room1,room2,room3,room4,room5,room6,room7,room8,room9};
    struct Room* map[9] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

    //randomnize rooms
    srand(time(0));
    for (int i = 0; i < 9 ; i++){
        int j = rand()%9;
        if( map[j] != NULL ){
            while (map[j] != NULL){
                j++;
                if(j > 8 ){j=0;}
            }
            map[j] = roomarr[i];
        }
        else{
            map[j] = roomarr[i];
        }
    }

    //set (pointers) directions of rooms
    for (int p = 0; p < 9; p++ ){
        if( p > 2 ){
            setNorth(map[p], map[p-3]);
        }
        if( p < 6 ){
            setSouth(map[p], map[p+3]);
        }
        if( p != 0 && p != 3 && p != 6){
            setWest(map[p], map[p-1]);
        }
        if( p != 2 && p != 5 && p != 8){
            setEast(map[p],map[p+1]);
        }
    }

    //make items
    struct Item * item1 = makeitem("butter knife");
    struct Item * item2 = makeitem("bat");
    struct Item * item3 = makeitem("wrench");
    struct Item * item4 = makeitem("rope");
    struct Item * item5 = makeitem("dagger");
    struct Item * item6 = makeitem("rifle");
    struct Item * item7 = makeitem("hammer");
    struct Item * item8 = makeitem("lead pipe");
    struct Item * item9 = makeitem("poison bottle");
    //item arr for easier access later
    struct Item* itemarr[9] = {item1,item2,item3,item4,item5,item6,item7,item8,item9};

    //make characters
    struct Character * char1 = makeChar("minh");
    struct Character * char2 = makeChar("james");
    struct Character * char3 = makeChar("ivan");
    struct Character * char4 = makeChar("tenzin");
    struct Character * char5 = makeChar("edrick");
    struct Character * char6 = makeChar("chang");
    struct Character * char7 = makeChar("kevin");
    struct Character * char8 = makeChar("michael");
    struct Character * char9 = makeChar("joey");
    //arr for easier access later
    struct Character * chararr[9] = {char1,char2,char3,char4,char5,char6,char7,char8,char9};
    int charnum[9] = {0,0,0,0,0,0,0,0,0};

    //add item to rooms
    additem(room1, item1);
    additem(room2, item2);
    additem(room3, item3);
    additem(room4, item4);
    additem(room5, item5);
    additem(room6, item6);
    additem(room7, item7);
    additem(room8, item8);
    additem(room9, item9);

    //set chracter locations
    setloc(char1, room1);
    setloc(char2, room2);
    setloc(char3, room3);
    setloc(char4, room4);
    setloc(char5, room5);
    setloc(char6, room6);
    setloc(char7, room7);
    setloc(char8, room8);
    setloc(char9, room9);
    addChar(room1, char1, charnum[0]);
    addChar(room2, char2, charnum[1]);
    addChar(room3, char3, charnum[2]);
    addChar(room4, char4, charnum[3]);
    addChar(room5, char5, charnum[4]);
    addChar(room6, char6, charnum[5]);
    addChar(room7, char7, charnum[6]);
    addChar(room8, char8, charnum[7]);
    addChar(room9, char9, charnum[8]);

    //asks for user name 
    char avatarname[MAX_LINE];
    prompt(avatarname, "Welcome, please type your name, keep it short and simple: ");
    
    struct Character * avatar = makeChar(avatarname);

    //spawn avatar in a room with userinputed name
    setloc(avatar, room6);
    addChar(room6, avatar, charnum[5]);
    room6->num +=1;

    //generate answer
    char* targetRoom = roomarr[rand()%9]->name;
    char* targetChar = chararr[rand()%9]->name;
    char* targetItem = itemarr[rand()%9]->name;  

    //set boolean to check win statement
    bool booroom;
    bool booitem;
    bool boochara;
    printMap(map);
    //main game portion with clue counter
    int clue = 0;
    while( clue <= 10 ){
        struct Room* curroom = avatar ->location;
        char cmd[MAX_LINE];
        promptWithLoc(cmd, "Type your command:", getRoomName(curroom));
        // printf("%s", cmd);
        //if command is help
        if(strcmp(cmd,"help")==0){
            printf(BLD "COMMANDS:\n" RESET);
            describe("list", "list all room, character and item names\n");
            describe("look", "show current room, items in the room, player's inventory, and the name of the rooms surrounding the current room\n");
            describe("go", "move the avatar, you can pick from four directions north, south, west, and east\n");
            describe("take", "pick up an item and place it in player inventory\n");
            describe("drop", "drop an item from player inventory to the room player is currently in\n");
            describe("inventory", "list all items in your current inventory\n");
            describe("clue", "make your guess of the murderer, murder room and murder weapon with this call, you can pick from all valid characters\n");
            describe("help", "show the list of all commands\n");
        }
        //or list
        else if(strcmp(cmd,"list")==0){
            printf("This is a list of all rooms, characters and items\n");
            printf("rooms include: kitchen, study, hall, ballroom, library, lounge, bathroom, office, and bedroom\n");
            printf("items include: butter knife, rope, rifle, dagger, hammer, lead pipe, poison bottle, and bat\n");
            printf("characters include: minh, ivan, tenzin, edrick, chang, kevin, joey, and james\n");
        }
        //or look
        else if(strcmp(cmd, "look")==0){
            printf("You are currently in the %s\n", getloc(avatar)->name);
            int h=0;
            //get all characters in the room
            printf("with: ");
            for(int z = 0;z<9;z++){
                if(strcmp(getloc(chararr[z])->name, curroom->name)==0){
                    printf("%s ", getcharname(chararr[z]));
                    h = 1;
                }
            }
            if(h!=1){
                printf("no one");
            }
            printf("\n");
            //items in the room
            struct Item * temp = getloc(avatar)->itemList;
            if(getloc(avatar)->itemList!=NULL){
                printf("the room has item(s): ");
                while((getloc(avatar)->itemList)!=NULL){
                  printf("%s ",(getloc(avatar)->itemList->name));
                  getloc(avatar)->itemList = getloc(avatar)->itemList->next;
                }
                printf("\n");
            }
            else{
                printf("with no items in the room\n");
            }
            getloc(avatar)->itemList = temp;
            //inventory
            if((avatar->inventory) ==NULL){
                  printf("nothing in your inventory\n");
            }
            else{
                printf("items in your inventory include: ");
                struct Item* original = avatar->inventory;
                while (avatar->inventory!=NULL){
                     printf("%s ",avatar->inventory->name);
                     avatar->inventory = avatar->inventory->next;
                }
                printf("\n");
                avatar->inventory = original;
            }
            //print rooms in every direction 
            if (getloc(avatar)->North != NULL){
                printf("To your north is %s\n",getloc(avatar)->North->name);
            }
            if(getloc(avatar)->South != NULL){
                printf("To your south is %s\n", getloc(avatar)->South->name);
            }
            if(getloc(avatar)->East !=NULL){
                printf("To your east is %s\n", getloc(avatar)->East->name);
            }
            if(getloc(avatar)->West!=NULL){
                printf("To your west is %s\n", getloc(avatar)->West->name);
            }
        }
        //if command was go
        else if(startsWith(cmd,"go")!=0){
            // printf("make sure to specify which direction would you like to go? north, south, west, or east?\n");
            char direction[MAX_LINE];
            slice(cmd, direction, strlen("go "), MAX_LINE);
            struct Room *curroom = avatar->location;
            struct Room *target;
            //cases for each direction
            if(strcmp(direction, "north")==0){
                target = getNorth(curroom);
                //if invalid direction
                if(getNorth(curroom)==NULL){
                    printf("there is no path in that direction\n");
                }
                //otherwise...
                else{  
                    removeChar(curroom, avatar, curroom->num);
                    setloc(avatar, target);
                    addChar(target,avatar,target->num);
                }
            }
            else if(strcmp(direction, "south")==0){
                target = getSouth(curroom);
                if(getSouth(curroom)==NULL){
                    printf("there is no path in that direction\n");
                }
                else{  
                    removeChar(curroom, avatar, curroom->num);
                    setloc(avatar, target);
                    addChar(target,avatar,target->num);
                }
            }
            else if(strcmp(direction,"west")==0){
                target = getWest(curroom);
                if(getWest(curroom)==NULL){
                    printf("there is no path in that direction\n");
                }
                else{  
                    removeChar(curroom, avatar, curroom->num);
                    setloc(avatar, target);
                    addChar(target,avatar,target->num);
                }
            }
            else if(strcmp(direction,"east")==0){
                target = getEast(curroom);
                if(getEast(curroom)==NULL){
                    printf("there is no path in that direction\n");
                }
                else{  
                    removeChar(curroom, avatar, curroom->num);
                    setloc(avatar, target);
                    addChar(target,avatar,target->num);
                }
            }
            else{
                printf("invalid direction\n");
            }
        }
        //if command was inventory
        else if(strcmp(cmd,"inventory")==0){
            if((avatar->inventory) ==NULL){
                  printf("nothing in your inventory\n");
            }
            else{
                printf("items in your inventory include: ");
                struct Item* original = avatar->inventory;
                while (avatar->inventory!=NULL){
                     printf("%s ",avatar->inventory->name);
                     avatar->inventory = avatar->inventory->next;
                }
                printf("\n");
                avatar->inventory = original;
            }
        }
        //take item command
        else if(startsWith(cmd, "take") != 0){
            char *arr[9] = {"bat","rope","butter knife","dagger","rifle","wrench","hammer","lead pipe","poison bottle"};
            char description[MAX_LINE];
            slice(cmd, description, strlen("take "), MAX_LINE);
            curroom = getloc(avatar);
            //iterate to see if such item exist in the game
            bool c = false;
            for (int t=0;t<9;t++){
                if (startsWith(getItemName(itemarr[t]), description)==0){
                    c = true;
                }
            }
            if(!c){
                printf("that item does not exist\n");
            }
            else{
                struct Item * m;
                //if current room has no items
                if(curroom->itemList == NULL){
                    printf("sorry, there is nothing in this room\n");
                }
                //if it has more than one items
                else if((curroom->itemList!=NULL)&&(curroom->itemList->next !=NULL)){
                    m = curroom->itemList;
                    while(m != NULL){
                        if (startsWith(m->name, description) != 0){
                            if (add(avatar,m) != 0) {
                                removeItem(curroom, m);
                                printf("you took the %s\n", description);
                            } else {
                                printf("sorry, your inventory is full\n");
                            }
                            break;
                        }
                        m  = getNext(m);
                    }
                }
                //if it has only one item
                else{
                    m =curroom->itemList;
                    if (add(avatar,m) != 0) {
                        removeItem(curroom,m);
                        printf("you took the %s\n", description);
                    } else {
                        printf("sorry, your inventory is full\n");
                    }
                }
            }                
        }
        //if command was drop
        else if(startsWith(cmd, "drop") != 0){
            char *arr1[9] = {"bat","rope","butter knife","dagger","rifle","wrench","hammer","lead pipe","poison bottle"};
            char description[MAX_LINE];
            slice(cmd, description, strlen("drop "), MAX_LINE);
            curroom = getloc(avatar);
            //loop through array of item list to check if input item is valid
            bool c = false;
            for (int t=0;t<9;t++){
                if (strcmp(description,getItemName(itemarr[t]))==0){
                    c = true;
                }
            }
            if(!c){
                printf("that item does not exist\n");
            }
            //if it does exist, check if it's in the inventory of the user
            else{
                struct Item * m;
                //if user inventory is empty
                if(avatar->inventory ==NULL){
                    printf("you do not carry any item at the moment");
                }
                //if user inventory has more than one item
                else if((avatar->inventory !=NULL)&&(avatar->inventory->next !=NULL)){
                    m = avatar->inventory;
                    while(m != NULL){
                        if (strcmp(getItemName(m),description)==0){
                            rmv(avatar, m);
                            additem(curroom,m);
                        }
                        m  = getNext(m);
                    }
                }
                //if user inventory has one item
                else {
                    m = avatar->inventory;
                    rmv(avatar, m);
                    additem(curroom, m);
                }
            }
        }
        else if(strcmp(cmd,"quit")==0){
            clue = 12;
        }
        //if command is clue
        else if(startsWith(cmd, "clue")!=0){
            //temperary statement for testing
            //printf("%s,%s,%s\n",targetRoom,targetChar,targetItem);
            char des[MAX_LINE];
            slice(cmd, des, strlen("clue "), MAX_LINE);
            // prompt(des, "Who is the murderer?");
            //check if user input a valid character
            int rep = 0;
            for(int r = 0; r<9;r++){
                //case insensitive
                if(strcasecmp(chararr[r]->name,des)==0){
                    rep = 1;
                }
            }
            if(rep == 0){
                printf("not a valid chracter\n");
            }
            else if((curroom->num)>=3){
                printf("too many characters in this room already\n");
            }
            //they do input a valid character
            else{ 
                clue++;
                //satisfy all three conditions to win
                booroom = false;
                booitem = false;
                boochara = false;
                //check if room matches the target room
                if(strcmp(getloc(avatar)->name, targetRoom)==0){
                    printf("ROOM MATCH\n");
                    booroom = true;
                }
                else{
                    printf("WRONG ROOM\n");
                }
                //check if item matches (in the room or on the character)
                struct Item * inv = avatar ->inventory;
                struct Item * roominv = curroom->itemList;
                //set boolean (find item or not)
                bool temp = false;
                //loop through user inventory and see if item is there
                while(inv!=NULL){
                    if(strcmp(inv->name,targetItem)==0){
                        temp=true;
                    }
                    inv = inv->next;
                }
                //loop through room itemList and see if item is there
                while(roominv!=NULL){
                    if(strcmp(roominv->name,targetItem)==0){
                        temp=true;
                    }
                    roominv = roominv->next;
                }
                //if item is in either room or inventory
                if(temp){
                    printf("ITEM MATCH\n");
                    booitem = true;
                }
                else{
                    printf("WRONG ITEM\n");
                }
                //checked validity above, now check if already in room
                int h = 0;
                for(int z = 0; z<9;z++){
                    //iterate and check all character in room
                    if(strcmp(getloc(chararr[z])->name, curroom->name)==0){
                        //the desired character is already in the room
                        if(strcmp(getcharname(chararr[z]),des)==0){
                            if(strcmp(targetChar,des)==0){
                                printf("CHARACTER MATCH\n");
                                boochara = true;
                                h=1;
                            }
                        }
                    }
                    //valid character, but not in the room
                    if(h!=1){
                        if(strcmp(getcharname(chararr[z]),des)==0){
                            //check where the called character is and bring character in current room
                            struct Room* position = getloc(chararr[z]);
                            //set character to current room
                            setloc(chararr[z], curroom);
                            //check where character is on the character array
                            int temp = 0;
                            for(int v = 0; v<9;v++){
                                if(strcmp(roomarr[v]->name,curroom->name)==0){
                                    temp = v;
                                    break;
                                }
                            }
                            addChar(curroom,chararr[z],charnum[temp]);
                            curroom->num+=1;
                            //after adding character to the room, check if it's the correct guess
                            if(strcmp(des,targetChar)==0){
                                printf("CHARACTER MATCH\n");
                                boochara = true;
                            }
                            else{
                                printf("WRONG CHARACTER\n");
                            }
                        }
                    }
                    //check if guesses correctly
                    if(booroom && booitem && boochara){
                        //breaks gaming loop
                        clue = 11;
                    }
                }               
            }
        }
        else{
            printf("Please input a valid command\n");
        }
    }
    //check if user gets all three answers correct
    if(clue==12){
        printf("Thank you for playing, see you next time!\n");
    }
    else if(booroom && booitem && boochara){
        printf("YOU WON\n");
    }
    else{
        printf("YOU LOST\n");
    }
    //free everything
    //fress rooms
    for(int k =0;k<9;k++){
        free(roomarr[k]);
    }
    //free items
    for(int l=0;l<9;l++){
        free(itemarr[l]);
    }
    //free characters
    for(int j=0;j<9;j++){
        free(chararr[j]);
    }
    //and avatar
    free(avatar);

    return 0;
}
