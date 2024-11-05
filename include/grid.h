#ifndef GRID_H
#define GRID_H

#include <stdio.h>

typedef struct {
    int **keys;
    char ***values;
    size_t size;
    char *result;
} grid_t;

grid_t *init_grid(const size_t size);

void reset_grid(grid_t *grid);

void free_grid(grid_t *grid);

void print_grid(grid_t *grid);

#endif // GRID_H
