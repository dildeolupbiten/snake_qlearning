#include "../include/utils.h"
#include "../include/random.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

void get_valid_moves(
    State valid_moves[4], 
    const State head, 
    int *count, 
    const int width, 
    const int height, 
    int **grid
) {
    for (int i = 0; i < 4; i++) {
        State next_state = {head.x + MOVES[i].x, head.y + MOVES[i].y};
        if  (
            0 <= next_state.x && next_state.x < width
            && 
            0 <= next_state.y && next_state.y < height
            && 
            grid[next_state.x][next_state.y] == 0
        ) {
            valid_moves[(*count)++] = MOVES[i];           
        }
    }
}

int get_state_key(
    const State food_position, 
    const int size, 
    const State snake_body[size], 
    const int table_size
) {
    unsigned long long state_key = 0;
    unsigned long long m = ((unsigned long long)1 << 32) - 1;
    unsigned long long n = ((unsigned long long)1 << 63) - 1;
    int temp[2] = {0};
    for (int i = 0; i < size + 1; i++) {
        if (i == size) {
            temp[0] = food_position.x;
            temp[1] = food_position.y;
        } else {
            temp[0] = snake_body[i].x;
            temp[1] = snake_body[i].y;
        }
        for (int j = 0; j < 2; j++) {
            state_key ^= temp[j];
            state_key *= m;
            state_key ^= state_key >> 7;
            state_key += m * (2 * i + j);
            state_key %= n;
        }
    }
    int index = state_key % table_size;
    return index;
}

void init_state_key(const int state_key, QLearningAgent *agent) {
    if (!agent -> keys[state_key]) { agent -> keys[state_key] = 1; }
}

double max_q_value(const double value[4]) {
    double m = value[0];
    for (int i = 1; i < 4; i++) { if (m < value[i]) { m = value[i]; } }
    return m;
}

State choose_move(
    SnakeGame *game, 
    QLearningAgent *agent, 
    const double exploration_rate,
    const int width,
    const int height,
    int **grid,
    const int table_size
) {
    if (random_double() < exploration_rate) {
        State valid_moves[4];
        int count = 0;
        get_valid_moves(
            valid_moves, 
            game -> snake_body[0], 
            &count, 
            width, 
            height, 
            grid
        );
        if (!count) {
            game -> done = 1;
            return (State){-1, -1};
        }
        return valid_moves[random_int(count)];
    } else {
        int state_key = get_state_key(
            game -> food_position, 
            game -> size, 
            game -> snake_body,
            table_size
        );
        init_state_key(state_key, agent);
        double *value = agent -> values[state_key];
        double max_q = max_q_value(value);
        State best_moves[4];
        int count = 0;
        for (int i = 0; i < 4; i++) {
            if (value[i] == max_q) { best_moves[count++] = MOVES[i]; }
        }
        return best_moves[random_int(count)];
    }
}

int move_index(const State move) {
    for (int i = 0; i < 4; i++) {
        if (MOVES[i].x == move.x && MOVES[i].y == move.y) { return i; }
    }
    return 0;
}

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
) {
    int current_state_key = get_state_key(
        current_food_position, 
        current_size, 
        current_snake_body,
        table_size
    );
    init_state_key(current_state_key, agent);
    int next_state_key = get_state_key(
        game -> food_position, 
        game -> size, 
        game -> snake_body,
        table_size
    );
    init_state_key(next_state_key, agent);
    int mv_index = move_index(move);
    double current_q = agent -> values[current_state_key][mv_index];
    double max_next_q = max_q_value(agent -> values[next_state_key]);
    double new_q = q_algorithm(
        current_q, 
        max_next_q, 
        reward, 
        learning_rate, 
        discount_factor
    );
    agent -> values[current_state_key][mv_index] = new_q;
}

void augment_snake_body(SnakeGame *game, const State next_state, int **grid) {
    State *temp = realloc(
        game -> snake_body, 
        (game -> size + 1) * sizeof(State)
    );
    if (!temp) { 
        free_SnakeGame(game);
        printf("Memory allocation failed.");
        return;
    };
    temp[game -> size] = next_state;
    for (int i = game -> size; i > 0; i--) {
        State tmp = temp[i];
        temp[i] = temp[i - 1];
        temp[i - 1] = tmp;
    }
    
    game -> snake_body = temp;
    grid[next_state.x][next_state.y] = 1;
    game -> size += 1;
}

void reduce_snake_body(SnakeGame *game, int **grid) {
    State tail = game -> snake_body[game -> size - 1];
    grid[tail.x][tail.y] = 0;
    State *temp = realloc(
        game -> snake_body, 
        (game -> size - 1) * sizeof(State)
    );
    if (!temp) { 
        printf("Memory allocation failed.");
        return;
    };
    game -> snake_body = temp;
    game -> size -= 1;
}

