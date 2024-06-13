#ifndef board_h
#define board_h

#define BOARD_SIZE 64

typedef struct {
    int square[BOARD_SIZE];
    int turnToPlay; // 1Àº ¹éÀÌ ÇÒ Â÷·Ê, 0Àº ÈæÀÌ ÇÒ Â÷·Ê
    int numMoves;
} Board;

void initializeBoard(Board* b);

void setBoardFromFEN(Board* b, const char* fen);
#endif