#ifndef HELPER_H
#define HELPER_H

#include "grid.h"
#include "snake.h"
#include "agent.h"

int get_valid_moves(State valid_moves[4], snake_t *snake, grid_t *grid);

void choose_move(
    State *move, 
    snake_t *snake, 
    grid_t *grid, 
    agent_t *agent,
    const double epsilon
);

int penalty_for_nearby_obstacles(snake_t *snake, grid_t *grid);

int bonus_for_closeness_to_target(snake_t *snake);

int penalty_for_long_distance(State next_state, snake_t *snake);

int step(const State move, snake_t *snake, grid_t *grid);

double q_algorithm(
    const double current_q,
    const double max_next_q,
    const int reward,
    const double learning_rate,
    const double discount_factor
);

int move_index(const State move);

void update_q_value(
    const State current_target,
    const State *current_body,
    const size_t current_size,
    snake_t *snake,
    agent_t *agent,
    const State move,
    const int reward,
    const double learning_rate,
    const double discount_factor
);

char *get_result(
    const double avg_success,
    const double avg_reward,
    const double learning_rate,
    const int episode
);

void train_snake(
    snake_t *snake,
    grid_t *grid,
    agent_t *agent, 
    double learning_rate,
    const double learning_decay,
    const double discount_factor,
    const double epsilon,
    const int episodes
);

#endif // HELPER_H