int step(
    const State move, 
    SnakeGame *game,
    const int width, 
    const int height, 
    int **grid
) {
    int reward = 0;
    State next_state = {
        game -> snake_body[0].x + move.x, 
        game -> snake_body[0].y + move.y
    };
    if (
        (0 <= next_state.x && next_state.x < width) 
        && 
        (0 <= next_state.y && next_state.y < height) 
        &&
        (grid[next_state.x][next_state.y] == 0)
    ) {
        augment_snake_body(game, next_state, grid);
        if (
            next_state.x == game -> food_position.x 
            && 
            next_state.y == game -> food_position.y
        ) {
            game -> food_position = place_food(width, height, grid);
            reward += 100;
        } else {
            reduce_snake_body(game, grid);
            reward += -1;
        }
    } else {
        game -> done = 1;
        reward -= 100;
    }
    return reward;
}

void write_CSV(const char *filename, QLearningAgent *agent) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Unable to create file");
        return;
    }
    fprintf(file, "Occupied,Left,Right,Up,Down\n");
    for (int i = 0 ; i < agent -> table_size; i++) {
        fprintf(
            file, 
            "%d,%f,%f,%f,%f\n", 
            agent -> keys[i], 
            agent -> values[i][0], 
            agent -> values[i][1], 
            agent -> values[i][2], 
            agent -> values[i][3]
        );
    }
    fclose(file);
}

void read_CSV(const char *filename, QLearningAgent *agent) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open file");
        return;
    }
    char line[128];
    char *columns[5];
    int nrows = 0;
    int ncols = 0;
    char ***csv = malloc(sizeof(char **));
    int index = 0;
    while (fgets(line, sizeof(line), file)) {
        if (!index) { index = 1; continue; }
        line[strcspn(line, "\n")] = 0;
        ncols = 0;
        char *token = strtok(line, ",");
        while (token != NULL && ncols < 5) {
            columns[ncols++] = token;
            token = strtok(NULL, ",");
        }
        char ***temp = realloc(csv, (nrows + 1) * sizeof(char **));
        if (!temp) {
            printf("Memory allocation failed.\n");
            break;
        }
        temp[nrows] = malloc(ncols * sizeof(char *));
        for (int i = 0; i < ncols; i++) {
            temp[nrows][i] = columns[i];
        }
        csv = temp;
        nrows++;
    }
    if (nrows) {
        load(nrows, ncols, csv, agent);
    }
    free(csv);
    fclose(file);
}

double atod(char *str) {
    char *endptr;
    return strtod(str, &endptr);
}

void load(int nrows, int ncols, char ***csv, QLearningAgent *agent) {
    int *keys = realloc(agent -> keys, nrows * sizeof(int));
    if (!keys) {
        free_QLearningAgent(agent);
        printf("Memory allocation failed.\n");
        return;
    }
    double (*values)[4] = realloc(agent -> values, nrows * sizeof(double[4]));
    if (!values) {
        free(keys);
        free_QLearningAgent(agent);
        printf("Memory allocation failed.\n");
        return;
    }
    for (int i = 0; i < nrows; i++) {
        keys[i] = atoi(csv[i][0]);
        for (int j = 1; j < ncols; j++) {
            values[i][j - 1] = atod(csv[i][j]);
        }
    }
    agent -> keys = keys;
    agent -> values = values;
    agent -> table_size = nrows;
}

char **split(char *str, const char *delimiter) {
    char **splitted = NULL;
    int count = 0;
    char *token = strtok(str, delimiter);
    while (token != NULL) {
        char **temp = realloc(splitted, (count + 1) * sizeof(char *));
        if (!temp) {
            printf("Memory allocation failed.\n");
            for (int i = 0; i < count; i++) {
                free(splitted[i]);
            }
            free(splitted);
            return NULL;
        }
        splitted = temp;
        splitted[count] = strdup(token);
        if (!splitted[count]) {
            printf("Memory allocation failed.\n");
            for (int i = 0; i < count; i++) {
                free(splitted[i]);
            }
            free(splitted);
            return NULL;
        }
        count++;
        token = strtok(NULL, delimiter);
    }
    char **temp = realloc(splitted, (count + 1) * sizeof(char *));
    if (!temp) {
        printf("Memory allocation failed.\n");
        for (int i = 0; i < count; i++) {
            free(splitted[i]);
        }
        free(splitted);
        return NULL;
    }
    splitted = temp;
    splitted[count] = NULL;
    return splitted;
}

