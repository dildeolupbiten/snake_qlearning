#include "../include/grid.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int **init_grid(const int width, const int height) {
    int **grid = malloc(width * sizeof(int *));
    if (!grid) {
        printf("Memory allocation failed.");
        return NULL;
    }
    for (int i = 0; i < width; i++) {
        grid[i] = malloc(height * sizeof(int));
        if (!grid[i]) {
            for (int j = 0; j < i; j++) {
                free(grid[j]);
            }
            free(grid);
            printf("Memory allocation failed.");
            return NULL;
        }
        memset(grid[i], 0, height * sizeof(int));
    }
    return grid;
}

void reset_grid(const int width, const int height, int **grid) {
    for (int i = 0; i < width; i++) {
        memset(grid[i], 0, height * sizeof(int));
    }
    grid[1][1] = 1; // The starting position of the snake
}

void free_grid(int **grid, const int width) {
    for (int i = 0; i < width; i++) {
        free(grid[i]);
    }
    free(grid);
}

