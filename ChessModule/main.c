#include <stdio.h>
#include "board.h"
#include "interface.h"
#include "move.h"
int main() {
    Board board;

    initializeBoard(&board);

    while (1) {
        printBoard(&board);
        MoveList moveList = generateLegalMoves(&board);
        char san = getSAN();
        applyMove(&board, &moveList, paresSAN(&san));
    }
    

    return 0;
}