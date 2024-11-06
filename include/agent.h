#ifndef AGENT_H
#define AGENT_H

#include "snake.h"

typedef struct {
    int *keys;
    double (*values)[4];
    size_t size;
    size_t count;
} agent_t;

agent_t *init_agent(const size_t size);

void save_agent(const char *filename, agent_t *agent);

agent_t *load_agent(const char *filename);

void free_agent(agent_t *agent);

int get_key(
    const State target, 
    const State *body, 
    const size_t size, 
    agent_t *agent
);

void init_key(const int key, agent_t *agent);

#endif // AGENT_H

