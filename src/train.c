#include "../include/train.h"
#include "../include/grid.h"
#include "../include/utils.h"
#include <stdlib.h>
#include <stdio.h>

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
) {
    int all = 0;
    for (int episode = 0; episode < episodes; episode++) {
        reset_SnakeGame(game, width, height, grid);
        reset_grid(width, height, grid);
        int total_reward = 0;
        while (!game -> done) {
            
            State move = choose_move(
                game,
                agent, 
                max_exploration_rate, 
                width, 
                height, 
                grid,
                table_size
            );
            
            if (move.x == -1 && move.y == -1) {
                game -> done = 1;
                break;
            }
            State current_food_position = game -> food_position;
            int current_size = game -> size;
            State current_snake_body[current_size];
            for (int i = 0; i < current_size; i++) {
                current_snake_body[i] = game -> snake_body[i];
            }
            
            int reward = step(move, game, width, height, grid);  

            update_q_value(
                current_food_position, 
                current_size, 
                current_snake_body, 
                move, 
                reward, 
                game, 
                agent,
                learning_rate,
                discount_factor,
                table_size
            );

            total_reward += reward;
        }
        if (max_exploration_rate > min_exploration_rate) {
            max_exploration_rate *= exploration_decay;
        }
        all += total_reward;
        printf(
            "Episode: %d, Reward: %d Avg Reward: %f\n", 
            episode, 
            total_reward, 
            (double)all / (episode + 1)
        );
    }
}
