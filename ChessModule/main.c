#include "board.h"
#include "move.h"
#include "legalMove.h"
#include "interface.h"
int main() {
    Board board;

    initializeBoard(&board);    
    MoveList lastMoveList = { 0, 0 };

    while (1) {
        printBoard(&board);
        MoveList moveList = { 0, 0 };
        moveList = generateLegalMoves(&board, lastMoveList);
        printMoveList(&moveList);
        char san;
        getSAN(&san);
        if (san != '\0') {
           applyMove(&board, &moveList, paresSAN(&san));
           MoveList lastMoveList = generateLegalMoves(&board); // 수를 두고 난 후 갈 수 있는 곳을 알아내기(체크 여부를 판단하기 위함)
           board.turnToPlay = !(board.turnToPlay);
        }
    }
    

    return 0;
}