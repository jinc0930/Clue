#include "rooms.h"
#include "prettyprint.h"
#include "utils.h"
#include "character.h"
#include "items.h"
#include "pool.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_LINE 100

int main() {
    // a random seed
    srand(time(0));
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
    struct Room* map[9] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

    //randomnize rooms
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

    char instructions[MAX_LINE];
    prompt(instructions, "First time playing? y/n");

    if(strcmp(instructions,"y")==0){
        printf("Welcome! This is a game of clue!\nThe game begins by asking user to input their name, and sets given name as the avatar name.");
        printf("Then, it asks for user to input commands for gameplay, there are many commands, type help to view a list of them.\n");
        printf("The game will end when the user wins or loses, the user can win by guessing the answer correctly, which consist of the correct room, murder weapon, and muderer.\n");
        printf("The 8 npcs spawned randomly will give out hints to the player.\n");
        printf("There are two types of npcs, the hint givers gives out three guarenteed accurate hints that eliminates the suspision while the accusers prompt suspision on possible room, item and person, however - these accusassions are not guarenteed to be correct.\n");
        printf("If the user fails to guess all three correctly with one clue call, they can try again - when clue is called at the eleventh time, the game will end. \n");
    }

    //asks for user name
    char avatarname[MAX_LINE];
    prompt(avatarname, "Welcome, please type your name, keep it short and simple:");

    for(;;) {
        if (strlen(avatarname) == 0) {
            promptErr(avatarname, "Name is blank, please input a name:");
        } else {
            break;
        }
    }
    
    bool already_exists = false;
    int avatarIdx;
    for (size_t i = 0; i < 9; i++){
        if (strcmp(avatarname, getcharname(chararr[i])) == 0) {
            already_exists = true;
            avatarIdx = i;
            break;
        }
    };

    // check there is no conflict
    if (!already_exists) {
        // then choose a random one
        avatarIdx = rand()%9;
    }

    struct Character * avatar = chararr[avatarIdx];
    avatar->name = avatarname;
    avatar->id = "avatar";

    // pool for distribution of items and chars
    struct Pool poolRoom = makePool(9, 1);
    struct Pool poolChar = makePool(9, 1);
    struct Pool poolItem = makePool(9, 1);

    for (size_t i = 0; i < 9; i++) {
        // add a random item to the room
        additem(roomarr[i], itemarr[poolTake(&poolItem, 1)]);

        // take a random character from pool
        struct Character * charChosen = chararr[poolTake(&poolChar, 1)];
        
        // add character to the room
        addChar(roomarr[i], charChosen);

        // add location to the room
        setloc(charChosen, roomarr[i]);
    }

    chararr[avatarIdx]->location->visited = true;
    // exclude avatar
    int exclude[] = {avatarIdx};

    // pools for generating: answers and hints
    struct Pool poolHintsChar = makePoolExcluding(9, 1, exclude, sizeof(exclude)/sizeof(exclude[0])); //size 8
    struct Pool poolHintsRoom = makePool(9, 1);
    struct Pool poolHintsItem = makePool(9, 1);

    //pick the murderer (poolHintsChar size = 7)
    int murderIdx = poolTake(&poolHintsChar, 1); 
    chararr[murderIdx]->id = "murderer";

    //generate answer
    char* targetChar = chararr[murderIdx]->name;
    char* targetRoom = roomarr[poolTake(&poolHintsRoom, 1)]->name;
    char* targetItem = itemarr[poolTake(&poolHintsItem, 1)]->name;

    // pool for distributing the ids
    int excludes[] = {murderIdx, avatarIdx};
    struct Pool poolChars = makePoolExcluding(9, 1, excludes, sizeof(excludes)/sizeof(excludes[0])); // size 7

    // generate 4 hint-givers and set hints
    for (size_t i = 0; i < 4; i++){
        int idx = poolTake(&poolChars, 1);
        chararr[idx]->id = "hint giver";

        // take hints
        struct Item * itemHint = itemarr[poolTake(&poolHintsItem, 1)];
        struct Room * roomHint = roomarr[poolTake(&poolHintsRoom, 1)];
        struct Character * charHint = chararr[poolTake(&poolHintsChar, 1)];

        // todo:
        // set_nice_hint(chararr[idx], itemHint, roomHint, charHint);
    }
    
    // generate 3 accusers and set hints
    int probabilities[] = {16, 32, 64};
    for (size_t i = 0; i < 3; i++){
        int idx = poolTake(&poolChars, 1);
        chararr[idx]->id = "accuser";
        struct Room * roomHint = roomarr[poolTake(&poolHintsRoom, 1)];

        struct Character * accused;
        // check if is going to accuse the correct answer
        if ((rand() % 100) < probabilities[i]) {
            // printf("some accuser is accusing correctly!\n");
            accused = chararr[murderIdx];
        } else {
            // select a random one excluding avatar and self
            int excludes[] = {avatarIdx, idx};
            struct Pool notMe = makePoolExcluding(9, 1, excludes, sizeof(excludes)/sizeof(excludes[0]));
            accused = chararr[poolTake(&notMe, 1)];
        }

        // omit 1 or 2 hints about the item
        if (i > 1 && rand() % 2 == 0) {
            // omit hint
        } else {
            struct Item * itemHint = itemarr[poolTake(&poolHintsItem, 1)];
            // set_item_hint(chararr[idx], itemHint);
        }

        // FIXME
        // set_room_hint(chararr[idx], roomHint);
        // set_char_hint(chararr[idx], accused);
    }

    // hints for the murderer
    struct Item * itemHint = itemarr[poolTake(&poolHintsItem, 1)];
    struct Room * roomHint = roomarr[poolTake(&poolHintsRoom, 1)];
    // make a new pool excluding murder and player
    int excludesMurd[] = {avatarIdx, murderIdx};
    struct Pool notMe = makePoolExcluding(9, 1, excludesMurd, sizeof(excludesMurd)/sizeof(excludesMurd[0]));
    struct Character * accused = chararr[poolTake(&notMe, 1)];
    
    // todo: set_nice_hint(chararr[murderIdx], itemHint, roomHint, accused);

    // printf("hints omitted: C:%d I:%d R:%d\n", poolHintsChar.length, poolHintsItem.length, poolHintsRoom.length); // debug
    
    // (old good way)
    //shuffle character array
    // int intarr1[9]={0,1,2,3,4,5,6,7,8};
    // int intarr2[9]={10,10,10,10,10,10,10,10,10};
    // for (int b = 0; b < 9 ; b++){
    //     int n = rand()%9;
    //     if( intarr2[n] != 10 ){
    //         while (intarr2[n] != 10){
    //             n++;
    //             if(n > 8){n=0;}
    //         }
    //         intarr2[n] = intarr1[b];
    //     }
    //     else{
    //         intarr2[n] = intarr1[b];
    //     }
    // }
    // //asign murderer
    // char* targetChar = chararr[intarr2[0]]->name;
    // chararr[intarr2[0]]->id = "murderer";

    //assign id to all characters, 5 being hint giver and 3 being accuser
    // for(int m=1;m<9;m++){
    //     if(m<6){
    //         chararr[intarr2[m]]->id = "hint giver";
    //     }
    //     else{
    //         chararr[intarr2[m]]->id = "accuser";
    //     }
    // }

    //set boolean to check win statement
    bool booroom;
    bool booitem;
    bool boochara;
    printMap(map);
    // debugMap(map);
    
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
            describe("map", "show the map\n");
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
        else if(startsWith("talk", cmd)){
            // todo
        }
        //or look
        else if(strcmp(cmd, "look")==0){
            printf("You are currently in the %s\n", getloc(avatar)->name);
            int h=0;
            //get all characters in the room
            printf("with: ");
            for (size_t i = 0; i < MAX_CHARACTER; i++){
                struct Character *ch = curroom->chara[i];
                if (ch != NULL && strcmp(ch->id, "avatar") != 0) {
                    printf("%s ", getcharname(ch));
                    h = 1;
                }
            };
            if(h!=1){
                printf("no one");
            }
            printf("\n");
            //items in the room
            struct Item * temp = avatar->location->itemList;
            // struct Item * prev;
            if(temp!=NULL){
                printf("the room has item(s): ");
                while(temp!=NULL){
                  printf("%s / ", temp->name);
                  temp = temp->next;
                }
                printf("\n");
            }
            else{
                printf("with no items in the room\n");
            }
            // getloc(avatar)->itemList = temp;
            //inventory
            if((avatar->inventory) ==NULL){
                  printf("nothing in your inventory\n");
            }
            else{
                printf("items in your inventory include: ");
                struct Item * temp = avatar->inventory;
                while (temp!=NULL){
                     printf("%s / ",temp->name);
                     temp = temp->next;
                }
                printf("\n");
                // avatar->inventory = temp;
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
        else if(startsWith("go", cmd)){
            // printf("make sure to specify which direction would you like to go? north, south, west, or east?\n");
            char direction[MAX_LINE];
            slice(cmd, direction, strlen("go "), MAX_LINE);
            struct Room *curroom = avatar->location;
            struct Room *target;
            //cases for each direction
            if(startsWith(direction, "north")){
                target = getNorth(curroom);
                //if invalid direction
                if(getNorth(curroom)==NULL){
                    printErr("there is no path in that direction");
                } else {  
                    if (moveChar(curroom, target, avatar) == -1) exit(1); // one slot is always available
                    target->visited = true;
                    printMap(map);
                }
            }
            else if(startsWith(direction, "south")){
                target = getSouth(curroom);
                if(getSouth(curroom)==NULL){
                    printErr("there is no path in that direction");
                } else {  
                    if (moveChar(curroom, target, avatar) == -1) exit(1); // one slot is always available
                    target->visited = true;
                    printMap(map);
                }
            }
            else if(startsWith(direction,"west")){
                target = getWest(curroom);
                if(getWest(curroom)==NULL){
                    printErr("there is no path in that direction");
                } else {
                    if (moveChar(curroom, target, avatar) == -1) exit(1); // one slot is always available
                    target->visited = true;
                    printMap(map);
                }
            }
            else if(startsWith(direction,"east")){
                target = getEast(curroom);
                if(getEast(curroom)==NULL){
                    printErr("there is no path in that direction");
                }
                else{  
                    if (moveChar(curroom, target, avatar) == -1) exit(1); // one slot is always available
                    target->visited = true;
                    printMap(map);
                }
            }
            else{
                printErr("invalid direction\n");
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
        else if(startsWith("take", cmd)){
            char *arr[9] = {"bat","rope","butter knife","dagger","rifle","wrench","hammer","lead pipe","poison bottle"};
            char description[MAX_LINE];
            slice(cmd, description, strlen("take "), MAX_LINE);
            if (strcmp(description, "") == 0) {
                prompt(description, "What item do you like to take?");
            }
            curroom = getloc(avatar);
            //iterate to see if such item exist in the game
            bool c = false;
            for (int t=0;t<9;t++){
                if (startsWith(description, getItemName(itemarr[t]))){
                    c = true;
                }
            }
            if(!c){
                printErr("that item does not exist");
            }
            else{
                struct Item * m;
                //if current room has no items
                if(curroom->itemList == NULL){
                    printErr("sorry, there is nothing in this room");
                }
                //if it has more than one items
                else if((curroom->itemList!=NULL)&&(curroom->itemList->next !=NULL)){
                    m = curroom->itemList;
                    while(m != NULL){
                        if (startsWith(description, m->name)){
                            if (add(avatar,m) != 0) {
                                removeItem(curroom, m);
                                printf(YEL "you took the %s\n" RESET, m->name);
                            } else {
                                printErr("sorry, your inventory is full");
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
                        printf(YEL "you took the %s\n", m->name);
                    } else {
                        printf("sorry, your inventory is full\n");
                    }
                }
            }                
        }
        //if command was drop
        else if(startsWith("drop", cmd)){
            char *arr1[9] = {"bat","rope","butter knife","dagger","rifle","wrench","hammer","lead pipe","poison bottle"};
            char description[MAX_LINE];
            slice(cmd, description, strlen("drop "), MAX_LINE);
            if (strcmp(description, "") == 0) {
                prompt(description, "What item do you like to drop?");
            }
            curroom = getloc(avatar);
            //loop through array of item list to check if input item is valid
            bool c = false;
            for (int t=0;t<9;t++){
                if (strcmp(description,getItemName(itemarr[t]))==0){
                    c = true;
                }
            }
            if(!c){
                printErr("that item does not exist");
            }
            //if it does exist, check if it's in the inventory of the user
            else{
                struct Item * m;
                //if user inventory is empty
                if(avatar->inventory ==NULL){
                    printErr("you do not carry any item at the moment");
                }
                //if user inventory has more than one item
                else if((avatar->inventory !=NULL)&&(avatar->inventory->next !=NULL)){
                    m = avatar->inventory;
                    while(m != NULL){
                        if (strcmp(getItemName(m),description)==0){
                            rmv(avatar, m);
                            additem(curroom,m);
                            printf(YEL "you dropped the %s\n" RESET, m->name);
                            break;
                        }
                        m  = getNext(m);
                    }
                }
                //if user inventory has one item
                else {
                    m = avatar->inventory;
                    rmv(avatar, m);
                    additem(curroom, m);
                    printf(YEL "you dropped the %s\n" RESET, m->name);
                }
                
            }
        }
        else if(strcmp(cmd,"quit")==0){
            clue = 12;
        }
        //or map
        else if(strcmp(cmd,"map")==0){
            printMap(roomarr);
        }
        //if command is clue
        else if(startsWith("clue", cmd)){
            //temperary statement for testing
            //printf("%s,%s,%s\n",targetRoom,targetChar,targetItem);
            char des[MAX_LINE];
            slice(cmd, des, strlen("clue "), MAX_LINE);
            if (strcmp(des, "") == 0) {
                prompt(des, "Who is the murderer");
            }
            //check if user input a valid character
            int rep = 0;
            for(int r = 0; r<9;r++){
                //case insensitive
                if(strcasecmp(chararr[r]->name,des)==0){
                    rep = 1;
                }
            }
            if(rep == 0){
                printErr("not a valid chracter");
            }
            else if(roomCharLength(curroom) >= MAX_CHARACTER){
                printErr("too many characters in this room already");
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
                    printSucc("ROOM MATCH");
                    booroom = true;
                }
                else{
                    printErr("WRONG ROOM");
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
                    printSucc("ITEM MATCH");
                    booitem = true;
                }
                else{
                    printErr("WRONG ITEM");
                }
                //checked validity above, now check if already in room
                int h = 0;
                for(int z = 0; z<9;z++){
                    //iterate and check all character in room
                    if(strcmp(getloc(chararr[z])->name, curroom->name)==0){
                        //the desired character is already in the room
                        if(strcmp(getcharname(chararr[z]),des)==0){
                            if(strcmp(targetChar,des)==0){
                                printSucc("CHARACTER MATCH");
                                boochara = true;
                                h=1;
                            }
                        }
                    }
                    //valid character, but not in the room
                    if(h!=1){
                        if(strcmp(getcharname(chararr[z]),des)==0){
                            //check where character is on the character array
                            int temp = 0;
                            for(int v = 0; v<9;v++){
                                if(strcmp(roomarr[v]->name,curroom->name)==0){
                                    temp = v;
                                    break;
                                }
                            }
                            moveChar(chararr[z]->location, curroom, chararr[z]);
                            //after adding character to the room, check if it's the correct guess
                            if(strcmp(des,targetChar)==0){
                                printSucc("CHARACTER MATCH");
                                boochara = true;
                            }
                            else{
                                printErr("WRONG CHARACTER");
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
