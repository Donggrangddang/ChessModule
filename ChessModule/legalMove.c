#include "legalMove.h"
#include "piece.h"
#include "board.h"
#include "move.h"
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
int generateAttackMap(Board* b, int color) {

	Move move = { 0, 0 };
	MoveList legalMoveList = { move, 0 };

	for (int startSquare = 0; startSquare < BOARD_SIZE; startSquare++) {

		int piece = b->square[startSquare];

		if (isColor(piece, color)) {

			if (isKnight(piece)) { // ����Ʈ
				generateKnightMoves(startSquare, &legalMoveList, b);
			}
			else if (isKing(piece) == FALSE) {
				if (isStraightPiece(piece)) { // ��, ��
					generateStraightMoves(startSquare, &legalMoveList, b);
				}
				if (isSlidingPiece(piece)) { // ���, ��
					generateSlidingMoves(startSquare, &legalMoveList, b);
				}
				if (isPawn(piece)) { // ��
					generatePawnMoves(startSquare, &legalMoveList, b);
				}
			}
			else if (isKing(piece) == TRUE) { // ŷ
				generateKingMoves(startSquare, &legalMoveList, b);
			}
		}
	}

	int attackMap[64];
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

	// �������� ����
	qsort(attackMap, attackMapSize, sizeof(int), compareIntegers);
	attackMap[63] = attackMapSize;
	return attackMap;
};

