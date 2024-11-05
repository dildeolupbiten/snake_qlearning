#include "../include/random.h"
#include <stdlib.h>
#include <time.h>

void init_random() {
    srand(time(NULL));
}

int randrange(const int start, const int end) {
    return start + (rand() % (end - start));
}

