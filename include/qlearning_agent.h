#ifndef QLEARNING_AGENT_H
#define QLEARNING_AGENT_H

typedef struct {
    int *keys;
    double (*values)[4];
    int table_size;
} QLearningAgent;

QLearningAgent *init_QLearningAgent(const int table_size);

void free_QLearningAgent(QLearningAgent *agent);

#endif // QLEARNING_AGENT_H
