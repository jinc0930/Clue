#include <stdio.h>
#include "prettyprint.h"
#define VAL 120

void prompt(char out[], char*str) {
    printf(RED "%s\n >>> " RESET, str);
    fgets(out, VAL, stdin);
    // remove trailing newline
    out[strcspn(out, "\n")] = 0;
}

void printDimmed(char*str) {
    printf(BLK "%s", str);
}

void promptWithLoc(char out[], char*str, char*loc) {
    printf("\n" BLK "%s\n" BLD BLU "%s >>> " YEL RESET, str, loc);
    fgets(out, VAL, stdin);
    // remove trailing newline
    out[strcspn(out, "\n")] = 0;
}

void describe(char *cmd, char *description) {
    printf(BLD "   %s: " RESET "%s", cmd, description);
}

const char * cell(char *str) {
    // char* inner = malloc(18*sizeof(char));
    static char inner[18];
    int is_end = 0;
    for (int i = 1; i < 18; i++) {
        if (str[i] == '\0' && is_end == 0 && i < 17) {
            is_end = 1;
            inner[i] = str[i];
        } else {
            inner[i] = ' ';
        }
    }
    return inner;
}

void printRow(char *col1, char *col2, char *col3) {
    printf("|%s|%s|%s|\n", cell(col1), cell(col2), cell(col3));
}

void printHeader() {
    printf("+------------------+-------------------+------------------+\n");
}

// print all the rooms
// void printMap(struct Room* map[9]) {
    
// }