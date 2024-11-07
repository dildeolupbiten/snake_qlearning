#include "../include/utils.h"
#include "../include/helper.h"
#include "../include/random.h"
#include <stdlib.h>
#include <unistd.h>

int get_valid_moves(State valid_moves[4], snake_t *snake, grid_t *grid) {
    int len = 0;
    State next_state;
    for (int i = 0; i < 4; i++) {
        next_state.x = snake -> body[0].x + MOVES[i].x;
        next_state.y = snake -> body[0].y + MOVES[i].y;
        if (
            0 <= next_state.x && next_state.x < (int)grid -> size
            &&
            0 <= next_state.y && next_state.y < (int)grid -> size
            &&
            grid -> keys[next_state.x][next_state.y] == 0
        ) {
            valid_moves[len++] = MOVES[i];
        }
    }
    return len;
}

void choose_move(
    State *move, 
    snake_t *snake, 
    grid_t *grid, 
    agent_t *agent,
    const double epsilon
) {
    State random_move;
    if (uniform() < epsilon) {
        State valid_moves[4];
        int len = get_valid_moves(valid_moves, snake, grid);
        if (!len) {
            snake -> done = 1;
            move -> x = -1;
            move -> y = -1;
            return;
        }
        random_move = valid_moves[randrange(0, len)];
        move -> x = random_move.x;
        move -> y = random_move.y;
    } else {
        int key = get_key(snake -> target, snake -> body, snake -> size, agent);
        init_key(key, agent);
        size_t size = sizeof(double);
        size_t dims[1] = {4};
        double max_q = *((double *)max(agent -> values[key], size, dims, 1));
        State best_moves[4];
        int len = 0;
        for (int i = 0; i < 4; i++) {
            if (agent -> values[key][i] == max_q) {
                best_moves[len++] = MOVES[i];
            }
        }
        random_move = best_moves[randrange(0, len)];
        move -> x = random_move.x;
        move -> y = random_move.y;
    }
}

int penalty_for_nearby_obstacles(snake_t *snake, grid_t *grid) {
    int x = snake -> body[0].x;
    int y = snake -> body[0].y;
    int penalty = 0;
    if (x > 0 && grid -> keys[x - 1][y] != 0) { 
        penalty -= 10; 
    }
    if (x < (int)grid -> size - 1 && grid -> keys[x + 1][y] != 0) {
        penalty -= 10;
    }
    if (y > 0 && grid -> keys[x][y - 1] != 0) {
        penalty -= 10;
    }
    if (y < (int)grid -> size - 1 && grid -> keys[x][y + 1] != 0) {
        penalty -= 10;
    }
    return penalty;
}

int bonus_for_closeness_to_target(snake_t *snake) {
    int x = abs(snake -> body[0].x - snake -> target.x);
    int y = abs(snake -> body[0].y - snake -> target.y);
    int distance = x + y;
    if (distance == 1) {
        return 10;
    } else if (distance == 2) {
        return 5;
    }
    return 0;
}

int penalty_for_long_distance(State next_state, snake_t *snake) {
    int old_distance = 0;
    old_distance += abs(snake -> body[0].x - snake -> target.x);
    old_distance += abs(snake -> body[0].y - snake -> target.y);
    int new_distance = 0;
    new_distance += abs(next_state.x - snake -> target.x);
    new_distance += abs(next_state.y - snake -> target.y);
    if (new_distance < old_distance) {
        return 10;
    } else {
        return -10;
    }
}

int step(const State move, snake_t *snake, grid_t *grid) {
    State next_state = {
        move.x + snake -> body[0].x, 
        move.y + snake -> body[0].y
    };
    int reward = 0;
    reward += penalty_for_long_distance(next_state, snake);
    if (
        0 <= next_state.x && next_state.x < (int)grid -> size
        && 
        0 <= next_state.y && next_state.y < (int)grid -> size
        &&
        grid -> keys[next_state.x][next_state.y] == 0
    ) {
        increase_snake(snake, grid, next_state);
        if (
            snake -> body[0].x == snake -> target.x 
            && 
            snake -> body[0].y == snake -> target.y 
        ) {
            grid -> values[snake -> target.x][snake -> target.y] = BLACK_CUBE;
            set_target(snake, grid);
            snake -> success += 1;
            reward += 300;
        } else {
            decrease_snake(snake, grid);
            snake -> success = 0;
            reward += -5;
        }
        reward += penalty_for_nearby_obstacles(snake, grid);
        reward += bonus_for_closeness_to_target(snake);
    } else {
        reward -= 200;
        snake -> done = 1;
    }
    return reward;
}

