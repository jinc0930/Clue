#include <stdio.h>
#include "cli.h"


void prompt(char out[], char*str) {
    printf(GRN "%s\n >>> " RESET BLD RESET, str);
    scanf("%s", out);
}

void promptWithLoc(char out[], char*str, char*loc) {
    printf(BLD "%s\n" BLU "%s >>> " YEL RESET, str, loc);
    scanf("%s", out);
}

void describe(char *cmd, char *description) {
    printf(BLD "   %s: " RESET "%s", cmd, description);
}