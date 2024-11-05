#ifndef CONSTS_H
#define CONSTS_H

#define CLEAR "\033[2J"
#define MOVE_CURSOR_TO_TOP "\x1B[H"
#define BLACK_CUBE "\033[30m\u25a0\033[0m"
#define RED_CUBE "\033[31m\u25a0\033[0m"
#define BLUE_CUBE "\033[34m\u25a0\033[0m"

typedef struct {
    int x;
    int y;
} State;

extern const State MOVES[4];

#endif // CONSTS_H