double q_algorithm(
    const double current_q,
    const double max_next_q,
    const int reward,
    const double learning_rate,
    const double discount_factor
) {
    double next_q = reward + discount_factor * max_next_q;
    return (1 - learning_rate) * current_q + learning_rate * next_q;
}

int move_index(const State move) {
    int index = -1;
    for (int i = 0; i < 4; i++) {
        if (move.x == MOVES[i].x && move.y == MOVES[i].y) {
            index = i;
        }
    }
    return index;
}

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
) {
    int current_key = get_key(
        current_target, 
        current_body, 
        current_size, 
        agent
    );
    init_key(current_key, agent);
    int next_key = get_key(
        snake -> target, 
        snake -> body, 
        snake -> size, 
        agent
    );
    init_key(next_key, agent);
    int index = move_index(move);
    double max_next_q = *((double *)max(
        agent -> values[next_key], 
        sizeof(double), 
        (size_t[]){4}, 
        1
    ));
    double current_q = agent -> values[current_key][index];
    double new_q = q_algorithm(
        current_q, 
        max_next_q, 
        reward, 
        learning_rate, 
        discount_factor
    );
    agent -> values[current_key][index] = new_q;
}

char *get_result(
    const double avg_success,
    const double avg_reward,
    const double learning_rate,
    const int episode
) {
    char *result;
    int length = snprintf(
        NULL, 
        0, 
        "%s%d %s%.2f %s%.2f %s%.2f", 
        "Episode: ",
        episode + 1,
        "Avg. success: ",
        avg_success,
        "Avg. reward: ",
        avg_reward,
        "Learning rate: ",
        learning_rate
    );
    length += 1;
    result = (char *)malloc(length);
    if (result != NULL) {
        snprintf(
            result, 
            length, 
            "%s%d %s%.2f %s%.2f %s%.2f", 
            "Episode: ",
            episode + 1,
            "Avg. success: ",
            avg_success,
            "Avg. reward: ",
            avg_reward,
            "Learning rate: ",
            learning_rate
        );
    } else {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    return result;
}

void train_snake(
    snake_t *snake,
    grid_t *grid,
    agent_t *agent, 
    double learning_rate,
    const double learning_decay,
    const double discount_factor,
    const double epsilon,
    const int episodes
) {
    printf("%s\n", CLEAR);
    long avg_reward = 0;
    long avg_success = 0;
    for (long episode = 0; episode < episodes; episode++) {
        reset_snake(snake, grid);
        int total_reward = 0;
        int total_success = 0;
        State move = {0, 0};
        while (!snake -> done) {
            int reward = 0;
            choose_move(&move, snake, grid, agent, epsilon);
            if (move.x == -1 && move.y == -1) {
                snake -> done = 1;
                reward -= 100;
            }
            State current_target = snake -> target;
            State current_body[snake -> size];
            copy(snake -> body, current_body, sizeof(State), snake -> size);
            size_t current_size = snake -> size;
            reward += step(move, snake, grid);
            update_q_value(
                current_target,
                current_body,
                current_size,
                snake,
                agent,
                move,
                reward,
                learning_rate,
                discount_factor
            );
            total_reward += reward;
            total_success += snake -> success;
            if (episode >= episodes - 100) {
                grid -> result = get_result(
                    total_success,
                    total_reward,
                    learning_rate,
                    episode                
                );
                print_grid(grid);
                usleep(100000);
            }
        }
        avg_success += total_success;
        avg_reward += total_reward;
        printf(
            "%s%ld, %s%f, %s%f, %s%f, %s%f\n", 
            "Episode: ",
            episode,
            "Key / Episode: ",
            (double)agent -> count / episode,
            "Avg. success: ",
            (double)avg_success / (episode + 1),
            "Avg. reward: ",
            (double)avg_reward / (episode + 1),
            "Learning rate: ",
            learning_rate
        );
        learning_rate *= learning_decay;
    }
}

