#ifndef board_h
#define board_h

#define BOARD_SIZE 64

typedef struct {
    short square[BOARD_SIZE];
    short turnToPlay; // 1은 백이 할 차례, 0은 흑이 할 차례
} Board;

void initializeBoard(Board* b);
#endif