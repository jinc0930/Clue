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