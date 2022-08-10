#include <string.h>
#include <stdbool.h>

bool startsWith(const char *prefix, const char *str) {
    return strncmp(prefix, str, strlen(prefix)) == 0;
}

void slice(const char *str, char *result, size_t start, size_t end) {
    strncpy(result, str + start, end - start);
};