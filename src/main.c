#include "../include/helper.h"
#include "../include/random.h"
#include "../include/utils.h"

int main () {
    init_random();
    
    double learning_rate = .3;
    double learning_decay = .9999999;
    double discount_factor = .9;
    double epsilon = .05;
    long episodes = 100000000;
    int key_size = 140000000;
    int grid_size = 10;

    grid_t *grid = init_grid(grid_size);
    snake_t *snake = init_snake(grid_size);
    
    agent_t *agent = load_agent("agent.dat");
    if (!agent) {
        agent = init_agent(key_size);
    }
    
    train_snake(
        snake,
        grid,
        agent, 
        learning_rate,
        learning_decay,
        discount_factor,
        epsilon,
        episodes
    );
    
    save_agent("agent.dat", agent);
    
    free_grid(grid);
    free_snake(snake);
    free_agent(agent);
    return 0;
}

