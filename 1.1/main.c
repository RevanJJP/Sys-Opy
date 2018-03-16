#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "blockarray.h"

void print_static_array (struct static_array* array);
void print_dynamic_array (struct dynamic_array* array);

int main() {

    return 0;
}

void print_static_array (struct static_array* array) {
    if(array==NULL) {
        printf("Array doesn't exist!");
        return;
    }
    for (int i = 0; i < array->size; i++) {
        printf("%d: %s", i, array->array[i]);
        printf("\n");
    }
}
void print_dynamic_array (struct dynamic_array* array) {
    if(array==NULL) {
        printf("Array doesn't exist!");
        return;
    }
    for (int i = 0; i < array -> size; i++) {
        printf("%d: %s", i, array->array[i]);
        printf("\n");
    }
}
