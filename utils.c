#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

bool startsWith(const char *prefix, const char *str) {
    bool starts = false;
    if (strncmp(prefix, str, strlen(prefix)) == 0) starts = true;
    return starts;
}

void slice(const char *str, char *result, size_t start, size_t end) {
    strncpy(result, str + start, end - start);
};

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void shuffle(int arr[], int n ){
    for (int i = n-1; i > 0; i--) {
        int j = rand() % (i+1);
        swap(&arr[i], &arr[j]);
    }
}