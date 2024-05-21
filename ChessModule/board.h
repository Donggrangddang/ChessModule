#ifndef board_h
#define board_h

#define BOARD_SIZE 64

typedef struct {
    short square[BOARD_SIZE];
    short turnToPlay;
} Board;

void initializeBoard(struct Board* b);

#endif