#ifndef board_h
#define board_h

#define BOARD_SIZE 64

typedef struct {
    short square[BOARD_SIZE];
    short turnToPlay; // 1Àº ¹éÀÌ ÇÒ Â÷·Ê, 0Àº ÈæÀÌ ÇÒ Â÷·Ê
} Board;

void initializeBoard(Board* b);

char getPieceSymbol(int piece);

void printBoard(Board* b);
#endif