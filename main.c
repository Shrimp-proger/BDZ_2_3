#include <stdio.h>
#include <stdlib.h>
#include "map_lib.h"

int main() {
    Map* my_map = map_create();

    int value1 = 100;
    int value2 = 200;
    map_insert(my_map, 10, &value1);
    map_insert(my_map, 20, &value2);
    int* found = (int*)map_get(my_map, 10);
    if (found) printf("Key 10: %d\n", *found);

    map_erase(my_map, 10);
    map_destroy(my_map);
    return 0;
}