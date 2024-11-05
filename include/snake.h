#ifndef SNAKE_H
#define SNAKE_H

#include "grid.h"
#include "consts.h"

typedef struct {
    State *body;
    State target;
    size_t size;
    int done;
    int success;
} snake_t;

snake_t *init_snake(const size_t size);

void set_target(snake_t *snake, grid_t *grid);

void reset_snake(snake_t *snake, grid_t *grid);

void free_snake(snake_t *snake);

void increase_snake(snake_t *snake, grid_t *grid, State next_state);

void decrease_snake(snake_t *snake, grid_t *grid);

#endif // SNAKE_H
