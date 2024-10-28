#ifndef TRAIN_H
#define TRAIN_H

#include "qlearning_agent.h"
#include "snake_game.h"

void train_snake(
    QLearningAgent *agent, 
    SnakeGame *game, 
    const double learning_rate, 
    const double discount_factor, 
    double max_exploration_rate, 
    const double min_exploration_rate, 
    const double exploration_decay, 
    const int episodes,
    const int width,
    const int height,
    int **grid,
    const int table_size
);

#endif // TRAIN_H
