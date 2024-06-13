#include "board.h"
#include "move.h"
#include "legalMove.h"
#include "interface.h"
int main() {
    Board board;
    const char* fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w - - 0 1";
    const char* doubleCheckFen = "1rbk4/5p1p/p6p/1qp5/8/P5P1/1PPB1P1P/2KRR3 w - - 0 1";

    setBoardFromFEN(&board, doubleCheckFen);
    MoveList lastMoveList = { 0, 0 };

    while (1) {
        printFEN(&board);
        printBoard(&board);
        MoveList moveList = { 0, 0 };
        moveList = generateLegalMoves(&board);
        printMoveList(&moveList);
        char san;
        getSAN(&san);
        if (san != '\0') {
        doMove(&board, &moveList, paresSAN(&san));
        }
    }
    

    return 0;
}