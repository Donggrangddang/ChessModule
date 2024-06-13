#include "board.h"
#include "legalMove.h"
#include "piece.h"
#include <stdio.h>


int convertToIndex(char file, char rank) {
	return (rank - '1') * 8 + (file - 'a');
}


char convertToSAN(int square) {
	return (square % 8) + 'a', (square / 8) + '1';
}


int doMove(Board* b, MoveList* l, Move move) {
	for (int i = 0; i < l->size; i++) {
		if ((move.startSquare == l->movesList[i].startSquare) && (move.targetSquare == l->movesList[i].targetSquare)) {
			int piece = b->square[move.targetSquare];
			b->square[move.targetSquare] = b->square[move.startSquare];
			b->square[move.startSquare] = None;
			b->turnToPlay = !(b->turnToPlay);
			b->numMoves++;
			return piece;
		}
	}
	printf("illegal move\n");
	return 0;
}


void undoMove(Board* b, Move lastMove, int piece) {
	b->square[lastMove.startSquare] = b->square[lastMove.targetSquare];
	b->square[lastMove.targetSquare] = piece;
	b->turnToPlay = !(b->turnToPlay);
	b->numMoves--;
}