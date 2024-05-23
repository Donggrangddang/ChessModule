#include "piece.h"
#include "board.h"
#include <stdio.h>


/**
* @brief 숫자로 표시된 기물을 알파벳으로 변경
* @param int piece 기물
* @return 알파벳으로 표기된 기물
*/
char getPieceSymbol(int piece) {
    switch (piece & 7) {
    case King: return (piece & White) ? 'K' : 'k';  
    case Pawn: return (piece & White) ? 'P' : 'p';
    case Knight: return (piece & White) ? 'N' : 'n';
    case Bishop: return (piece & White) ? 'B' : 'b';
    case Rook: return (piece & White) ? 'R' : 'r';
    case Queen: return (piece & White) ? 'Q' : 'q';
    default: return '.';
    }
}

/**
* @brief 보드의 현 상태 출력
* @param Board* b 보드의 메모리 주소
*/
void printBoard(Board * b) {
    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
            int index = rank * 8 + file;
            char piece = getPieceSymbol(b->square[index]);
            printf("%c ", piece);
        }
        printf("\n");
    }
}