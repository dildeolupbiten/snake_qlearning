#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include "agent.h"

void copy(const void *src, void *dst, const size_t size, const size_t len);

void set(void *dst, const void *value, const size_t size, const size_t len);

size_t resize(const size_t size, const size_t *dims, const int dim);

int byte_compare(const void *a, const void *b, const size_t size);

int compare(
    const void *a, 
    const void *b, 
    const size_t size, 
    const size_t *dims, 
    const int dim
);

void *extreme(
    const void *p, 
    const size_t size, 
    const size_t *dims, 
    const int dim,
    const int sign
);

void *max(
    const void *p, 
    const size_t size, 
    const size_t *dims, 
    const int dim
);

unsigned long long hash(
    const void *p, 
    const size_t size, 
    const size_t *dims, 
    const int dim
);

#endif // UTILS_H

