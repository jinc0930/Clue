#include <string.h>
#include "../character.h"
#include "../rooms.h"
#ifndef PRETTYPRINT_H
#define PRETTYPRINT_H
// colors
// #define BLK     "\x1B[30m"
#define RED     "\x1B[31m"
#define GRN     "\x1B[32m"
#define YEL     "\x1B[33m"
#define BLU     "\x1B[34m"
#define MAG     "\x1B[35m"
#define CYN     "\x1B[36m"
#define WHT     "\x1B[37m"
// reset
#define RESET   "\x1B[0m"
// bold
#define BLD     "\x1B[1m"
// underdeline
#define UND     "\x1B[4m"

//bg
#define BLKHB "\e[0;100m"

// print a dimmed text
void printDimmed(const char*str);

// prompt a pretty message and output to out in red
void promptErr(char out[], const char*str);

// prompt a pretty message and output to out
void prompt(char out[], const char*str);

// prompt a pretty message and output to out with some localization
void promptWithLoc(char out[], const char*str, const char*loc);

// describe a command but indented and bold
void describe(const char *cmd, const char *description);

// formatted string cell/room to be used
void cell(char *dest, const char *str);

// finish current row
void endCells();

// print a row with 3 columns
void printCell(const char *col, const char *modifiers);

// print a border for header/footer/in between
void rowBorder();

// print all the rooms
void printMap(struct Room* map[9]);

// debug map for testing purposes
void debugMap(struct Room* map[9]);

// print a new line error
void printErr(const char*str);

// print a new line success
void printSucc(const char*str);

// print all items in the iventory
void printInventory(struct Character* avatar);

// print all items in the room
void printRoomItems(struct Room* room);

// simulate typing
void printTyping(const char *msg);

// print asci clue characters
void printClue();
#endif