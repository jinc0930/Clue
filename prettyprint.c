#include "prettyprint.h"
#include "rooms.h"
#include "character.h"
#include "items.h"
#include <stdio.h>
#include <time.h>
#define VAL 120

void prompt(char out[], const char*str) {
    printf(CYN "%s\n>>> " RESET, str);
    fgets(out, VAL, stdin);
    // remove trailing newline
    out[strcspn(out, "\n")] = 0;
}

void promptErr(char out[], const char*str) {
    printf(RED "%s\n>>> " RESET, str);
    fgets(out, VAL, stdin);
    // remove trailing newline
    out[strcspn(out, "\n")] = 0;
}

void printErr(const char*str) {
    printf(BLD RED "%s\n" RESET, str);
}

void printSucc(const char*str) {
    printf(BLD GRN "%s\n" RESET, str);
}

void promptWithLoc(char out[], const char*str, const char*loc) {
    // check if str is empty
    if ((str != NULL) && (str[0] == '\0')) {
        printf("\n" BLD BLU "%s >>> " RESET, loc);
    } else {
        printf("\n%s\n" BLD BLU "%s >>> " RESET, str, loc);
    }
    fgets(out, VAL, stdin);
    // remove trailing newline
    out[strcspn(out, "\n")] = 0;
}

void describe(const char *cmd, const char *description) {
    printf(BLD "   %s: " RESET "%s", cmd, description);
}

void cell(char *dest, const char *str) {
    if (str != NULL) {
        strncpy(dest, str, 17);
    } else {
        strncpy(dest, "                ", 17);
    }
    size_t len = strnlen(dest, 17);
    for (size_t i = len; i < 17; i++) {
        dest[i] = ' ';
    };
    dest[17] = '\0';
}
void printCell(const char *col, const char *modifiers) {
    char c[18] = {0};
    cell(c, col);
    printf("|");
    printf("%s" " %s " RESET, modifiers, c);
}

void endCells() {
    printf("|\n");
}

void rowBreak() {
    printf("+-------------------+-------------------+-------------------+\n");
}
// print all the rooms
void printMap(struct Room* map[9]) {
    rowBreak();
    for (size_t i = 0; i < 9; i = i+3) {
        for (size_t j = 0; j < 3; j++) {
            if (map[i+j]->visited == true) {
                if (isIdInside(map[i+j], "avatar") == true) {
                    char checkmark[18] = {0};
                    sprintf(checkmark,"%s *", map[i+j]->name);
                    printCell(checkmark, BLD CYN);
                } else {
                    char checkmark[18] = {0};
                    sprintf(checkmark,"%s *", map[i+j]->name);
                    printCell(checkmark, "");
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
                        printCell("******", "");
                    } else {
                        printCell(NULL, "");
                    }
                }
            }
            endCells();
        }
        rowBreak();
    }
}

void printInventory(struct Character * avatar) {
    struct Item * temp = avatar->inventory;
    if(temp!=NULL) {
        int i = 0;
        printf("Inventory: [");
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
        printf("Inventory: nothing.\n");
    }
}

void printRoomItems(struct Room * room) {
    struct Item * temp = room->itemList;
    if(temp!=NULL) {
        int i = 0;
        printf("Room items: [");
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
        printf("Room items: nothing.\n");
    }
}

// 32 milliseconds
struct timespec remaining, request = {0, 30000000};

void printTyping(const char *msg) {
    int i = 0;
    while ( msg[i] != '\0') {
        printf("%c", msg[i++]);
        fflush(stdout);
        nanosleep(&request, &remaining);
    };
}

void printClue() {
    puts(" ______     __         __  __     ______    ");
    puts("/\\  ___\\   /\\ \\       /\\ \\/\\ \\   /\\  ___\\   ");
    puts("\\ \\ \\____  \\ \\ \\____  \\ \\ \\_\\ \\  \\ \\  __\\   ");
    puts(" \\ \\_____\\  \\ \\_____\\  \\ \\_____\\  \\ \\_____\\ ");
    puts("  \\/_____/   \\/_____/   \\/_____/   \\/_____/ ");
    puts("");
}
