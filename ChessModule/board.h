#ifndef board_h
#define board_h

#define BOARD_SIZE 64

typedef struct {
    int square[BOARD_SIZE];
    int turnToPlay; // 1�� ���� �� ����, 0�� ���� �� ����
} Board;

void initializeBoard(Board* b);
#endif