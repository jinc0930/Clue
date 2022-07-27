#include "rooms.h"
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
    for (int i = 0; i < 9 ; i++ ){
        int j = rand()%9;
        if( map[j] != NULL ){
            while( map[j] != NULL){
                j++;
                if( j > 8 ){j=0;}
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
    struct Item * item1 = makeitem("knife");
    struct Item * item2 = makeitem("bat");
    struct Item * item3 = makeitem("wrench");
    struct Item * item4 = makeitem("rope");
    struct Item * item5 = makeitem("dagger");
    struct Item * item6 = makeitem("rifle");
    //item arr for easier access later
    struct Item* itemarr[6] = {item1,item2,item3,item4,item5,item6};

    //make characters
    struct Charatcer * char1 = makeChar("minh");
    struct Charatcer * char2 = makeChar("james");
    struct Charatcer * char3 = makeChar("ivan");
    struct Charatcer * char4 = makeChar("tenzin");
    struct Charatcer * char5 = makeChar("edrick");
    //arr for easier access later
    struct Character * chararr[5] = {char1,char2,char3,char4,char5};
    int charnum[9] = {0,0,0,0,0,0,0,0,0};

    //add item to rooms
    additem(room1, item1);
    additem(room2, item2);
    additem(room3, item3);
    additem(room4, item4);
    additem(room5, item5);
    additem(room6, item6);

    //set chracter locations
    setloc(char1, room1);
    setloc(char2, room2);
    setloc(char3, room3);
    setloc(char4, room4);
    setloc(char5, room5);
    addChar(room1, char1, charnum[0]);
    addChar(room2, char2, charnum[1]);
    addChar(room3, char3, charnum[2]);
    addChar(room4, char4, charnum[3]);
    addChar(room5, char5, charnum[4]);

    //asks for user name 
    printf("Welcome, please type your name, keep it short and simple\n");
    char avatarname[MAX_LINE];
    scanf("%s", avatarname);
    struct Character * avatar = makeChar(avatarname);

    //spawn avatar in a room with userinputed name
    setloc(avatar, room6);
    addChar(room6, avatar, charnum[5]);

    //generate answer
    char* targetRoom = roomarr[rand()%9]->name;
    char* targetChar = chararr[rand()%5]->name;
    char* targetItem = itemarr[rand()%6]->name;  

    //set boolean to check win statement
    bool booroom;
    bool booitem;
    bool boochara;

    //main game portion with clue counter
    int clue = 0;
    while( clue <= 10 ){
        struct Room* curroom = avatar ->location;
        char cmd[MAX_LINE];
        printf("Type your command : \n");
        scanf("%s", cmd);
        //if command is help
        if(strcmp(cmd,"help")==0){
            printf("This is a list of all commands\n");
            printf("help: shows the list of all commands\n");
            printf("list: list of all room, character and item names\n");
            printf("look: shows current room, items in the room, player's inventory, and the same information for rooms in all directions\n");
            printf("go: move the avatar, you can pick from four directions north, south, west, and east\n");
            printf("take: pick up an item and place it in your inventory\n");
            printf("drop: drop an item in the room you are currently in\n");
            printf("inventory: list all items in your current inventory\n");
            printf("clue: make your guess with this call, you can pick from all valid characters\n");
        }
        //or list
        else if(strcmp(cmd,"list")==0){
            printf("This is a list of all rooms, characters and items\n");
            printf("rooms include: kitchen, study, hall, ballroom, library, lounge, bathroom, office, and bedroom\n");
            printf("items include: knife, rope, rifle, dagger, and bat\n");
            printf("characters include: minh, ivan, tenzin, edrick, and james\n");
        }
        //or look
        else if(strcmp(cmd, "look")==0){
            printf("You are currently in the %s\n", getloc(avatar)->name);
            int h=0;
            //get all characters in the room
            printf("with: ");
            for(int z = 0;z<5;z++){
                if(strcmp(getloc(chararr[z])->name, curroom->name)==0){
                    printf("%s ", getcharname(chararr[z]));
                    h = 1;
                }
            }
            if(h!=1){
                printf("noone");
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
        else if(strcmp(cmd,"go")==0){
            printf("make sure to specify which direction would you like to go? north, south, west, or east?\n");
            char direction[MAX_LINE];
            scanf("%s",direction);
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
        else if(strcmp(cmd,"take")==0){
            char *arr[6] = {"bat","rope","knife","dagger","rifle","wrench"};
            printf("make sure you specify what item you would like to take\n");
            char description[MAX_LINE];
            scanf("%s",description);
            curroom = getloc(avatar);
            //iterate to see if such item exist in the game
            bool c = false;
            for (int t=0;t<6;t++){
                if (strcmp(description,getname(itemarr[t]))==0){
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
                        if (strcmp(m->name,description)==0){
                            removeItem(curroom, m);
                            add(avatar,m);
                        }
                        m  = getNext(m);
                    }
                }
                //if it has only one item
                else{
                    m =curroom->itemList;
                    removeItem(curroom,m);
                    add(avatar,m);
                }
            }                
        }
        //if command was drop
        else if(strcmp(cmd,"drop")==0){
            char * arr1[6] = {"bat","rope","knife","dagger","rifle","wrench"};
            printf("what would you like to drop?\n");
            char description[MAX_LINE];
            scanf("%s",description);
            curroom = getloc(avatar);
            //loop through array of item list to check if input item is valid
            bool c=false;
            for (int t=0;t<6;t++){
                if (strcmp(description,getname(itemarr[t]))==0){
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
                        if (strcmp(getN(m),description)==0){
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
        //if command is clue
        else if(strcmp(cmd,"clue")==0){
            //temperary statement for testing
            //printf("%s,%s,%s\n",targetRoom,targetChar,targetItem);
            printf("make sure you include a character\n");
            char des[MAX_LINE];
            scanf("%s",des);
            //check if user input a valid character
            int rep = 0;
            for(int r = 0; r<5;r++){
                if(strcmp(chararr[r]->name,des)==0){
                    rep = 1;
                }
            }
            if(rep == 0){
                printf("not a valid chracter\n");
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
                //check character
                //checked validity above, now check if already in room
                int h=0;
                for(int z = 0;z<5;z++){
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
                            int temp =0;
                            for(int v = 0; v<9;v++){
                                if(strcmp(roomarr[v]->name,curroom->name)==0){
                                    temp = v;
                                    break;
                                }
                            }
                            addChar(curroom,chararr[z],charnum[temp]);
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
    if(booroom && booitem && boochara){
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
    for(int l=0;l<6;l++){
        free(itemarr[l]);
    }
    //free characters
    for(int j=0;j<5;j++){
        free(chararr[j]);
    }
    //and avatar
    free(avatar);

    return 0;
}
