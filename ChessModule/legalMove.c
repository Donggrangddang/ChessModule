#include "legalMove.h"
#include "piece.h"
#include "board.h"
#include "move.h"
#include "malloc.h"
#include <stdio.h>
#include <stdlib.h>

const int KnightDirectionOffsets[8][2] = {
		{-2, -1}, {-2, 1}, {2, -1}, {2, 1},
		{-1, -2}, {-1, 2}, {1, -2}, {1, 2}
};
const int DirectionOffsets[8] = {8, -8, -1, 1, 7, -7, 9, -9}; // �� �� �� ��

/**
* @brief ������ �������� list�� �߰�
* @param MoveList* list MoveList�� �޸� �ּ�
* @param Move move ������ ��
*/
void addLegalMove(MoveList* list, Move move) {
	list->movesList[list->size++] = move;
};


void initMove(Move* m, int startSquare, int targetSquare) {
	m->startSquare = startSquare;
	m->targetSquare = targetSquare;
}

/**
* @brief AttackMap�� ����� �Լ�
* @param Board* b Board�� �޸� �ּ�
* @param int color �����ϴ� ��
* @return AttackMap �����ϰ� �ִ� ��� ĭ�� �迭
*/
int* generateAttackMap(Board* b, int color) {
	if (b == NULL) {
		return NULL;  // ���尡 NULL�̸� ��� NULL�� ��ȯ
	}

	MoveList legalMoveList = { .movesList = { {0} }, .size = 0 };

	for (int startSquare = 0; startSquare < BOARD_SIZE; startSquare++) {
		int piece = b->square[startSquare];

		if (isColor(piece, color)) {
			if (isKnight(piece)) {
				generateKnightMoves(startSquare, &legalMoveList, b, !(b->turnToPlay));
			}
			else if (!isKing(piece)) {
				if (isStraightPiece(piece)) {
					generateStraightMoves(startSquare, &legalMoveList, b, !(b->turnToPlay));
				}
				if (isSlidingPiece(piece)) {
					generateSlidingMoves(startSquare, &legalMoveList, b, !(b->turnToPlay));
				}
				if (isPawn(piece)) {
					generatePawnAttackMoves(startSquare, &legalMoveList, b, !(b->turnToPlay));
				}
			}
			else {
				generateKingAttackMoves(startSquare, &legalMoveList, b, !(b->turnToPlay));
			}
		}
	}

	printf("Legal move list size: %d\n", legalMoveList.size);
	for (int i = 0; i < legalMoveList.size; i++) {
		printf("Move %d: %d\n", i, legalMoveList.movesList[i].targetSquare);
	}

	int* attackMap = (int*)malloc((BOARD_SIZE + 1) * sizeof(int));
	if (attackMap == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		return NULL;  // �޸� �Ҵ� ���� �� NULL�� ��ȯ
	}

	int attackMapSize = 0;

	for (int i = 0; i < legalMoveList.size; i++) {
		int targetSquare = legalMoveList.movesList[i].targetSquare;

		// �ߺ� Ȯ��
		int isDuplicate = 0;
		for (int j = 0; j < attackMapSize; j++) {
			if (attackMap[j] == targetSquare) {
				isDuplicate = 1;
				break;
			}
		}

		// �ߺ��� �ƴϸ� �߰�
		if (!isDuplicate) {
			attackMap[attackMapSize++] = targetSquare;
		}
	}

	printf("Attack map size: %d\n", attackMapSize);
	for (int i = 0; i < attackMapSize; i++) {
		printf("Attack map [%d] = %d\n", i, attackMap[i]);
	}

	attackMap[BOARD_SIZE] = attackMapSize;
	return attackMap;
}


MoveList generateLegalMoves(Board* b) {

	int* attackMap = generateAttackMap(b, !(b->turnToPlay));
	Move move = { 0, 0 };
	MoveList checkedLegalMoveList = { move, 0 };

	if (isChecked(b, attackMap)) {
		int kingSquare = 0;
		for (int i = 0; i < BOARD_SIZE; i++) {
			if (b->square[i] == (King | b->turnToPlay)) {
				kingSquare = i;
				break;
			}
		}
		for (int i = 0; i < 8; i++) {
			int targetSquare = DirectionOffsets[i] + kingSquare;
			for (int j = 0; j < attackMap[63]; j++) {
				if (targetSquare == attackMap[j]) {
					break;
				}
				if (j == attackMap[63] - 1) {
					Move move = { kingSquare, targetSquare };
					addLegalMove(&checkedLegalMoveList, move);
				}
			}
		}
		return checkedLegalMoveList;
	}

	MoveList legalMoveList = {move , 0 };

	for (int startSquare = 0; startSquare < BOARD_SIZE; startSquare++) {

		int piece = b->square[startSquare];

		if (isColor(piece, b->turnToPlay)) {

			if (isKnight(piece)) { // ����Ʈ
				generateKnightMoves(startSquare, &legalMoveList, b, b->turnToPlay);
			}
			else if (isKing(piece) == FALSE) {
				if (isStraightPiece(piece)) { // ��, ��
					generateStraightMoves(startSquare, &legalMoveList, b, b->turnToPlay);
				}
				if (isSlidingPiece(piece)) { // ���, ��
					generateSlidingMoves(startSquare, &legalMoveList, b, b->turnToPlay);
				}
				if (isPawn(piece)) { // ��
					generatePawnMoves(startSquare, &legalMoveList, b, b->turnToPlay);
				}
			}
			else if (isKing(piece) == TRUE) { // ŷ
				generateKingMoves(startSquare, &legalMoveList, b, b->turnToPlay, attackMap);
			}
		}
	}

	free(attackMap);

	return legalMoveList;
}