MoveList generateLegalMoves(Board* b) {

	int attackMap = generateAttackMap(b, !(b->turnToPlay));
	int* attackMapPointer = &attackMap;
	Move move = { 0, 0 };
	MoveList checkedLegalMoveList = { move, 0 };

	if (isChecked(b, &attackMap)) {
		int kingSquare = 0;
		for (int i = 0; i < BOARD_SIZE; i++) {
			if (b->square[i] == (King | b->turnToPlay)) {
				kingSquare = i;
				break;
			}
		}
		for (int i = 0; i < 8; i++) {
			int targetSquare = DirectionOffsets[i] + kingSquare;
			for (int j = 0; j < attackMapPointer[63]; j++) {
				if (targetSquare == attackMapPointer[j]) {
					break;
				}
				if (j == attackMapPointer[63] - 1) {
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
				generateKnightMoves(startSquare, &legalMoveList, b);
			}
			else if (isKing(piece) == FALSE) {
				if (isStraightPiece(piece)) { // ��, ��
					generateStraightMoves(startSquare, &legalMoveList, b);
				}
				if (isSlidingPiece(piece)) { // ���, ��
					generateSlidingMoves(startSquare, &legalMoveList, b);
				}
				if (isPawn(piece)) { // ��
					generatePawnMoves(startSquare, &legalMoveList, b);
				}
			}
			else if (isKing(piece) == TRUE) { // ŷ
				generateKingMoves(startSquare, &legalMoveList, b);
			}
		}
	}
	return legalMoveList;
}


/**
* @brief �밢�� ������(���)�� ����� �ִ� �Լ�
* @param int startSquare : ������ �⹰�� ��ġ�� Square
* @param Movelist *l : �� �������� �߰��� Movelist�� �޸� �ּ�
* @param Board *b : ������ �⹰�� ã�� Board�� �޸� �ּ�
*20250517
*/
void generateSlidingMoves(int startSquare, MoveList* l, Board* b) {

	for (int i = 4; i < 8; i++) {
		int targetSquare = startSquare;
		targetSquare += DirectionOffsets[i];
		while (targetSquare >= 0 && targetSquare < BOARD_SIZE) {
			if (targetSquare % 8 == 0 || targetSquare % 8 == 7 || targetSquare / 8 == 0 || targetSquare / 8 == 7) {
				if (b->square[targetSquare] == None) { // �� �� �ִ� ĭ�� �����
					Move move = { startSquare, targetSquare };
					addLegalMove(l, move);
				}
				else if (isColor(b->square[targetSquare], b->turnToPlay) == FALSE) { // �� �� �ִ� ĭ�� ��� �⹰�� �ִ�
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
			else if (isColor(b->square[targetSquare], b->turnToPlay) == FALSE) { // �� �� �ִ� ĭ�� ��� �⹰�� �ִ�
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
void generateStraightMoves(int startSquare, MoveList* l, Board* b) {

	for (int i = 0; i < 4; i++) {
		int targetSquare = startSquare;
		targetSquare += DirectionOffsets[i];
		while (targetSquare >= 0 && targetSquare < BOARD_SIZE) {
			if (targetSquare % 8 == 0 || targetSquare % 8 == 7 || targetSquare / 8 == 0 || targetSquare / 8 == 7) {
				if (b->square[targetSquare] == None) { // �� �� �ִ� ĭ�� �����
					Move move = { startSquare, targetSquare };
					addLegalMove(l, move);
				}
				else if (isColor(b->square[targetSquare], b->turnToPlay) == FALSE) { // �� �� �ִ� ĭ�� ��� �⹰�� �ִ�
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
			else if (isColor(b->square[targetSquare], b->turnToPlay) == FALSE) { // �� �� �ִ� ĭ�� ��� �⹰�� �ִ�
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
void generateKnightMoves(int startSquare, MoveList* l, Board* b) {

	for (int i = 0; i < 8; i++) {
		int newRow = startSquare / 8 + KnightDirectionOffsets[i][0];
		int newCol = startSquare % 8 + KnightDirectionOffsets[i][1];

		// ���ο� ��ġ�� ���� �ȿ� �ִ��� Ȯ��
		if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
			int targetSquare = newRow * 8 + newCol;
			if ((b->square[targetSquare] == None) || (isColor(b->square[targetSquare], b->turnToPlay) == FALSE)) {
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
void generateKingMoves(int startSquare, MoveList* l, Board* b) {

	for (int i = 0; i < 8; i++) {
		int targetSquare = startSquare;
		targetSquare += DirectionOffsets[i];
		if (targetSquare >= 0 && targetSquare < BOARD_SIZE) {
			if (abs((targetSquare / 8) - (startSquare / 8)) > 1 || abs((targetSquare % 8) - (startSquare % 8)) > 1) {
				continue;
			}
			// üũ ���ϸ� �̶�� ������ �߰��ؾ���
			if (b->square[targetSquare] == None) { // �� �� �ִ� ĭ�� �����
				Move move = {startSquare, targetSquare};
				addLegalMove(l, move);
			}
			else if (isColor(b->square[targetSquare], b->turnToPlay) == FALSE) { // �� �� �ִ� ĭ�� ��� �⹰�� �ִ�
				Move move = {startSquare, targetSquare};
				addLegalMove(l, move);
			}
			else
			{
				break;
			}
		}
	}
}


void generatePawnMoves(int startSquare, MoveList* l, Board* b) {
	// ���� ������ �� �ִ��� Ȯ��
	if (b->square[startSquare - 8 + 16 * b->turnToPlay] == None) {
		// ù �̵� �� �� ĭ ���� ���� ���� Ȯ��
		if ((48 - 40 * b->turnToPlay <= startSquare) && (startSquare <= 55 - 40 * b->turnToPlay) &&
			(b->square[startSquare - 16 + 32 * b->turnToPlay] == None)) {
			Move move = { startSquare, startSquare - 16 + 32 * b->turnToPlay };
			addLegalMove(l, move);
		}
		// �� ĭ ����
		Move move = { startSquare, startSquare - 8 + 16 * b->turnToPlay };
		addLegalMove(l, move);
	}

	// ���� ��ġ�� a���� �ƴ� ���, �밢�� �������� �̵� ���� ���� Ȯ��
	if (startSquare % 8 != 0) {
		int leftCaptureSquare = startSquare - 9 + 16 * b->turnToPlay;
		if (b->square[leftCaptureSquare] != None && b->square[leftCaptureSquare] != b->turnToPlay) {
			Move move = { startSquare, leftCaptureSquare };
			addLegalMove(l, move);
		}
	}

	// ���� ��ġ�� h���� �ƴ� ���, �밢�� ���������� �̵� ���� ���� Ȯ��
	if (startSquare % 8 != 7) {
		int rightCaptureSquare = startSquare - 7 + 16 * b->turnToPlay;
		if (b->square[rightCaptureSquare] != None && b->square[rightCaptureSquare] != b->turnToPlay) {
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
	int attackMapSize = attackMap[63];

	for (int i = 0; i < attackMapSize - 1; i++) {
		if (kingSquare == attackMap[i]) {
			return TRUE;
		}
	}
	return FALSE;
}
