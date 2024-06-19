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
const int DirectionOffsets[8] = {8, -8, -1, 1, 7, -7, 9, -9}; // 북 남 서 동 북서 남동 북동 남서

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
* @brief 킹의 움직임을 만들어 주는 함수(체크 고려 x)
* @param int startSquare : 움직일 기물이 위치한 Square
* @param Movelist *l : 이 움직임을 추가할 Movelist의 메모리 주소
* @param Board *b : 움직일 기물을 찾을 Board의 메모리 주소
*20240525
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
			if (isChecked(b, attackMap, targetSquare) == FALSE) {
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


void generateSlidingAttackMoves(int startSquare, MoveList* l, Board* b, int color) {
	int length = 2;
	int array_i[4] = { 0 }; // 모든 경우를 커버할 수 있도록 크기를 4로 설정

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
		targetSquare += DirectionOffsets[array_i[j]]; // 문제 1과 2를 해결하기 위해 괄호를 올바르게 닫음
		while (targetSquare >= 0 && targetSquare < BOARD_SIZE) {
			if (targetSquare % 8 == 0 || targetSquare % 8 == 7 || targetSquare / 8 == 0 || targetSquare / 8 == 7) {
				if (b->square[targetSquare] == None) { // 갈 수 있는 칸이 비었다
					Move move = { startSquare, targetSquare };
					addLegalMove(l, move);
				}
				else { // 갈 수 있는 칸에 기물이 있다
					Move move = { startSquare, targetSquare };
					addLegalMove(l, move);
				}
				break; // 가장자리에 도착하면 루프를 종료
			}
			if (b->square[targetSquare] == None) { // 갈 수 있는 칸이 비었다
				Move move = { startSquare, targetSquare };
				addLegalMove(l, move);
				targetSquare += DirectionOffsets[array_i[j]]; // 문제 3을 해결하기 위해 'i'를 'array_i[j]'로 변경
			}
			else { // 갈 수 있는 칸에 상대 기물이 있다
				Move move = { startSquare, targetSquare };
				addLegalMove(l, move);
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
void generateStraightAttackMoves(int startSquare, MoveList* l, Board* b, int color) {

	int length = 2;
	int array_i[4] = { 0 }; // 모든 경우를 커버할 수 있도록 크기를 4로 설정
	int temp[4] = { 0 }; // 최대 크기로 선언하여 모든 경우에 재사용

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
		targetSquare += DirectionOffsets[array_i[j]]; // 문제 1과 2를 해결하기 위해 괄호를 올바르게 닫음
		while (targetSquare >= 0 && targetSquare < BOARD_SIZE) {
			if (targetSquare % 8 == 0 || targetSquare % 8 == 7 || targetSquare / 8 == 0 || targetSquare / 8 == 7) {
				if (b->square[targetSquare] == None) { // 갈 수 있는 칸이 비었다
					Move move = { startSquare, targetSquare };
					addLegalMove(l, move);
				}
				else { // 갈 수 있는 칸에 기물이 있다
					Move move = { startSquare, targetSquare };
					addLegalMove(l, move);
				}
				break; // 가장자리에 도착하면 루프를 종료
			}
			if (b->square[targetSquare] == None) { // 갈 수 있는 칸이 비었다
				Move move = { startSquare, targetSquare };
				addLegalMove(l, move);
				targetSquare += DirectionOffsets[array_i[j]]; // 문제 3을 해결하기 위해 'i'를 'array_i[j]'로 변경
			}
			else { // 갈 수 있는 칸에 상대 기물이 있다
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
		// 같은 행 또는 열에서 벗어나는지 확인
		if (abs((targetSquare / 8) - (startSquare / 8)) > 1 || abs((targetSquare % 8) - (startSquare % 8)) > 1) {
			continue;
		}
		// 보드의 경계를 넘어서는지 확인
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