#include "../include/qlearning_agent.h"
#include <stdlib.h>
#include <stdio.h>

QLearningAgent *init_QLearningAgent(const int table_size) {
    QLearningAgent *agent = malloc(sizeof(QLearningAgent));
    if (!agent) { 
        printf("Memory allocation failed.");
        return NULL;
    }
    agent -> keys = malloc(table_size * sizeof(int));
    if (!agent -> keys) {
        free(agent);
        printf("Memory allocation failed.");
        return NULL;
    }
    agent -> values = malloc(table_size * sizeof(double[4]));
    if (!agent -> values) {
        free(agent -> keys);
        free(agent);
        printf("Memory allocation failed.");
        return NULL;
    }
    agent -> table_size = table_size;
    return agent;
}

void free_QLearningAgent(QLearningAgent *agent) {
    free(agent -> keys);
    free(agent -> values);
    free(agent);
}

