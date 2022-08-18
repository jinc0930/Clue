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
// #define DEBUG 1 // or just use: make debug
#ifdef DEBUG
# define DEBUG_PRINT(x) printf x
#else
# define DEBUG_PRINT(x) do {} while (0)
#endif

int main() {
    printClue();
    // a random seed
    srand(time(NULL));
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

    #ifndef TEST
    char instructions[MAX_LINE];
    prompt(instructions, "First time playing? y/n");

    for(;;) {
        if(strcasecmp(instructions,"y")==0 || startsWith("y", instructions) || startsWith("Y", instructions)){
            printf("Welcome! This is a game of clue!\nThe game begins by asking player to input name, and sets given name as the avatar name.");
            printf("Then, it asks for user to input commands for gameplay, there are many commands, input help to view a list of them.\n");
            printf("The game will end when the user wins or loses\n");
            printf("The player can win by guessing the answer correctly, which consist of the correct room, murder weapon, and muderer.\n");
            printf("To make the game more winnable, 8 npcs will spawn randomly will give out hints to the player.\n");
            printf("There are two types of npcs, hint givers gives out three guarenteed accurate hints that eliminates the suspision of a room, weapon, and person\n");
            printf("The accusers, on the other hand, prompt suspision on possible room, weapon and muderder, however - these accusassions are not guarenteed to be correct.\n");
            printf("If the truth potion, a hidden item is found by the player, they can use it on one npc where the npc will give out one guarenteed correct accusion\n");
            printf("If the user fails to guess all three correctly, they can try again until clue is called at the eleventh time, where the game will end.\n");
            break;
        } else if (strcasecmp(instructions,"n")==0 || startsWith("n", instructions) || startsWith("N", instructions)){
            break;
        } else{
            promptErr(instructions, "First time playing? y/n");
        }
    }
    #endif
    
    #ifndef TEST
    //asks for user name
    char avatarname[MAX_LINE];
    prompt(avatarname, "Welcome, please type your name, keep it short and simple:");

    DEBUG_PRINT(("DEBUG: check name\n"));
    for(;;) {
        if (strlen(avatarname) == 0) {
            promptErr(avatarname, "Name is blank, please input a name:");
        } else {
            break;
        }
    }
    #endif
    bool already_exists = false;
    int avatarIdx = 0;
    #ifndef TEST
    for (size_t i = 0; i < 9; i++){
        if (strcasecmp(avatarname, getcharname(chararr[i])) == 0) {
            already_exists = true;
            avatarIdx = i;
            break;
        }
    };
    #endif

    DEBUG_PRINT(("DEBUG: check if name already exists\n"));
    // check there is no conflict
    if (!already_exists) {
        // then choose a random one
        avatarIdx = rand()%9;
    }

    DEBUG_PRINT(("DEBUG: set new character\n"));
    struct Character * avatar = chararr[avatarIdx];
    avatar->id = "avatar";
    #ifndef TEST
    avatar->name = avatarname;
    #else
    avatar->name = "tester";
    #endif

    // make a string of all current characters in game
    DEBUG_PRINT(("DEBUG: create string of current characters\n"));
    char ingame[500];
    strcpy(ingame, "");
    for (size_t i = 0; i < 9; i++){
        if (avatarIdx == i) continue;
        if (i != 0) strcat(ingame, ", ");
        strcat(ingame, chararr[i]->name);
    }

    DEBUG_PRINT(("DEBUG: make 3 pools for distribution\n"));
    // pool for distribution of items and chars
    struct Pool poolRoom = makePool(9, 1);
    struct Pool poolChar = makePool(9, 1);
    struct Pool poolItem = makePool(9, 1);
    DEBUG_PRINT(("DEBUG: distribute item and chars\n"));
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
    DEBUG_PRINT(("DEBUG: create pool hints\n"));
    // pools for generating: answers and hints
    struct Pool poolHintsChar = makePoolExcluding(9, 1, exclude, sizeof(exclude)/sizeof(exclude[0])); //size 8
    struct Pool poolHintsRoom = makePool(9, 1);
    struct Pool poolHintsItem = makePool(9, 1);

    DEBUG_PRINT(("DEBUG: generate answer\n"));
    //pick the murderer (poolHintsChar size = 7)
    int murderIdx = poolTake(&poolHintsChar, 1); 
    chararr[murderIdx]->id = "murderer";
    int roomIdx = poolTake(&poolHintsRoom, 1);
    int itemIdx = poolTake(&poolHintsItem, 1);

    //generate answer
    char* targetChar = chararr[murderIdx]->name;
    char* targetRoom = roomarr[roomIdx]->name;
    char* targetItem = itemarr[itemIdx]->name;

    // pool for distributing the ids
    int excludes[] = {murderIdx, avatarIdx};
    struct Pool poolChars = makePoolExcluding(9, 1, excludes, sizeof(excludes)/sizeof(excludes[0])); // size 7

    // generate 4 hint-givers and set hints
    DEBUG_PRINT(("DEBUG: generate hint givers\n"));
    for (size_t i = 0; i < 4; i++){
        int idx = poolTake(&poolChars, 1);
        chararr[idx]->id = "hint giver";

        // take hints
        struct Item * itemHint = itemarr[poolTake(&poolHintsItem, 1)];
        struct Room * roomHint = roomarr[poolTake(&poolHintsRoom, 1)];
        struct Character * charHint = chararr[poolTake(&poolHintsChar, 1)];

        DEBUG_PRINT(("DEBUG: set hints for a hint giver\n"));
        set_item_hint(chararr[idx], itemHint);
        set_room_hint(chararr[idx], roomHint);
        set_char_hint(chararr[idx], charHint);
    }
    
    // generate 3 accusers and set hints
    DEBUG_PRINT(("DEBUG: shuffle accusers proababilities\n"));
    int probs[] = {0, 0, 0, 0, 2, 4, 8, 16, 64};
    shuffle(probs, sizeof(probs)/sizeof(probs[0]));

    DEBUG_PRINT(("DEBUG: generate accusers\n"));
    for (size_t i = 0; i < 3; i++){
        int idx = poolTake(&poolChars, 1);
        chararr[idx]->id = "accuser";

        struct Character * accused = NULL;
        struct Item * accused_item = NULL;
        struct Room * accused_room = NULL;

        // accuse correct item?
        if ((rand() % 100 < probs[i*3])) {
            // printf("accusing right item\n");
            accused_item = itemarr[itemIdx];
        } else {
            accused_item = itemarr[poolTake(&poolHintsItem, 1)];
        }
        // accuse correct room?
        if ((rand() % 100 < probs[i*3+1])) {
            // printf("accusing right room\n");
            accused_room = roomarr[roomIdx];
        } else {
            accused_room = roomarr[poolTake(&poolHintsRoom, 1)];
        }

        // accuse correct char?
        if ((rand() % 100) < probs[i*3+2]) {
            // printf("accusing right char\n");
            accused = chararr[murderIdx];
        } else {
            // select a random one excluding avatar and self
            int excludes[] = {avatarIdx, idx};
            struct Pool notMe = makePoolExcluding(9, 1, excludes, sizeof(excludes)/sizeof(excludes[0]));
            accused = chararr[poolTake(&notMe, 1)];
        }

        DEBUG_PRINT(("DEBUG: set hints for an accuser\n"));
        set_item_hint(chararr[idx], accused_item);
        set_room_hint(chararr[idx], accused_room);
        set_char_hint(chararr[idx], accused);
    }

    // hints for the murderer
    DEBUG_PRINT(("DEBUG: generate hints for the murderer\n"));
    struct Item * itemHint = itemarr[poolTake(&poolHintsItem, 1)];
    struct Room * roomHint = roomarr[poolTake(&poolHintsRoom, 1)];
    // make a new pool excluding murder and player
    int excludesMurd[] = {avatarIdx, murderIdx};
    struct Pool notMe = makePoolExcluding(9, 1, excludesMurd, sizeof(excludesMurd)/sizeof(excludesMurd[0]));
    struct Character * accused = chararr[poolTake(&notMe, 1)];
    
    DEBUG_PRINT(("DEBUG: set hints for an accuser\n"));
    set_item_hint(chararr[murderIdx], itemHint);
    set_room_hint(chararr[murderIdx], roomHint);
    set_char_hint(chararr[murderIdx], accused);

    DEBUG_PRINT(("DEBUG: start the game\n"));
    //set boolean to check win statement
    bool booroom;
    bool booitem;
    bool boochara;
    printMap(map);
    printRoomItems(avatar->location);

    //main game portion with clue counter
    int clue = 0;
    int attempts = 0;
    #ifndef TEST
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
            printf("characters include: %s.\n", ingame);
        }
        else if(startsWith("talk", cmd)){
            // todo
            char description[MAX_LINE];
            slice(cmd, description, strlen("talk "), MAX_LINE);
            if (strcmp(description, "") == 0) {
                prompt(description, "Who do you want to talk to?");
            }

            bool check= false;
            //check if character input is valid(that character exists)
            for(int a =0;a<9;a++){
                if((strcmp(description,chararr[a]->name)==0)&&(strcmp(description,avatar->name)!=0)&& (strcmp(getloc(avatar)->name,getloc(chararr[a])->name)==0)){
                    // experimental typing
                    char str[100];
                    sprintf(str, "%s %s.\n", chararr[a]->prefix[0], chararr[a]->hints[0]);
                    printTyping(str);
                    sprintf(str, "%s %s.\n", chararr[a]->prefix[1], chararr[a]->hints[1]);
                    printTyping(str);
                    sprintf(str, "%s %s.\n", chararr[a]->prefix[2], chararr[a]->hints[2]);
                    printTyping(str);

                    // old way
                    // printf("%s %s.\n",chararr[a]->prefix[0],chararr[a]->hints[0]);
                    // printf("%s %s.\n",chararr[a]->prefix[1],chararr[a]->hints[1]);
                    // printf("%s %s.\n",chararr[a]->prefix[2],chararr[a]->hints[2]);
                    check = true;
                    break;
                }
            }

            if(check!=true){
                printErr("Invalid talk target. Note: You cannot talk to npcs not in the same room");
            }
        }
        //or look
        else if(strcmp(cmd, "look")==0){
            printf("You are currently in the %s\n", getloc(avatar)->name);
            int h=0;
            //get all characters in the room
            printf("With: ");
            for (size_t i = 0; i < MAX_CHARACTER; i++){
                struct Character *ch = curroom->chara[i];
                if (ch != NULL && strcmp(ch->id, "avatar") != 0) {
                    if (h == 1) {
                        printf(", %s", getcharname(ch));
                    } else {
                        printf("%s", getcharname(ch));
                    }
                    h = 1;
                }
            };
            if(h!=1){
                printf("no one.");
            }
            printf("\n");
            // room items
            printRoomItems(curroom);
            //inventory
            printInventory(avatar);
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
                    printRoomItems(target);
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
                    printRoomItems(target);
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
                    printRoomItems(target);
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
                    printRoomItems(target);
                }
            }
            else{
                printErr("invalid direction\n");
            }
        }
        //if command was inventory
        else if(strcmp(cmd,"inventory")==0){
            printInventory(avatar);
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
                        printErr("sorry, your inventory is full\n");
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
            printInventory(avatar);
            printRoomItems(curroom);
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
            struct Character * clueChar = NULL;
            for(int r = 0; r<9;r++){
                if(strcmp(chararr[r]->name, des) == 0 && strcmp(chararr[r]->id, avatar->id) != 0){
                    clueChar = chararr[r];
                    break;
                }
            }
            if(clueChar == NULL){
                printErr("not a valid character");
            }
            else if(roomCharLength(curroom) >= MAX_CHARACTER){
                printErr("too many characters in this room already");
            }
            //they do input a valid character
            else{ 
                attempts++;
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
                struct Item * inv = avatar->inventory;
                struct Item * roominv = curroom->itemList; 
                //set boolean (find item or not)
                bool temp = false;
                //loop through user inventory and see if item is there
                while(inv!=NULL){
                    if(strcmp(inv->name,targetItem)==0){
                        temp=true;
                        break;
                    }
                    inv = inv->next;
                }
                // //loop through room itemList and see if item is there
                while(roominv!=NULL){
                    if(strcmp(roominv->name,targetItem)==0){
                        temp=true;
                    }
                    roominv = roominv->next;
                }
                //if item is in inventory
                if(temp){
                    printSucc("ITEM MATCH");
                    booitem = true;
                }
                else{
                    printErr("WRONG ITEM");
                }
                // move char
                moveChar(clueChar->location, curroom, clueChar);
                
                // is the target char
                if (strcmp(clueChar->name, targetChar) == 0) {
                    printSucc("CHARACTER MATCH");
                    boochara = true;
                }
                // is inside the room?
                else if (isCharInside(curroom, targetChar)) {
                    printSucc("CHARACTER MATCH");
                    boochara = true;
                } else {
                    printErr("WRONG CHARACTER");
                }
                
                //check if guesses correctly
                if(booroom && booitem && boochara){
                    //breaks gaming loop
                    clue = 11;
                }
            }
        }
        else{
            printErr("Please input a valid command\n");
        }
    }
    #endif
    //check if user gets all three answers correct
    if(clue==12){
        printf("Thank you for playing, see you next time!\n");
    }
    else if(booroom && booitem && boochara){
        puts("    /\\_____/\\");
        puts("   /  o   o  \\");
        puts("  ( ==  v  == )");
        puts("   )         (");
        puts("  (           )");
        puts(" ( (  )   (  ) )");
        puts("(__(__)___(__)__)");
        printSucc("YOU WON!");
        printf("Attempts: %d\n", attempts);
        printf("Press ENTER key to quit the game\n");
        getchar(); // prevent cmd to close when running from exe
    }
    else{
        puts("    /\\_____/\\");
        puts("   /  o   o  \\");
        puts("  ( ==  ^  == )");
        puts("   )         (");
        puts("  (           )");
        puts(" ( (  )   (  ) )");
        puts("(__(__)___(__)__)");
        printErr("YOU LOST");
        printf("Press ENTER key to quit the game\n");
        getchar(); // prevent cmd to close when running from exe
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
    // free(avatar);

    return 0;
}
