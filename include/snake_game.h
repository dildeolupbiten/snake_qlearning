#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include "consts.h"

typedef struct {
    State *snake_body;
    State food_position;
    int size;
    int done;
} SnakeGame;

SnakeGame *init_SnakeGame();

void free_SnakeGame(SnakeGame *game);

State place_food(const int width, const int height, int **grid);

void reset_SnakeGame(
    SnakeGame *game, 
    const int width, 
    const int height, 
    int **grid
);

#endif // SNAKE_GAME_H
