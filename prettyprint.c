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

void printErr(char*str) {
    printf(BLD RED "%s\n" RESET, str);
}

void printSucc(char*str) {
    printf(BLD GRN "%s\n" RESET, str);
}

void printDimmed(char*str) {
    printf(BLK "%s", str);
}

void promptWithLoc(char out[], char*str, char*loc) {
    // check if str is empty
    if ((str != NULL) && (str[0] == '\0')) {
        printf("\n" BLD BLU "%s >>> " RESET, loc);
    } else {
        printf("\n" BLK "%s\n" BLD BLU "%s >>> " RESET, str, loc);
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
        strncpy(dest, "                 ", 18);
    }
    size_t len = strnlen(dest, 18);
    for (size_t i = len; i < 18; i++) {
        dest[i] = ' ';
    };
    dest[18] = '\0';
}
void printCell(char *col, char *modifiers) {
    char c[18];
    cell(c, col);
    printf(BLK "|" RESET);
    printf("%s" " %s " RESET, modifiers, c);
}

void endCells() {
    printf(BLK "|" RESET "\n");
}

void rowBreak() {
    printf(BLK "+--------------------+--------------------+--------------------+\n" RESET);
}

// print all the rooms
void printMap(struct Room* map[9]) {
    rowBreak();
    for (size_t i = 0; i < 9; i = i+3) {
        for (size_t j = 0; j < 3; j++) {
            if (isAvatarInside(map[i+j])) printCell(map[i+j]->name, BLD CYN);
            else printCell(map[i+j]->name, BLD);
        }
        endCells();
        for (size_t j = 0; j < 3; j++) {
            for (size_t k = 0; k < 3; k++) {
                struct Character * c = map[i+k]->chara[j];
                if (c != NULL && strcmp(c->id, "avatar") == 0) {
                    printCell(c->name, CYN);
                } else {
                    printCell(c != NULL ? c->name : NULL, "");
                }
            }
            endCells();
        }
        rowBreak();
    }
}