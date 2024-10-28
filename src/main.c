#include "../include/grid.h"
#include "../include/qlearning_agent.h"
#include "../include/snake_game.h"
#include "../include/utils.h"
#include "../include/random.h"
#include "../include/train.h"
#include <stdlib.h>

int main (int argc, char *argv[]) {
    if (argc != 10) { return 0; }
    double learning_rate = atod(split(argv[1], "=")[1]);
    double discount_factor = atod(split(argv[2], "=")[1]);
    double max_exploration_rate = atod(split(argv[3], "=")[1]);
    double min_exploration_rate = atod(split(argv[4], "=")[1]);
    double exploration_decay = atod(split(argv[5], "=")[1]);
    
    int width = atoi(split(argv[6], "=")[1]);
    int height = atoi(split(argv[7], "=")[1]);
    int episodes = atoi(split(argv[8], "=")[1]);
    int table_size = atoi(split(argv[9], "=")[1]);
    
    init_random();
    int **grid = init_grid(width, height);
    SnakeGame *game = init_SnakeGame();
    QLearningAgent *agent = init_QLearningAgent(0);
    
    read_CSV("model.csv", agent);
    if (!agent -> table_size) {
        free_QLearningAgent(agent);
        agent = init_QLearningAgent(table_size);
    }
    
    train_snake(
        agent, 
        game, 
        learning_rate, 
        discount_factor, 
        max_exploration_rate, 
        min_exploration_rate, 
        exploration_decay, 
        episodes,
        width,
        height,
        grid,
        agent -> table_size
    );
    
    write_CSV("model.csv", agent);
    
    free_grid(grid, width);
    free_SnakeGame(game);
    free_QLearningAgent(agent);
}

