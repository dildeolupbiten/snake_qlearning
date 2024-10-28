#include "../include/snake_game.h"
#include <stdlib.h>
#include <time.h>

void init_random() {
    srand(time(NULL));
}

int random_int(int n) {
    return rand() % n;
}

double random_double() {
    return (double)rand() / RAND_MAX;
}

