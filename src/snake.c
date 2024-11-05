#include "../include/snake.h"
#include "../include/random.h"
#include <stdlib.h>
#include <stdio.h>

snake_t *init_snake(const size_t size) {
    snake_t *snake = malloc(sizeof(snake_t));
    if (!snake) { 
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    snake -> body = malloc(size * sizeof(State));
    if (!snake -> body) {
        free(snake);
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    snake -> size = 1;
    snake -> done = 0;
    snake -> body[0] = (State){1, 1};
    return snake;
}

void set_target(snake_t *snake, grid_t *grid) {
    while (1) {
        State state = {randrange(0, grid -> size), randrange(0, grid -> size)};
        if (!grid -> keys[state.x][state.y]) { 
            snake -> target.x = state.x;
            snake -> target.y = state.y;
            grid -> values[state.x][state.y] = BLUE_CUBE;
            return; 
        }
    }
}

void reset_snake(snake_t *snake, grid_t *grid) {
    for (size_t row = 0; row < snake -> size; row++) {
        snake -> body[row] = (State){0, 0};
    }
    snake -> size = 1;
    snake -> done = 0;
    snake -> success = 0;
    snake -> body[0] = (State){1, 1};
    reset_grid(grid);
    set_target(snake, grid);
}

void free_snake(snake_t *snake) {
    free(snake -> body);
    free(snake);
}

void increase_snake(snake_t *snake, grid_t *grid, State next_state) {
    if (snake -> size == grid -> size) {
        snake -> done = 1;
        return;
    }
    snake -> body[snake -> size].x = next_state.x;
    snake -> body[snake -> size].y = next_state.y;
    for (size_t i = snake -> size; i > 0; i--) {
        State temp = snake -> body[i];
        snake -> body[i] = snake -> body[i - 1];
        snake -> body[i - 1] = temp;
    }
    grid -> keys[next_state.x][next_state.y] = 1;
    grid -> values[next_state.x][next_state.y] = RED_CUBE;
    snake -> size++;
}

void decrease_snake(snake_t *snake, grid_t *grid) {
    size_t index = snake -> size - 1;
    State tail = snake -> body[index];
    grid -> keys[tail.x][tail.y] = 0;
    grid -> values[tail.x][tail.y] = BLACK_CUBE;
    snake -> body[index] = (State){0, 0};
    snake -> size--;
}

