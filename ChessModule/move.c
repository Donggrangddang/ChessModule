#include "board.h"
#include "legalMove.h"
#include "piece.h"
#include <stdio.h>

/**
* @brief ���ڷ� �Էµ� �������� ���ڷ� �ٲٴ� �Լ�
* @param char file ����
* @param char rank ����
* @return short �ٲ� ����
*/
short convertToIndex(char file, char rank) {
	return (rank - '1') * 8 + (file - 'a');
}

/**
* @brief Board�� �������� �ݿ��ϴ� �Լ� (turnToPlay �ٲٴ� �� main.c�� ��)
* @param Board* b �������� �ݿ��� Board�� �޸� �ּ�
* @param MoveList* l ������ ���������� ���θ� �Ǵ��� MoveList�� �޸� �ּ�
* @param Move move ���� ������
*/
int applyMove(Board* b, MoveList* l, Move move) {
	for (int i = 0; i < l->size; i++) {
		if (move.startSquare == l->movesList[i].startSquare && move.targetSquare == l->movesList[i].targetSquare) {
			b->square[move.targetSquare] = b->square[move.startSquare];
			b->square[move.startSquare] = None;
			return 0;
		}
	}
	printf("illegal move\n");
	return 0;
}