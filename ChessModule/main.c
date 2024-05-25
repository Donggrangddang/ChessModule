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
           MoveList lastMoveList = generateLegalMoves(&board); // ���� �ΰ� �� �� �� �� �ִ� ���� �˾Ƴ���(üũ ���θ� �Ǵ��ϱ� ����)
           board.turnToPlay = !(board.turnToPlay);
        }
    }
    

    return 0;
}