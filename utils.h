#include <string.h>
#ifndef UTILS_H
#define UTILS_H
// check if string starts with prefix and remove the trailing \n
int startsWith(const char *prefix, const char *str);

// get a slice of a string
void slice(const char *str, char *result, size_t start, size_t end);
#endif