/**
* @brief �밢�� ������(���)�� ����� �ִ� �Լ�
* @param int startSquare : ������ �⹰�� ��ġ�� Square
* @param Movelist *l : �� �������� �߰��� Movelist�� �޸� �ּ�
* @param Board *b : ������ �⹰�� ã�� Board�� �޸� �ּ�
*20250517
*/
void generateSlidingMoves(int startSquare, MoveList* l, Board* b, int color) {

	for (int i = 4; i < 8; i++) {
		int targetSquare = startSquare;
		targetSquare += DirectionOffsets[i];
		while (targetSquare >= 0 && targetSquare < BOARD_SIZE) {
			if (targetSquare % 8 == 0 || targetSquare % 8 == 7 || targetSquare / 8 == 0 || targetSquare / 8 == 7) {
				if (b->square[targetSquare] == None) { // �� �� �ִ� ĭ�� �����
					Move move = { startSquare, targetSquare };
					addLegalMove(l, move);
				}
				else if (isColor(b->square[targetSquare], color) == FALSE) { // �� �� �ִ� ĭ�� ��� �⹰�� �ִ�
					Move move = { startSquare, targetSquare };
					addLegalMove(l, move);
				}
				break; // �����ڸ��� �����ϸ� ������ ����
			}
			if (b->square[targetSquare] == 0) { // �� �� �ִ� ĭ�� �����
				Move move = {startSquare, targetSquare};
				addLegalMove(l, move);
				targetSquare += DirectionOffsets[i];
			}
			else if (isColor(b->square[targetSquare], color) == FALSE) { // �� �� �ִ� ĭ�� ��� �⹰�� �ִ�
				Move move = {startSquare, targetSquare};
				addLegalMove(l, move);
				break;
			}
			else {
				break;
			}
		}
	}
}

/**
* @brief ���� ������(��)�� ����� �ִ� �Լ�
* @param int startSquare : ������ �⹰�� ��ġ�� Square
* @param Movelist *l : �� �������� �߰��� Movelist�� �޸� �ּ�
* @param Board *b : ������ �⹰�� ã�� Board�� �޸� �ּ�
*20250517
*/
void generateStraightMoves(int startSquare, MoveList* l, Board* b, int color) {

	for (int i = 0; i < 4; i++) {
		int targetSquare = startSquare;
		targetSquare += DirectionOffsets[i];
		while (targetSquare >= 0 && targetSquare < BOARD_SIZE) {
			if (targetSquare % 8 == 0 || targetSquare % 8 == 7 || targetSquare / 8 == 0 || targetSquare / 8 == 7) {
				if (b->square[targetSquare] == None) { // �� �� �ִ� ĭ�� �����
					Move move = { startSquare, targetSquare };
					addLegalMove(l, move);
				}
				else if (isColor(b->square[targetSquare], color) == FALSE) { // �� �� �ִ� ĭ�� ��� �⹰�� �ִ�
					Move move = { startSquare, targetSquare };
					addLegalMove(l, move);
				}
				break; // �����ڸ��� �����ϸ� ������ ����
			}
			if (b->square[targetSquare] == None) { // �� �� �ִ� ĭ�� �����
				Move move = { startSquare, targetSquare };
				addLegalMove(l, move);
				targetSquare += DirectionOffsets[i];
			}
			else if (isColor(b->square[targetSquare], color) == FALSE) { // �� �� �ִ� ĭ�� ��� �⹰�� �ִ�
				Move move = {startSquare, targetSquare};
				addLegalMove(l, move);
				break;
			}
			else {
				break;
			}
		}
	}
}

/**
* @brief ����Ʈ�� �������� ����� �ִ� �Լ�
* @param int startSquare : ������ �⹰�� ��ġ�� Square
* @param Movelist *l : �� �������� �߰��� Movelist�� �޸� �ּ�
* @param Board *b : ������ �⹰�� ã�� Board�� �޸� �ּ�
*20250517
*/
void generateKnightMoves(int startSquare, MoveList* l, Board* b, int color) {

	for (int i = 0; i < 8; i++) {
		int newRow = startSquare / 8 + KnightDirectionOffsets[i][0];
		int newCol = startSquare % 8 + KnightDirectionOffsets[i][1];

		// ���ο� ��ġ�� ���� �ȿ� �ִ��� Ȯ��
		if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
			int targetSquare = newRow * 8 + newCol;
			if ((b->square[targetSquare] == None) || (isColor(b->square[targetSquare], color) == FALSE)) {
				Move move = { startSquare, targetSquare };
				addLegalMove(l, move);
			}
		}
	}
}

