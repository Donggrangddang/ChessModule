#include "legalMove.h"
#include "piece.h"
#include "board.h"
#include <stdio.h>


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

/**
* @brief ������ ��� �������� ����� �Լ�
* @param Board* b Board�� �޸� �ּ�
* @return Movelist ������ �������� �� ��
*/
MoveList generateLegalMoves(Board* b) {

	Move move = { 0, 0 };
	MoveList legalMoveList = {move, 0};

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
				else if (isSlidingPiece(piece)) { // ���, ��
					generateSlidingMoves(startSquare, &legalMoveList, b);
				}
				else { // ��
					generatePawnMoves(startSquare, &legalMoveList, b);
				}
			} else if (isKing(piece) == TRUE) { // ŷ
				generateKingMoves(startSquare, &legalMoveList, b);
			}
		}
	}
	return legalMoveList;
};

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
* @brief ŷ�� �������� ����� �ִ� �Լ�(üũ ��� x)
* @param int startSquare : ������ �⹰�� ��ġ�� Square
* @param Movelist *l : �� �������� �߰��� Movelist�� �޸� �ּ�
* @param Board *b : ������ �⹰�� ã�� Board�� �޸� �ּ�
*20240525
*/
void generateKingMoves(int startSquare, MoveList* l, Board* b) {
	// üũ �޴� ���� ������ �� ���� �����ؾ���

	for (int i = 0; i < 8; i++) {
		int targetSquare = startSquare;
		targetSquare += DirectionOffsets[i];
		if (targetSquare >= 0 && targetSquare < BOARD_SIZE) {
			// üũ ���ϸ� �̶�� ������ �߰��ؾ���
			if (b->square[targetSquare] == None) { // �� �� �ִ� ĭ�� �����
				Move move = { startSquare, targetSquare };
				addLegalMove(l, move);
			}
			else if (isColor(b->square[targetSquare], b->turnToPlay) == FALSE) { // �� �� �ִ� ĭ�� ��� �⹰�� �ִ�
				Move move = { startSquare, targetSquare };
				addLegalMove(l, move);
			}
			else
			{
				break;
			}
		}
	}
}

/**
* @brief ���� �������� ����� �ִ� �Լ�
* @param int startSquare : ������ �⹰�� ��ġ�� Square
* @param Movelist *l : �� �������� �߰��� Movelist�� �޸� �ּ�
* @param Board *b : ������ �⹰�� ã�� Board�� �޸� �ּ�
* 20240525
*/
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

/**
* @brief üũ ���θ� �˷��ִ� �Լ�(���� �� ����ؼ� ���� ���� üũ���� �ƴ��� �˷���)
* @param MoveList* l : ������� ������ �� �ִ� ������ �˾Ƴ� MoveList�� �޸� �ּ�
* @param Board* b : Board�� �޸� �ּ�
* @return int üũ ���� (��ġ : TRUE, ����ġ : FALSE)
*/
int IsCheck(Board* b, MoveList lastMoveList) {

	int kingSquare;

	for (int i = 0; i < BOARD_SIZE; i++) { // �� ŷ ã��
		if (isKing(b->square[i])) {
			if (isColor(b->square[i], (b->turnToPlay))) {
				kingSquare = i;
			}
		}
	}
	for (int i = 0; i < lastMoveList.size; i++) {
		if (lastMoveList.movesList[i].targetSquare == kingSquare)
			return TRUE;
	}
	return FALSE;
}