#ifndef board_h
#define board_h

#define BOARD_SIZE 64

typedef struct {
    int square[BOARD_SIZE];
    int turnToPlay; // 1은 백이 할 차례, 0은 흑이 할 차례
    int numMoves;
    int enPassantSquare; // index 값 저장
} Board;

void initializeBoard(Board* b);

void setBoardFromFEN(Board* b, const char* fen);
#endif