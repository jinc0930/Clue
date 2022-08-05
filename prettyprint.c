#include "rooms.h"
#include "character.h"
#include <stdio.h>
#include "prettyprint.h"
#define VAL 120

void prompt(char out[], char*str) {
    printf(CYN "%s\n>>> " RESET, str);
    fgets(out, VAL, stdin);
    // remove trailing newline
    out[strcspn(out, "\n")] = 0;
}

void promptErr(char out[], char*str) {
    printf(RED "%s\n>>> " RESET, str);
    fgets(out, VAL, stdin);
    // remove trailing newline
    out[strcspn(out, "\n")] = 0;
}

void printDimmed(char*str) {
    printf(BLK "%s", str);
}

void promptWithLoc(char out[], char*str, char*loc) {
    // check if loc is empty
    if ((loc != NULL) && (loc[0] == '\0')) {
        printf("\n" BLK "%s\n" BLD BLU ">>> " YEL RESET, str);
    } else {
        printf("\n" BLK "%s\n" BLD BLU "%s >>> " YEL RESET, str, loc);
    }
    fgets(out, VAL, stdin);
    // remove trailing newline
    out[strcspn(out, "\n")] = 0;
}

void describe(char *cmd, char *description) {
    printf(BLD "   %s: " RESET "%s", cmd, description);
}

void cell(char *dest, char *str) {
    if (str != NULL) {
        strncpy(dest, str, 18);
    } else {
        strncpy(" ", str, 1);
    }
    size_t len = strnlen(dest, 18);
    for (size_t i = len; i < 18; i++) {
        dest[i] = ' ';
    };
    dest[18] = '\0';

}

void printRow(char *col1, char *col2, char *col3) {
    char c1[18], c2[18], c3[18];
    cell(c1, col1);
    cell(c2, col2);
    cell(c3, col3);
    printf(BLK "|" RESET " %s "BLK "|" RESET " %s "BLK "|" RESET " %s "BLK "|" RESET "\n", c1, c2, c3);
}

void rowBreak() {
    printf(BLK "+--------------------+--------------------+--------------------+\n" RESET);
}

// print all the rooms
void printMap(struct Room* map[9]) {
    rowBreak();
    for (size_t i = 0; i < 9; i = i+3) {
        printRow(getRoomName(map[i]), getRoomName(map[i+1]), getRoomName(map[i+2]));
        for (size_t j = 0; j < 3; j++)
        {
            printRow(getcharname(map[i]->chara[j]), getcharname(map[i+1]->chara[j]), getcharname(map[i+2]->chara[j]));
        }
        rowBreak();
    }
}