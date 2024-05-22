#include "legalMove.h"
#include "piece.h"
#include "board.h"


const int KnightDirectionOffsets[8] = {17, 10, -6, -15, -17, -10, 6, 15 };
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
* @brief ������ ��� �������� ����� �Լ�
* @param Board* b Board�� �޸� �ּ�
* @return Movelist ������ �������� �� ��
*/
MoveList generateLegalMoves(Board* b) {

	MoveList legalMoveList;
	legalMoveList.size = 0;

	for (int startSquare = 0; startSquare < BOARD_SIZE; startSquare++) {

		int piece = b->square[startSquare];

		if (isColor(piece, White)) { // ���߿� White�� ���� ������ ������ ������ �ٲ����

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
			}
			else { // ŷ
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
			Move move;
			if (b->square[targetSquare] == 0) { // �� �� �ִ� ĭ�� �����
				initMove(&move, startSquare, targetSquare);
				addLegalMove(l, move);
				targetSquare += DirectionOffsets[i];
			}
			else if (isColor(b->square[targetSquare], b->turnToPlay) == FALSE) { // �� �� �ִ� ĭ�� ��� �⹰�� �ִ�
				initMove(&move, startSquare, targetSquare);
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
void generateStraightMoves(int startSquare, MoveList* l, Board* b) {

	for (int i = 0; i < 4; i++) {
		int targetSquare = startSquare;
		targetSquare += DirectionOffsets[i];
		while (targetSquare >= 0 && targetSquare < BOARD_SIZE) {
			Move move;
			if (b->square[targetSquare] == None) { // �� �� �ִ� ĭ�� �����
				initMove(&move, startSquare, targetSquare);
				addLegalMove(l, move);
				targetSquare += DirectionOffsets[i];
			}
			else if (isColor(b->square[targetSquare], b->turnToPlay) == FALSE) { // �� �� �ִ� ĭ�� ��� �⹰�� �ִ�
				initMove(&move, startSquare, targetSquare);
				addLegalMove(l, move);
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
		int targetSquare = startSquare;
		targetSquare += KnightDirectionOffsets[i];
		if (targetSquare >= 0 && targetSquare < BOARD_SIZE) {
			Move move;
			if (b->square[targetSquare] == None) { // �� �� �ִ� ĭ�� �����
				initMove(&move, startSquare, targetSquare);
				addLegalMove(l, move);
			}
			else if (isColor(b->square[targetSquare], b->turnToPlay) == FALSE) { // �� �� �ִ� ĭ�� ��� �⹰�� �ִ�
				initMove(&move, startSquare, targetSquare);
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
	// üũ �޴� ���� ������ �� ���� �����ؾ���

	for (int i = 0; i < 8; i++) {
		int targetSquare = startSquare;
		targetSquare += DirectionOffsets[i];
		if (targetSquare >= 0 && targetSquare < BOARD_SIZE) {
			Move move;
			// üũ ���ϸ� �̶�� ������ �߰��ؾ���
			if (b->square[targetSquare] == None) { // �� �� �ִ� ĭ�� �����
				initMove(&move, startSquare, targetSquare);
				addLegalMove(l, move);
			}
			else if (isColor(b->square[targetSquare], b->turnToPlay) == FALSE) { // �� �� �ִ� ĭ�� ��� �⹰�� �ִ�
				initMove(&move, startSquare, targetSquare);
				addLegalMove(l, move);
			}
		}
	}
}