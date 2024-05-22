#define _CRT_SECURE_NO_WARNINGS

#include "board.h"
#include "interface.h"
#include "legalMove.h"
#include "piece.h"
#include <stdio.h>

char getSAN() {
	char san;
	printf("Enter your Move : ");
	if (scanf(" %c", &san) != 1) { // scanf_s 함수 사용
		printf("Failed to read input.\n");
		return '\0';
	}
	return san;
}

int convertToIndex(char file, char rank) {
	return (rank - '1') * 8 + (file - 'a');
}

Move paresSAN(char* san) {
	Move move;
	
	move.startSquare = convertToIndex(san[0], san[1]);
	move.targetSquare = convertToIndex(san[2], san[3]);
	
	return move;
}

void applyMove(Board* b, MoveList* l, Move move) {
	for (int i = 0; i < l->size; i++) {
		if (move.startSquare == l->movesList[i].startSquare && move.targetSquare == l->movesList[i].targetSquare) {
			b->square[move.targetSquare] = b->square[move.startSquare];
			b->square[move.startSquare] = None;
			break;
		}
	}
}