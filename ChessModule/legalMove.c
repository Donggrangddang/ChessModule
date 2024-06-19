#include "legalMove.h"
#include "piece.h"
#include "board.h"
#include "move.h"
#include "malloc.h"
#include "interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


const int KnightDirectionOffsets[8][2] = {
		{-2, -1}, {-2, 1}, {2, -1}, {2, 1},
		{-1, -2}, {-1, 2}, {1, -2}, {1, 2}
};
const int DirectionOffsets[8] = {8, -8, -1, 1, 7, -7, 9, -9}; // �� �� �� �� �ϼ� ���� �ϵ� ����

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
					generateStraightAttackMoves(startSquare, &legalMoveList, b, !(b->turnToPlay));
				}
				if (isSlidingPiece(piece)) {
					generateSlidingAttackMoves(startSquare, &legalMoveList, b, !(b->turnToPlay));
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

	qsort(attackMap, attackMapSize, sizeof(int), compareIntegers);

	attackMap[BOARD_SIZE] = attackMapSize;
	return attackMap;
}


MoveList generateLegalMoves(Board* b) {

	int* attackMap = generateAttackMap(b, !(b->turnToPlay));
	Move move = { 0, 0 };
	MoveList checkedLegalMoveList = { move, 0 };

	if (isChecked(b, attackMap, -1)) {
		int kingSquare = 0;
		for (int i = 0; i < BOARD_SIZE; i++) {
			if (isColor(b->square[i], b->turnToPlay) && isKing(b->square[i])) {
				kingSquare = i;
				break;
			}
		}
		printf("King is checked\n");
		MoveList legalMoveList = generateUncheckedMoves(b, attackMap);
		for (int i = 0; i < legalMoveList.size; i++) {
			Move move = legalMoveList.movesList[i];
			int piece = doMove(b, &legalMoveList, move);
			b->turnToPlay = !(b->turnToPlay);
			int* newAttackMap = generateAttackMap(b, !(b->turnToPlay));
			if (isChecked(b, newAttackMap, -1) == FALSE) {
				addLegalMove(&checkedLegalMoveList, move);
			}
			b->turnToPlay = !(b->turnToPlay);
			free(newAttackMap);
			undoMove(b, move, piece);
		}
		return checkedLegalMoveList;
	}

	return generateUncheckedMoves(b, attackMap);
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
* @brief ŷ�� �������� ����� �ִ� �Լ�(üũ ��� x)
* @param int startSquare : ������ �⹰�� ��ġ�� Square
* @param Movelist *l : �� �������� �߰��� Movelist�� �޸� �ּ�
* @param Board *b : ������ �⹰�� ã�� Board�� �޸� �ּ�
*20240525
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
			if (isChecked(b, attackMap, targetSquare) == FALSE) {
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


void generateSlidingAttackMoves(int startSquare, MoveList* l, Board* b, int color) {
	int length = 2;
	int array_i[4] = { 0 }; // ��� ��츦 Ŀ���� �� �ֵ��� ũ�⸦ 4�� ����

	int file = startSquare % 8;
	int rank = startSquare / 8;
	switch (file) {
	case 0:
		switch (rank) {
		case 0:
			array_i[0] = 5;
			length = 1;
			break;
		case 7:
			array_i[0] = 4;
			length = 1;
			break;
		default:
			array_i[0] = 5; array_i[1] = 6;
			length = 2;
			break;
		}
		break;
	case 7:
		switch (rank) {
		case 0:
			array_i[0] = 7;
			length = 1;
			break;
		case 7:
			array_i[0] = 5;
			length = 1;
			break;
		default:
			array_i[0] = 5; array_i[1] = 7;
			length = 2;
			break;
		}
		break;
	default:
		if (rank == 0) {
			array_i[0] = 6; array_i[1] = 7;
			length = 2;
		}
		else if (rank == 7) {
			array_i[0] = 4; array_i[1] = 5;
			length = 2;
		}
		else {
			array_i[0] = 4; array_i[1] = 5; array_i[2] = 6; array_i[3] = 7;
			length = 4;
		}
		break;
	}

	for (int j = 0; j < length; j++) {
		int targetSquare = startSquare;
		targetSquare += DirectionOffsets[array_i[j]]; // ���� 1�� 2�� �ذ��ϱ� ���� ��ȣ�� �ùٸ��� ����
		while (targetSquare >= 0 && targetSquare < BOARD_SIZE) {
			if (targetSquare % 8 == 0 || targetSquare % 8 == 7 || targetSquare / 8 == 0 || targetSquare / 8 == 7) {
				if (b->square[targetSquare] == None) { // �� �� �ִ� ĭ�� �����
					Move move = { startSquare, targetSquare };
					addLegalMove(l, move);
				}
				else { // �� �� �ִ� ĭ�� �⹰�� �ִ�
					Move move = { startSquare, targetSquare };
					addLegalMove(l, move);
				}
				break; // �����ڸ��� �����ϸ� ������ ����
			}
			if (b->square[targetSquare] == None) { // �� �� �ִ� ĭ�� �����
				Move move = { startSquare, targetSquare };
				addLegalMove(l, move);
				targetSquare += DirectionOffsets[array_i[j]]; // ���� 3�� �ذ��ϱ� ���� 'i'�� 'array_i[j]'�� ����
			}
			else { // �� �� �ִ� ĭ�� ��� �⹰�� �ִ�
				Move move = { startSquare, targetSquare };
				addLegalMove(l, move);
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
void generateStraightAttackMoves(int startSquare, MoveList* l, Board* b, int color) {

	int length = 2;
	int array_i[4] = { 0 }; // ��� ��츦 Ŀ���� �� �ֵ��� ũ�⸦ 4�� ����
	int temp[4] = { 0 }; // �ִ� ũ��� �����Ͽ� ��� ��쿡 ����

	int file = startSquare % 8;
	int rank = startSquare / 8;

	switch (file) {
	case 0:
		switch (rank) {
		case 0:
			temp[0] = 0; temp[1] = 3; length = 2;
			break;
		case 7:
			temp[0] = 1; temp[1] = 3; length = 2;
			break;
		default:
			temp[0] = 0; temp[1] = 1; temp[2] = 3; length = 3;
			break;
		}
		break;

	case 7:
		switch (rank) {
		case 0:
			temp[0] = 0; temp[1] = 2; length = 2;
			break;
		case 7:
			temp[0] = 1; temp[1] = 2; length = 2;
			break;
		default:
			temp[0] = 0; temp[1] = 1; temp[2] = 2; length = 3;
			break;
		}
		break;

	default:
		switch (rank) {
		case 0:
			temp[0] = 0; temp[1] = 2; temp[2] = 3; length = 3;
			break;
		case 7:
			temp[0] = 1; temp[1] = 2; temp[2] = 3; length = 3;
			break;
		default:
			temp[0] = 4; temp[1] = 5; temp[2] = 6; temp[3] = 7; length = 4;
			break;
		}
		break;
	}

	memcpy(array_i, temp, length * sizeof(int));

	for (int j = 0; j < length; j++) {
		int targetSquare = startSquare;
		targetSquare += DirectionOffsets[array_i[j]]; // ���� 1�� 2�� �ذ��ϱ� ���� ��ȣ�� �ùٸ��� ����
		while (targetSquare >= 0 && targetSquare < BOARD_SIZE) {
			if (targetSquare % 8 == 0 || targetSquare % 8 == 7 || targetSquare / 8 == 0 || targetSquare / 8 == 7) {
				if (b->square[targetSquare] == None) { // �� �� �ִ� ĭ�� �����
					Move move = { startSquare, targetSquare };
					addLegalMove(l, move);
				}
				else { // �� �� �ִ� ĭ�� �⹰�� �ִ�
					Move move = { startSquare, targetSquare };
					addLegalMove(l, move);
				}
				break; // �����ڸ��� �����ϸ� ������ ����
			}
			if (b->square[targetSquare] == None) { // �� �� �ִ� ĭ�� �����
				Move move = { startSquare, targetSquare };
				addLegalMove(l, move);
				targetSquare += DirectionOffsets[array_i[j]]; // ���� 3�� �ذ��ϱ� ���� 'i'�� 'array_i[j]'�� ����
			}
			else { // �� �� �ִ� ĭ�� ��� �⹰�� �ִ�
				Move move = { startSquare, targetSquare };
				addLegalMove(l, move);
				break;
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


void generatePawnAttackMoves(int startSquare, MoveList* l, Board* b, int color) {
	if (startSquare % 8 != 0) {
		int leftCaptureSquare = startSquare - 9 + 16 * color;
		Move move = { startSquare, leftCaptureSquare };
		addLegalMove(l, move);
	}

	if (startSquare % 8 != 7) {
		int rightCaptureSquare = startSquare - 7 + 16 * color;
		Move move = { startSquare, rightCaptureSquare };
		addLegalMove(l, move);
	}
}


void generateKingCheckedMoves(int startSquare, MoveList* l, Board* b, int color, int* attackMap) {
	for (int i = 0; i < 8; i++) {
		int targetSquare = startSquare + DirectionOffsets[i];
		// ���� �� �Ǵ� ������ ������� Ȯ��
		if (abs((targetSquare / 8) - (startSquare / 8)) > 1 || abs((targetSquare % 8) - (startSquare % 8)) > 1) {
			continue;
		}
		// ������ ��踦 �Ѿ���� Ȯ��
		if (targetSquare >= 0 && targetSquare < BOARD_SIZE) {
			if (b->square[targetSquare] == None) {
				if (isChecked(b, attackMap, targetSquare) == FALSE) {
					Move move = { startSquare, targetSquare };
					addLegalMove(l, move);
				}
			}
		}
	}
}


int compareIntegers(const void* a, const void* b) {
	return (*(int*)a - *(int*)b);
}


int isChecked(Board* b, int* attackMap, int param) {

	int kingSquare = 0;

	if (param == -1) {
		for (int i = 0; i < BOARD_SIZE; i++) {
			if (isColor(b->square[i], b->turnToPlay) && isKing(b->square[i])) {
				kingSquare = i;
				break;
			}
		}
	}
	else {
		kingSquare = param;
	}

	int attackMapSize = attackMap[BOARD_SIZE];
	for (int i = 0; i < attackMapSize; i++) {
		if (kingSquare == attackMap[i]) {
			return TRUE;
		}
	}
	return FALSE;
}


MoveList generateUncheckedMoves(Board* b, int* attackMap) {
	Move move = { 0, 0 };
	MoveList legalMoveList = { move , 0 };

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


int isCheckMate(Board* b) {

	MoveList legalMoveList = generateLegalMoves(b);
	if (legalMoveList.size == 0) {
		if (isChecked(b, generateAttackMap(b, b->turnToPlay), -1)) {
			return 2;
		}
		return 1;
	}
	return 0;
}