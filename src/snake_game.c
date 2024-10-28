#include "../include/snake_game.h"
#include "../include/random.h"
#include <stdlib.h>
#include <stdio.h>

SnakeGame *init_SnakeGame() {
    SnakeGame *game = malloc(sizeof(SnakeGame));
    if (!game) { 
        printf("Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    return game;
}

void free_SnakeGame(SnakeGame *game) {
    free(game -> snake_body);
    free(game);
}

State place_food(const int width, const int height, int **grid) {
    while (1) {
        State state = {random_int(width), random_int(height)};
        if (!grid[state.x][state.y]) { return state; }
    }
}

void reset_SnakeGame(
    SnakeGame *game, 
    const int width, 
    const int height, 
    int **grid
) {
    free(game -> snake_body);
    State *temp = malloc((width * height - 1) * sizeof(State));
    if (!temp) {
        free(game);
        printf("Memory allocation failed.");
        return;
    }
    temp[0] = (State){1, 1};
    game -> snake_body = temp;
    game -> food_position = place_food(width, height, grid);
    game -> size = 1;
    game -> done = 0;
}

