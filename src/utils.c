#include "../include/utils.h"

#include <string.h>
#include <stdlib.h>

void copy(const void *src, void *dst, const size_t size, const size_t len) {
    for (size_t i = 0; i < len; i++) {
        void *a = (unsigned char *)src + i * size;
        void *b = (unsigned char *)dst + i * size;
        for (size_t j = 0; j < size; j++) {
            *((unsigned char *)b + j) = *((unsigned char *)a + j);
        }
    }
}

void set(void *dst, const void *value, const size_t size, const size_t len) {
    for (size_t i = 0; i < len; i++) {
        void *item = (unsigned char *)dst + i * size;
        for (size_t j = 0; j < size; j++) {
            *((unsigned char *)item + j) = *((unsigned char *)value + j);
        }
    }
}

size_t resize(const size_t size, const size_t *dims, const int dim) {
    size_t resized = size;
    for (int i = 1; i < dim; i++) {
        resized *= dims[i];
    }
    return resized;
}

int byte_compare(const void *a, const void *b, const size_t size) {
    int a_last = *((unsigned char *)a + size - 1);
    int b_last = *((unsigned char *)b + size - 1);
    int a_bit = a_last >> 7;
    int b_bit = b_last >> 7;
    if (a_bit > b_bit) { return -1; }
    if (a_bit < b_bit) { return 1; }
    int reverse_compare = (a_bit && a_last != 255);
    for (size_t i = 0; i < size; i++) {
        int x = *((unsigned char *)a + size - 1 - i);
        int y = *((unsigned char *)b + size - 1 - i);
        if (reverse_compare) {
             if (x < y) { return 1; };
             if (x > y) { return -1; }; 
        } else {
             if (x < y) { return -1; };
             if (x > y) { return 1; }; 
        } 
    }
    return 0;
}

int compare(
    const void *a, 
    const void *b, 
    const size_t size, 
    const size_t *dims, 
    const int dim
) {
    if (!dim) { return byte_compare(a, b, size); }
    size_t resized = resize(size, dims, dim);
    for (size_t i = 0; i < dims[0]; i++) {
        void *x = (unsigned char *)a + i * resized;
        void *y = (unsigned char *)b + i * resized;
        int compared = compare(x, y, size, dims + 1, dim - 1);
        if (compared) { return compared; }
    }
    return 0;
}

void *extreme(
    const void *p, 
    const size_t size, 
    const size_t *dims, 
    const int dim,
    const int sign
) {
    size_t resized = resize(size, dims, dim);
    void *m = (unsigned char *)p;
    for (size_t i = 1; i < dims[0]; i++) {
        void *it = (unsigned char *)p + i * resized; 
        if (compare(it, m, size, dims + 1, dim - 1) == sign) {
            m = it;
        }
    }
    return m;
}

void *max(
    const void *p, 
    const size_t size, 
    const size_t *dims, 
    const int dim
) { return extreme(p, size, dims, dim, 1); }