/**
* @brief ŷ�� �������� ����� �ִ� �Լ�(üũ ���)
* @param int startSquare : ������ �⹰�� ��ġ�� Square
* @param Movelist *l : �� �������� �߰��� Movelist�� �޸� �ּ�
* @param Board *b : ������ �⹰�� ã�� Board�� �޸� �ּ�
*20250517
*/
void generateKingMoves(int startSquare, MoveList* l, Board* b, int color, int* attackMap) {
	for (int i = 0; i < 8; i++) {
		int targetSquare = startSquare + DirectionOffsets[i];
		// ���� �� �Ǵ� ������ ������� Ȯ��
		if (abs((targetSquare / 8) - (startSquare / 8)) > 1 || abs((targetSquare % 8) - (startSquare % 8)) > 1) {
			continue;
		}
		// ������ ��踦 �Ѿ���� Ȯ��
		if (targetSquare >= 0 && targetSquare < BOARD_SIZE) {
			if (isChecked(b, attackMap) == FALSE) {
				if (b->square[targetSquare] == None) { // �� �� �ִ� ĭ�� �����
					Move move = { startSquare, targetSquare };
					addLegalMove(l, move);
				}
				else if (isColor(b->square[targetSquare], color) == FALSE) { // �� �� �ִ� ĭ�� ��� �⹰�� �ִ�
					Move move = { startSquare, targetSquare };
					addLegalMove(l, move);
				}
			}
		}
	}
}


void generateKingAttackMoves(int startSquare, MoveList* l, Board* b, int color) {
	for (int i = 0; i < 8; i++) {
		int targetSquare = startSquare + DirectionOffsets[i];
		// ���� �� �Ǵ� ������ ������� Ȯ��
		if (abs((targetSquare / 8) - (startSquare / 8)) > 1 || abs((targetSquare % 8) - (startSquare % 8)) > 1) {
			continue;
		}
		// ������ ��踦 �Ѿ���� Ȯ��
		if (targetSquare >= 0 && targetSquare < BOARD_SIZE) {
			Move move = { startSquare, targetSquare };
			addLegalMove(l, move);
		}
	}
}


void generatePawnMoves(int startSquare, MoveList* l, Board* b, int color) {
	// ���� ������ �� �ִ��� Ȯ��
	if (b->square[startSquare - 8 + 16 * color] == None) {
		// ù �̵� �� �� ĭ ���� ���� ���� Ȯ��
		if ((48 - 40 * color <= startSquare) && (startSquare <= 55 - 40 * color) &&
			(b->square[startSquare - 16 + 32 * color] == None)) {
			Move move = { startSquare, startSquare - 16 + 32 * color };
			addLegalMove(l, move);
		}
		// �� ĭ ����
		Move move = { startSquare, startSquare - 8 + 16 * color };
		addLegalMove(l, move);
	}

	// ���� ��ġ�� a���� �ƴ� ���, �밢�� �������� �̵� ���� ���� Ȯ��
	if (startSquare % 8 != 0) {
		int leftCaptureSquare = startSquare - 9 + 16 * color;
		if (b->square[leftCaptureSquare] != None && b->square[leftCaptureSquare] != color) {
			Move move = { startSquare, leftCaptureSquare };
			addLegalMove(l, move);
		}
	}

	// ���� ��ġ�� h���� �ƴ� ���, �밢�� ���������� �̵� ���� ���� Ȯ��
	if (startSquare % 8 != 7) {
		int rightCaptureSquare = startSquare - 7 + 16 * color;
		if (b->square[rightCaptureSquare] != None && b->square[rightCaptureSquare] != color) {
			Move move = { startSquare, rightCaptureSquare };
			addLegalMove(l, move);
		}
	}
}


void generatePawnAttackMoves(int startSquare, MoveList* l, Board* b, int color) {
	if (startSquare % 8 != 0) {
		int leftCaptureSquare = startSquare - 9 + 16 * color;
		if (b->square[leftCaptureSquare] != None && b->square[leftCaptureSquare] != color) {
			Move move = { startSquare, leftCaptureSquare };
			addLegalMove(l, move);
		}
	}

	// ���� ��ġ�� h���� �ƴ� ���, �밢�� ���������� �̵� ���� ���� Ȯ��
	if (startSquare % 8 != 7) {
		int rightCaptureSquare = startSquare - 7 + 16 * color;
		if (b->square[rightCaptureSquare] != None && b->square[rightCaptureSquare] != color) {
			Move move = { startSquare, rightCaptureSquare };
			addLegalMove(l, move);
		}
	}
}


int compareIntegers(const void* a, const void* b) {
	return (*(int*)a - *(int*)b);
}


int isChecked(Board* b, int* attackMap) {
	int kingSquare = 0;
	for (int i = 0; i < BOARD_SIZE; i++) {
		if (b->square[i] == (King | b->turnToPlay)) {
			kingSquare = i;
			break;
		}
	}
	int attackMapSize = attackMap[64];

	for (int i = 0; i < attackMapSize - 1; i++) {
		if (kingSquare == attackMap[i]) {
			return TRUE;
		}
	}
	return FALSE;
}
