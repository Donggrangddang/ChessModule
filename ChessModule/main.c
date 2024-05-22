#include <stdio.h>
#include "piece.h"
#include "board.h"
#include "move.h"

int main() {
    // make board
    Board board;

    // initializeBoard
    initializeBoard(&board);

    // print
    printf("Initialized board:\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%hd ", board.square[i]);
        if ((i + 1) % 8 == 0) {
            printf("\n");
        }
    }

    return 0;
}