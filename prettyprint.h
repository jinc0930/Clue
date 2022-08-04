#include <string.h>
// colors
#define BLK     "\x1B[30m"
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

// print a dimmed text
void printDimmed(char*str);

// prompt a pretty message and output to out in red
void promptErr(char out[], char*str);

// prompt a pretty message and output to out
void prompt(char out[], char*str);

// prompt a pretty message and output to out with some localization
void promptWithLoc(char out[], char*str, char*loc);

// describe a command but indented and bold
void describe(char *cmd, char *description);

// formatted string cell/room to be used
void cell(char *dest, char *str);

// print a row with 3 columns
void printRow(char *col1, char *col2, char *col3);

// print a border for header/footer/in between
void rowBorder();

// print all the rooms
void printMap(struct Room* map[9]);