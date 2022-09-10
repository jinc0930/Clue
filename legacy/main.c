#include "prettyprint.h"
#include "../rooms.h"
#include "../utils.h"
#include "../character.h"
#include "../items.h"
#include "../pool.h"
#include "../game.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_LINE 100

int main() {
    printClue();
    // a random seed
    srand(time(NULL));
    struct Game game = makeGame();

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

    // INIT GAME
    int avatarIdx = initGame(&game, avatarname);

    char ingame[500] = {0};
    strcpy(ingame, "");

    for (size_t i = 0; i < 9; i++){
        if (avatarIdx == i) continue;
        if (i != 0) strcat(ingame, ", ");
        strcat(ingame, game.characters[i]->name);
    }

    printMap(game.map);
    printRoomItems(game.avatar->location);

    #ifndef TEST
    while( game.attempts <= 10 ){
        struct Room* curroom = game.avatar->location;

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
                if((strcmp(description,game.characters[a]->name)==0)&&(strcmp(description,game.avatar->name)!=0)&& (strcmp(getloc(game.avatar)->name,getloc(game.characters[a])->name)==0)){
                    // experimental typing
                    char str[100];
                    sprintf(str, "%s %s.\n", game.characters[a]->prefix[0], game.characters[a]->hints[0]);
                    printTyping(str);
                    sprintf(str, "%s %s.\n", game.characters[a]->prefix[1], game.characters[a]->hints[1]);
                    printTyping(str);
                    sprintf(str, "%s %s.\n", game.characters[a]->prefix[2], game.characters[a]->hints[2]);
                    printTyping(str);
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
            printf("You are currently in the %s\n", getloc(game.avatar)->name);
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
            printInventory(game.avatar);
            //print rooms in every direction 
            if (getloc(game.avatar)->North != NULL){
                printf("To your north is %s\n",getloc(game.avatar)->North->name);
            }
            if(getloc(game.avatar)->South != NULL){
                printf("To your south is %s\n", getloc(game.avatar)->South->name);
            }
            if(getloc(game.avatar)->East !=NULL){
                printf("To your east is %s\n", getloc(game.avatar)->East->name);
            }
            if(getloc(game.avatar)->West!=NULL){
                printf("To your west is %s\n", getloc(game.avatar)->West->name);
            }
        }
        //if command was go
        else if(startsWith("go", cmd)){
            // printf("make sure to specify which direction would you like to go? north, south, west, or east?\n");
            char direction[MAX_LINE];
            slice(cmd, direction, strlen("go "), MAX_LINE);
            //cases for each direction
            if(startsWith(direction, "north")){
                if(move(&game, North) == -1){
                    printErr("there is no path in that direction");
                    continue;
                } else game.avatar->location->visited = true;
            }
            else if(startsWith(direction, "south")){
                if(move(&game, South) == -1){
                    printErr("there is no path in that direction");
                    continue;
                } else game.avatar->location->visited = true;
            }
            else if(startsWith(direction,"west")){
                if(move(&game, West) == -1){
                    printErr("there is no path in that direction");
                    continue;
                } else game.avatar->location->visited = true;
            }
            else if(startsWith(direction,"east")){
                if(move(&game, East) == -1){
                    printErr("there is no path in that direction");
                    continue;
                } else game.avatar->location->visited = true;
            } else {
                printErr("invalid direction\n");
                continue;
            }
            printMap(game.map);
            printRoomItems(game.avatar->location);
        }
        //if command was inventory
        else if(strcmp(cmd,"inventory")==0){
            printInventory(game.avatar);
        }
        //take item command
        else if(startsWith("take", cmd)){
            char description[MAX_LINE];
            slice(cmd, description, strlen("take "), MAX_LINE);
            if (strcmp(description, "") == 0) {
                prompt(description, "What item do you like to take?");
            }
            switch (take(&game, description)) {
            case Ok:
                printf(YEL "you took the %s\n", description);
                break;
            case Full:
                printErr("sorry, your inventory is full\n");
                break;
            case NotFound:
                printErr("sorry, there is nothing in this room");
                break;
            case Invalid:
                printErr("that item does not exist");
                break;
            };
        }
        //if command was drop
        else if(startsWith("drop", cmd)){
            char description[MAX_LINE];
            slice(cmd, description, strlen("take "), MAX_LINE);
            if (strcmp(description, "") == 0) {
                prompt(description, "What item do you like to drop?");
            }
            switch (drop(&game, description)) {
            case Ok:
                printf(YEL "you dropped the %s\n" RESET, description);
                break;
            case NotFound:
                printErr("you do not carry this item");
                break;
            case Invalid:
                printErr("that item does not exist");
                break;
            default:
                printErr("that item does not exist");
                break;
            };
        }
        else if(strcmp(cmd,"quit")==0){
            game.attempts = 12;
        }
        //or game.map
        else if(strcmp(cmd,"map")==0){
            printMap(game.map);
            printInventory(game.avatar);
            printRoomItems(curroom);
        }
        //if command is clue
        else if(startsWith("clue", cmd)){
            char des[MAX_LINE];
            slice(cmd, des, strlen("clue "), MAX_LINE);
            if (strcmp(des, "") == 0) {
                prompt(des, "Who is the murderer");
            }
            switch (clue(&game, des)) {
            case Full:
                printErr("too many characters in this room already");
                continue;
                break;
            case Invalid:
                printErr("not a valid character");
                break;
            default:
                printErr("not a valid character");
                break;
            };
            
            if (game.okItem == true) printSucc("ITEM MATCH");
            else printErr("WRONG ITEM");
            if (game.okRoom == true) printSucc("ROOM MATCH");
            else printErr("WRONG ROOM");
            if (game.okChar == true) printSucc("CHARACTER MATCH");
            else printErr("WRONG CHARACTER");
            
            if (game.okChar == true && game.okItem == true && game.okRoom == true) {
                //breaks gaming loop
                game.attempts = 11;
            }
        }
        else{
            printErr("Please input a valid command\n");
        }
    }
    #endif
    //check if user gets all three answers correct
    if(game.attempts==12){
        printf("Thank you for playing, see you next time!\n");
    }
    else if(game.okChar == true && game.okItem == true && game.okRoom == true){
        puts("    /\\_____/\\");
        puts("   /  o   o  \\");
        puts("  ( ==  v  == )");
        puts("   )         (");
        puts("  (           )");
        puts(" ( (  )   (  ) )");
        puts("(__(__)___(__)__)");
        puts("https://www.youtube.com/watch?v=PHgc8Q6qTjc");
        printSucc("YOU WON!");
        printf("Attempts: %d\n", game.attempts);
        printf("The correct answer was %s, %s, %s\n", game.targetRoom, game.targetChar, game.targetItem);
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
        puts("https://www.youtube.com/watch?v=PS_cV18z67Y");
        printErr("YOU LOST");
        printf("The correct answer was %s, %s, %s\n", game.targetRoom, game.targetChar, game.targetItem);
        printf("Press ENTER key to quit the game\n");
        getchar(); // prevent cmd to close when running from exe
    }
    
    freeGame(&game);
    return 0;
}