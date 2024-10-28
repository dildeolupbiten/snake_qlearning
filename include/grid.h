#ifndef GRID_H
#define GRID_H

int **init_grid(const int width, const int height);

void reset_grid(const int width, const int height, int **grid);

void free_grid(int **grid, const int width);

#endif // GRID_H
