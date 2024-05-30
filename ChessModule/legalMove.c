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
const int DirectionOffsets[8] = {8, -8, -1, 1, 7, -7, 9, -9}; // 북 남 서 동

/**
* @brief 가능한 움직임을 list에 추가
* @param MoveList* list MoveList의 메모리 주소
* @param Move move 움직임 값
*/
void addLegalMove(MoveList* list, Move move) {
	list->movesList[list->size++] = move;
};


void initMove(Move* m, int startSquare, int targetSquare) {
	m->startSquare = startSquare;
	m->targetSquare = targetSquare;
}

/**
* @brief AttackMap을 만드는 함수
* @param Board* b Board의 메모리 주소
* @param int color 공격하는 색
* @return AttackMap 공격하고 있는 모든 칸의 배열
*/
int* generateAttackMap(Board* b, int color) {
	if (b == NULL) {
		return NULL;  // 보드가 NULL이면 즉시 NULL을 반환
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
		return NULL;  // 메모리 할당 실패 시 NULL을 반환
	}

	int attackMapSize = 0;

	for (int i = 0; i < legalMoveList.size; i++) {
		int targetSquare = legalMoveList.movesList[i].targetSquare;

		// 중복 확인
		int isDuplicate = 0;
		for (int j = 0; j < attackMapSize; j++) {
			if (attackMap[j] == targetSquare) {
				isDuplicate = 1;
				break;
			}
		}

		// 중복이 아니면 추가
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

			if (isKnight(piece)) { // 나이트
				generateKnightMoves(startSquare, &legalMoveList, b, b->turnToPlay);
			}
			else if (isKing(piece) == FALSE) {
				if (isStraightPiece(piece)) { // 룩, 퀸
					generateStraightMoves(startSquare, &legalMoveList, b, b->turnToPlay);
				}
				if (isSlidingPiece(piece)) { // 비숍, 퀸
					generateSlidingMoves(startSquare, &legalMoveList, b, b->turnToPlay);
				}
				if (isPawn(piece)) { // 폰
					generatePawnMoves(startSquare, &legalMoveList, b, b->turnToPlay);
				}
			}
			else if (isKing(piece) == TRUE) { // 킹
				generateKingMoves(startSquare, &legalMoveList, b, b->turnToPlay, attackMap);
			}
		}
	}

	free(attackMap);

	return legalMoveList;
}

