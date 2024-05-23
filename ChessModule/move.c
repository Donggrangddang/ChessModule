#define _CRT_SECURE_NO_WARNINGS 
#include "board.h"
#include "legalMove.h"
#include "piece.h"
#include <stdio.h>

#define BUFFER_SIZE 5

void clearInputBuffer() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

void getSAN(char* buffer) {
	printf("Enter your Move: ");

	if (scanf("%4s", buffer) != 1) { // �ִ� 4���� �Է�
		printf("Failed to read input.\n");
		buffer[0] = '\0'; // ���� �� �� ���ڿ� ��ȯ
	}

	clearInputBuffer(); // ���� ����
}

int convertToIndex(char file, char rank) {
	return (rank - '1') * 8 + (file - 'a');
}

Move paresSAN(char* san) {
	Move move = { convertToIndex(san[0], san[1]), convertToIndex(san[2], san[3]) };
	return move;
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