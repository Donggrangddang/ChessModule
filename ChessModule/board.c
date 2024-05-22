#include "board.h"
#include "piece.h"
#include <stdio.h>

/**
* @brief 보드 처음 상태로 초기화
* @param Board* b 보드의 메모리 주소
*/
void initializeBoard(Board* b) {

    b->turnToPlay = 1;

    for (int i = 0; i < BOARD_SIZE; i++) {
        b->square[i] = None;
    }

    b->square[0] = White | Rook;
    b->square[1] = White | Knight;
    b->square[2] = White | Bishop;
    b->square[3] = White | Queen;
    b->square[4] = White | King;
    b->square[5] = White | Bishop;
    b->square[6] = White | Knight;
    b->square[7] = White | Rook;

    for (int i = 8; i < 16; i++) {
        b->square[i] = White | Pawn;
    }

    b->square[56] = Black | Rook;
    b->square[57] = Black | Knight;
    b->square[58] = Black | Bishop;
    b->square[59] = Black | Queen;
    b->square[60] = Black | King;
    b->square[61] = Black | Bishop;
    b->square[62] = Black | Knight;
    b->square[63] = Black | Rook;

    for (int i = 48; i < 56; i++) {
        b->square[i] = Black | Pawn;
    }
}