/**
* @brief 대각선 움직임(비숍)을 만들어 주는 함수
* @param int startSquare : 움직일 기물이 위치한 Square
* @param Movelist *l : 이 움직임을 추가할 Movelist의 메모리 주소
* @param Board *b : 움직일 기물을 찾을 Board의 메모리 주소
*20250517
*/
void generateSlidingMoves(int startSquare, MoveList* l, Board* b, int color) {

	for (int i = 4; i < 8; i++) {
		int targetSquare = startSquare;
		targetSquare += DirectionOffsets[i];
		while (targetSquare >= 0 && targetSquare < BOARD_SIZE) {
			if (targetSquare % 8 == 0 || targetSquare % 8 == 7 || targetSquare / 8 == 0 || targetSquare / 8 == 7) {
				if (b->square[targetSquare] == None) { // 갈 수 있는 칸이 비었다
					Move move = { startSquare, targetSquare };
					addLegalMove(l, move);
				}
				else if (isColor(b->square[targetSquare], color) == FALSE) { // 갈 수 있는 칸에 상대 기물이 있다
					Move move = { startSquare, targetSquare };
					addLegalMove(l, move);
				}
				break; // 가장자리에 도착하면 루프를 종료
			}
			if (b->square[targetSquare] == 0) { // 갈 수 있는 칸이 비었다
				Move move = {startSquare, targetSquare};
				addLegalMove(l, move);
				targetSquare += DirectionOffsets[i];
			}
			else if (isColor(b->square[targetSquare], color) == FALSE) { // 갈 수 있는 칸에 상대 기물이 있다
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
* @brief 직선 움직임(룩)을 만들어 주는 함수
* @param int startSquare : 움직일 기물이 위치한 Square
* @param Movelist *l : 이 움직임을 추가할 Movelist의 메모리 주소
* @param Board *b : 움직일 기물을 찾을 Board의 메모리 주소
*20250517
*/
void generateStraightMoves(int startSquare, MoveList* l, Board* b, int color) {

	for (int i = 0; i < 4; i++) {
		int targetSquare = startSquare;
		targetSquare += DirectionOffsets[i];
		while (targetSquare >= 0 && targetSquare < BOARD_SIZE) {
			if (targetSquare % 8 == 0 || targetSquare % 8 == 7 || targetSquare / 8 == 0 || targetSquare / 8 == 7) {
				if (b->square[targetSquare] == None) { // 갈 수 있는 칸이 비었다
					Move move = { startSquare, targetSquare };
					addLegalMove(l, move);
				}
				else if (isColor(b->square[targetSquare], color) == FALSE) { // 갈 수 있는 칸에 상대 기물이 있다
					Move move = { startSquare, targetSquare };
					addLegalMove(l, move);
				}
				break; // 가장자리에 도착하면 루프를 종료
			}
			if (b->square[targetSquare] == None) { // 갈 수 있는 칸이 비었다
				Move move = { startSquare, targetSquare };
				addLegalMove(l, move);
				targetSquare += DirectionOffsets[i];
			}
			else if (isColor(b->square[targetSquare], color) == FALSE) { // 갈 수 있는 칸에 상대 기물이 있다
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
* @brief 나이트의 움직임을 만들어 주는 함수
* @param int startSquare : 움직일 기물이 위치한 Square
* @param Movelist *l : 이 움직임을 추가할 Movelist의 메모리 주소
* @param Board *b : 움직일 기물을 찾을 Board의 메모리 주소
*20250517
*/
void generateKnightMoves(int startSquare, MoveList* l, Board* b, int color) {

	for (int i = 0; i < 8; i++) {
		int newRow = startSquare / 8 + KnightDirectionOffsets[i][0];
		int newCol = startSquare % 8 + KnightDirectionOffsets[i][1];

		// 새로운 위치가 보드 안에 있는지 확인
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
* @brief 킹의 움직임을 만들어 주는 함수(체크 고려)
* @param int startSquare : 움직일 기물이 위치한 Square
* @param Movelist *l : 이 움직임을 추가할 Movelist의 메모리 주소
* @param Board *b : 움직일 기물을 찾을 Board의 메모리 주소
*20250517
*/
void generateKingMoves(int startSquare, MoveList* l, Board* b, int color, int* attackMap) {
	for (int i = 0; i < 8; i++) {
		int targetSquare = startSquare + DirectionOffsets[i];
		// 같은 행 또는 열에서 벗어나는지 확인
		if (abs((targetSquare / 8) - (startSquare / 8)) > 1 || abs((targetSquare % 8) - (startSquare % 8)) > 1) {
			continue;
		}
		// 보드의 경계를 넘어서는지 확인
		if (targetSquare >= 0 && targetSquare < BOARD_SIZE) {
			if (isChecked(b, attackMap) == FALSE) {
				if (b->square[targetSquare] == None) { // 갈 수 있는 칸이 비었다
					Move move = { startSquare, targetSquare };
					addLegalMove(l, move);
				}
				else if (isColor(b->square[targetSquare], color) == FALSE) { // 갈 수 있는 칸에 상대 기물이 있다
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
		// 같은 행 또는 열에서 벗어나는지 확인
		if (abs((targetSquare / 8) - (startSquare / 8)) > 1 || abs((targetSquare % 8) - (startSquare % 8)) > 1) {
			continue;
		}
		// 보드의 경계를 넘어서는지 확인
		if (targetSquare >= 0 && targetSquare < BOARD_SIZE) {
			Move move = { startSquare, targetSquare };
			addLegalMove(l, move);
		}
	}
}


void generatePawnMoves(int startSquare, MoveList* l, Board* b, int color) {
	// 폰이 전진할 수 있는지 확인
	if (b->square[startSquare - 8 + 16 * color] == None) {
		// 첫 이동 시 두 칸 전진 가능 여부 확인
		if ((48 - 40 * color <= startSquare) && (startSquare <= 55 - 40 * color) &&
			(b->square[startSquare - 16 + 32 * color] == None)) {
			Move move = { startSquare, startSquare - 16 + 32 * color };
			addLegalMove(l, move);
		}
		// 한 칸 전진
		Move move = { startSquare, startSquare - 8 + 16 * color };
		addLegalMove(l, move);
	}

	// 현재 위치가 a열이 아닌 경우, 대각선 왼쪽으로 이동 가능 여부 확인
	if (startSquare % 8 != 0) {
		int leftCaptureSquare = startSquare - 9 + 16 * color;
		if (b->square[leftCaptureSquare] != None && b->square[leftCaptureSquare] != color) {
			Move move = { startSquare, leftCaptureSquare };
			addLegalMove(l, move);
		}
	}

	// 현재 위치가 h열이 아닌 경우, 대각선 오른쪽으로 이동 가능 여부 확인
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

	// 현재 위치가 h열이 아닌 경우, 대각선 오른쪽으로 이동 가능 여부 확인
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
