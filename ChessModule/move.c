#define _CRT_SECURE_NO_WARNINGS 
#include "board.h"
#include "legalMove.h"
#include "piece.h"
#include <stdio.h>

short convertToIndex(char file, char rank) {
	return (rank - '1') * 8 + (file - 'a');
}

int applyMove(Board* b, MoveList* l, Move move) {
	for (int i = 0; i < l->size; i++) {
		if (move.startSquare == l->movesList[i].startSquare && move.targetSquare == l->movesList[i].targetSquare) {
			b->square[move.targetSquare] = b->square[move.startSquare];
			b->square[move.startSquare] = None;
			b->turnToPlay = !(b->turnToPlay);
			return 0;
		}
	}
	printf("illegal move\n");
	return 0;
}
// Path: ChessModule/piece.c