#include "rooms.h"
#include "character.h"
#include "items.h"
#include <stdio.h>
#include "prettyprint.h"
#include <time.h>
#define VAL 120
// #define DEBUG_MAP 1;

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
    #if defined DEBUG_MAP
    debugMap(map);
    #else
    rowBreak();
    for (size_t i = 0; i < 9; i = i+3) {
        for (size_t j = 0; j < 3; j++) {
            if (map[i+j]->visited == true) {
                if (isIdInside(map[i+j], "avatar")) {
                    char checkmark[18];
                    sprintf(checkmark,"%s *", map[i+j]->name);
                    printCell(checkmark, BLD CYN);
                } else {
                    char checkmark[18];
                    sprintf(checkmark,"%s *", map[i+j]->name);
                    printCell(checkmark, BLK);
                }
            } else {
                printCell(map[i+j]->name, BLD);
            }
        }
        endCells();
        for (size_t j = 0; j < 3; j++) {
            for (size_t k = 0; k < 3; k++) {
                if (map[i+k]->visited == true) {
                    struct Character * c = map[i+k]->chara[j];
                    if (c != NULL && strcmp(c->id, "avatar") == 0) {
                        printCell(c->name, CYN);
                    } else {
                        printCell(c != NULL ? c->name : NULL, "");
                    }
                } else {
                    if (map[i+k]->chara[j] != NULL) {
                        printCell("******", BLK);
                    } else {
                        printCell(NULL, "");
                    }
                }
            }
            endCells();
        }
        rowBreak();
    }
    #endif
}

void printInventory(struct Character * avatar) {
    struct Item * temp = avatar->inventory;
    if(temp!=NULL) {
        int i = 0;
        printf(BLK "Inventory: " RESET "[");
        while(temp!=NULL) {
            if (i++ == 0) {
                printf(YEL "%s" RESET, temp->name);
            } else {
                printf(", "YEL "%s" RESET, temp->name);
            }
            temp = temp->next;
        }
        printf("]\n");
    } else {
        printf(BLK "Inventory: " RESET "nothing.\n");
    }
}

void printRoomItems(struct Room * room) {
    struct Item * temp = room->itemList;
    if(temp!=NULL) {
        int i = 0;
        printf(BLK "Room items: " RESET "[");
        while(temp!=NULL) {
            if (i++ == 0) {
                printf(YEL "%s" RESET, temp->name);
            } else {
                printf("," YEL " %s" RESET, temp->name);
            }
            temp = temp->next;
        }
        printf("]\n");
    } else {
        printf(BLK "Room items: " RESET "nothing.\n");
    }
}

// 32 milliseconds
struct timespec remaining, request = {0, 32000000};
void printTyping(char *msg) {
    int i = 0;
    while ( msg[i] != '\0') {
        printf("%c", msg[i++]);
        fflush(stdout);
        nanosleep(&request, &remaining);
    };
}

// for testing purposes
void debugMap(struct Room* map[9]) {
    puts(BLD "\nDEBUG MAP:" RESET);
    rowBreak();
    for (size_t i = 0; i < 9; i = i+3) {
        for (size_t j = 0; j < 3; j++) {
            if (map[i+j]->visited == true) {
                if (isIdInside(map[i+j], "avatar")) {
                    char checkmark[18];
                    sprintf(checkmark,"%s *", map[i+j]->name);
                    printCell(checkmark, BLD CYN);
                } else {
                    char checkmark[18];
                    sprintf(checkmark,"%s *", map[i+j]->name);
                    printCell(checkmark, BLK);
                }
            } else {
                printCell(map[i+j]->name, BLD);
            }
        }
        endCells();
        for (size_t j = 0; j < 3; j++) {
            for (size_t k = 0; k < 3; k++) {
                struct Character * c = map[i+k]->chara[j];
                if (c != NULL && strcmp(c->id, "avatar") == 0) {
                    printCell(c->name, CYN);
                } else if (c != NULL && strcmp(c->id, "murderer") == 0){
                    printCell(c->name, RED);
                } else if (c != NULL && strcmp(c->id, "hint giver") == 0){
                    printCell(c->name, GRN);
                } else if (c != NULL && strcmp(c->id, "accuser") == 0){
                    printCell(c->name, YEL);
                } else {
                    printCell(c != NULL ? c->name : NULL, "");
                }
            }
            endCells();
        }
        rowBreak();
    }
}
