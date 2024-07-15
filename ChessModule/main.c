#include "board.h"
#include "move.h"
#include "legalMove.h"
#include "interface.h"
int main() {
    Board board;
    const char* startingFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w - - 0 1";
    const char* doubleCheckFen = "1rbk4/5p1p/p6p/1qp5/8/P5P1/1PPB1P1P/2KRR3 w - - 0 1";

    setBoardFromFEN(&board, startingFen);
    MoveList lastMoveList = { 0, 0 };

    while (1) {
        printFEN(&board);
        printBoard(&board);
        MoveList moveList = { 0, 0 };
        moveList = generateLegalMoves(&board);
        printMoveList(&moveList);
        char san[6];
        getSAN(san);
        if (san != '\0') {
            doMove(&board, &moveList, parseSAN(&san));
        }
        int mate = isMate(&board);
        if (mate == 1) {
            printBoard(&board);
            printCheckMate(&board);
            break;
        }
        else if (mate == 2) {
            printBoard(&board);
            printStaleMate(&board);
            break;
        }
    }
    

    return 0;
}