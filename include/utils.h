#ifndef UTILS_H
#define UTILS_H

#include "consts.h"
#include "snake_game.h"
#include "qlearning_agent.h"

double q_algorithm(
    const double current_q, 
    const double max_next_q, 
    const int reward, 
    const double learning_rate, 
    const double discount_factor
);

void get_valid_moves(
    State valid_moves[4], 
    const State head, 
    int *count, 
    const int width, 
    const int height, 
    int **grid
);

int get_state_key(
    const State food_position, 
    const int size, 
    const State snake_body[size], 
    const int table_size
);

void init_state_key(const int state_key, QLearningAgent *agent);

double max_q_value(const double value[4]);

State choose_move(
    SnakeGame *game, 
    QLearningAgent *agent, 
    const double exploration_rate,
    const int width,
    const int height,
    int **grid,
    const int table_size
);

int move_index(const State move);

void update_q_value(
    const State current_food_position,
    const int current_size,
    const State current_snake_body[current_size],
    const State move,
    const int reward,
    SnakeGame *game,
    QLearningAgent *agent,
    const double learning_rate,
    const double discount_factor,
    const int table_size
);

void augment_snake_body(SnakeGame *game, const State next_state, int **grid);

void reduce_snake_body(SnakeGame *game, int **grid);

int step(
    const State move, 
    SnakeGame *game,
    const int width, 
    const int height, 
    int **grid
);

void write_CSV(const char *filename, QLearningAgent *agent);

void read_CSV(const char *filename, QLearningAgent *agent);

double atod(const char *str);

void load(
    const int nrows, 
    const int ncols, 
    char ***csv, 
    QLearningAgent *agent
);

char **split(char *str, const char *delimiter);

#endif // UTILS_H
