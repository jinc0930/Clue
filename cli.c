#include <stdio.h>
#include "cli.h"

// prompt a pretty message and output to out
void prompt(char out[], char*str) {
    printf(GRN "%s\n >>> " RESET BLD RESET, str);
    scanf("%s", out);
}

// prompt a pretty message and output to out with some localization
void promptWithLoc(char out[], char*str, char*loc) {
    printf(BLD "%s\n" BLU "%s >>> " YEL RESET, str, loc);
    scanf("%s", out);
}

// describe a command but indented and bold
void describe(char *cmd, char *description) {
    printf(BLD "   %s: " RESET "%s", cmd, description);
}