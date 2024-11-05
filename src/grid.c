#include "../include/grid.h"
#include "../include/utils.h"
#include <stdlib.h>

grid_t *init_grid(const size_t size) {
    grid_t *grid = malloc(sizeof(grid_t));
    if (!grid) {
        printf("Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    grid -> keys = malloc(size * sizeof(int *));
    if (!grid -> keys) {
        free(grid);
        printf("Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    grid -> values = malloc(size * sizeof(char **));
    if (!grid -> values) {
        free(grid -> keys);
        free(grid);
        printf("Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    for (size_t row = 0; row < size; row++) {
        grid -> keys[row] = malloc(size * sizeof(int));
        grid -> values[row] = malloc(size * sizeof(char *));
        if (!grid -> keys[row]) {
            for (size_t r = 0; r < row; r++) {
                free(grid -> keys[r]);
            }
            free(grid -> values);
            free(grid -> keys);
            free(grid);
            printf("Memory allocation failed.");
            exit(EXIT_FAILURE);
        }
    }
    grid -> size = size;
    return grid;
}

void reset_grid(grid_t *grid) {
    int value = 0;
    for (size_t row = 0; row < grid -> size; row++) {
        set(grid -> keys[row], &value, sizeof(int), grid -> size);
        for (size_t col = 0; col < grid -> size; col++) {
            grid -> values[row][col] = BLACK_CUBE;
        }
    }
    grid -> result = "";
    grid -> keys[1][1] = 1;
    grid -> values[1][1] = RED_CUBE;
}

void free_grid(grid_t *grid) {
    for (size_t row = 0; row < grid -> size; row++) {
        free(grid -> keys[row]);
        free(grid -> values[row]);
    }
    free(grid -> keys);
    free(grid -> values);
    free(grid);
}

void print_grid(grid_t *grid) {
    printf("%s\n", MOVE_CURSOR_TO_TOP);
    for (size_t row = 0; row < grid -> size; row++) {
        for (size_t col = 0; col < grid -> size; col++) {
            printf("%s ", grid -> values[row][col]);
            if (col == grid -> size - 1 && row == grid -> size / 2) {
                printf("%s", grid -> result);
            }
        }
        printf("\n");
    }
}
