#include "../include/agent.h"
#include "../include/utils.h"
#include <stdlib.h>

agent_t *init_agent(const size_t size) {
    agent_t *agent = malloc(sizeof(agent_t));
    if (!agent) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    agent -> keys = malloc(size * sizeof(int));
    if (!agent -> keys) {
        free(agent);
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    agent -> values = malloc(size * sizeof(double[4]));
    if (!agent -> values) {
        free(agent -> keys);
        free(agent);
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    agent -> size = size;
    return agent;
}

void save_agent(const char *filename, agent_t *agent) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Failed to open file");
        return;
    }
    fwrite(&agent -> size, sizeof(size_t), 1, file);
    fwrite(agent -> keys, sizeof(int), agent -> size, file);
    fwrite(agent -> values, sizeof(double[4]), agent -> size, file);
    fclose(file);
}

agent_t *load_agent(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        return NULL;
    }
    size_t size;
    fread(&size, sizeof(size_t), 1, file);
    agent_t *agent = init_agent(size);
    fread(agent -> keys, sizeof(int), size, file);
    fread(agent -> values, sizeof(double[4]), size, file);
    fclose(file);
    return agent;
}

void free_agent(agent_t *agent) {
    free(agent -> keys);
    free(agent -> values);
    free(agent);
}

int get_key(
    const State target, 
    const State *body, 
    const size_t size, 
    agent_t *agent
) {
    unsigned long long key = 0;
    unsigned long long prime_multiplier = ((unsigned long long)1 << 32) - 1;
    unsigned long long modulus = ((unsigned long long)1 << 63) - 1;
    int temp[2] = {0};
    for (size_t i = 0; i < size + 1; i++) {
        if (i == size) {
            temp[0] = target.x;
            temp[1] = target.y;
        } else {
            temp[0] = body[i].x;
            temp[1] = body[i].y;
        }
        for (int j = 0; j < 2; j++) {
            key ^= temp[j];
            key *= prime_multiplier;
            key ^= key >> 7;
            key += prime_multiplier * (2 * i + j);
        }
    }
    key %= modulus;
    return key % agent -> size;
}

void init_key(const int key, agent_t *agent) {
    if (!agent -> keys[key % agent -> size]) {
        agent -> keys[key % agent -> size] = 1;
    }
}

