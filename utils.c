#include <string.h>

int startsWith(const char *prefix, const char *str) {
    if(strncmp(prefix, str, strlen(str)) == 0) return 1;
    return 0;
}

void slice(const char *str, char *result, size_t start, size_t end) {
    strncpy(result, str + start, end - start);
};

void pool() {
    int counters[7] = { 0 };
}