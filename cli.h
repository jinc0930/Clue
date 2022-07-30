#define RED     "\x1B[31m"
#define GRN     "\x1B[32m"
#define YEL     "\x1B[33m"
#define BLU     "\x1B[34m"
#define MAG     "\x1B[35m"
#define CYN     "\x1B[36m"
#define WHT     "\x1B[37m"
#define RESET   "\x1B[0m"

#define BLD     "\x1B[1m"
#define UND     "\x1B[4m"
#define UND     "\x1B[4m"

// Send a formatted message and prompt
void prompt(char out[], char*str);

// Send a formatted message with localization and prompt
void promptWithLoc(char out[], char*str, char*loc);

// Describe a command
void describe(char *cmd, char *description);