#include "board.h"
#include "move.h"
#include "legalMove.h"
#include "interface.h"
int main() {
    Board board;

    initializeBoard(&board);    

    while (1) {
        printFEN(&board);
        MoveList moveList = { 0, 0 };
        moveList = generateLegalMoves(&board);
        printMoveList(&moveList);
        char san;
        getSAN(&san);
        if (san != '\0') {
        doMove(&board, &moveList, paresSAN(&san));
        }
        printBoard(&board);
    }
    

    return 0;
}