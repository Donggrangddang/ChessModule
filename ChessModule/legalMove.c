#include "legalMove.h"
#include "piece.h"
#include "board.h"


const int KnightDirectionOffsets[8] = {17, 10, -6, -15, -17, -10, 6, 15 };
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
* @brief 가능한 모든 움직임을 만드는 함수
* @param Board* b Board의 메모리 주소
* @return Movelist 가능한 움직임의 총 합
*/
MoveList generateLegalMoves(Board* b) {

	MoveList legalMoveList;
	legalMoveList.size = 0;

	for (int startSquare = 0; startSquare < BOARD_SIZE; startSquare++) {

		int piece = b->square[startSquare];

		if (isColor(piece, White)) { // 나중에 White를 현재 움직일 차례의 색으로 바꿔야함

			if (isKnight(piece)) { // 나이트
				generateKnightMoves(startSquare, &legalMoveList, b);
			}
			else if (isKing(piece) == FALSE) {
				if (isStraightPiece(piece)) { // 룩, 퀸
					generateStraightMoves(startSquare, &legalMoveList, b);
				}
				if (isSlidingPiece(piece)) { // 비숍, 퀸
					generateSlidingMoves(startSquare, &legalMoveList, b);
				}
			}
			else { // 킹
				generateKingMoves(startSquare, &legalMoveList, b);
			}
		}
	}
	return legalMoveList;
};

/**
* @brief 대각선 움직임(비숍)을 만들어 주는 함수
* @param int startSquare : 움직일 기물이 위치한 Square
* @param Movelist *l : 이 움직임을 추가할 Movelist의 메모리 주소
* @param Board *b : 움직일 기물을 찾을 Board의 메모리 주소
*20250517
*/
void generateSlidingMoves(int startSquare, MoveList* l, Board* b) {

	for (int i = 4; i < 8; i++) {
		int targetSquare = startSquare;
		targetSquare += DirectionOffsets[i];
		while (targetSquare >= 0 && targetSquare < BOARD_SIZE) {
			Move move;
			if (b->square[targetSquare] == 0) { // 갈 수 있는 칸이 비었다
				initMove(&move, startSquare, targetSquare);
				addLegalMove(l, move);
				targetSquare += DirectionOffsets[i];
			}
			else if (isColor(b->square[targetSquare], b->turnToPlay) == FALSE) { // 갈 수 있는 칸에 상대 기물이 있다
				initMove(&move, startSquare, targetSquare);
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
void generateStraightMoves(int startSquare, MoveList* l, Board* b) {

	for (int i = 0; i < 4; i++) {
		int targetSquare = startSquare;
		targetSquare += DirectionOffsets[i];
		while (targetSquare >= 0 && targetSquare < BOARD_SIZE) {
			Move move;
			if (b->square[targetSquare] == None) { // 갈 수 있는 칸이 비었다
				initMove(&move, startSquare, targetSquare);
				addLegalMove(l, move);
				targetSquare += DirectionOffsets[i];
			}
			else if (isColor(b->square[targetSquare], b->turnToPlay) == FALSE) { // 갈 수 있는 칸에 상대 기물이 있다
				initMove(&move, startSquare, targetSquare);
				addLegalMove(l, move);
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
void generateKnightMoves(int startSquare, MoveList* l, Board* b) {

	for (int i = 0; i < 8; i++) {
		int targetSquare = startSquare;
		targetSquare += KnightDirectionOffsets[i];
		if (targetSquare >= 0 && targetSquare < BOARD_SIZE) {
			Move move;
			if (b->square[targetSquare] == None) { // 갈 수 있는 칸이 비었다
				initMove(&move, startSquare, targetSquare);
				addLegalMove(l, move);
			}
			else if (isColor(b->square[targetSquare], b->turnToPlay) == FALSE) { // 갈 수 있는 칸에 상대 기물이 있다
				initMove(&move, startSquare, targetSquare);
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
void generateKingMoves(int startSquare, MoveList* l, Board* b) {
	// 체크 받는 곳은 움직일 수 없게 설정해야함

	for (int i = 0; i < 8; i++) {
		int targetSquare = startSquare;
		targetSquare += DirectionOffsets[i];
		if (targetSquare >= 0 && targetSquare < BOARD_SIZE) {
			Move move;
			// 체크 당하면 이라는 조건을 추가해야함
			if (b->square[targetSquare] == None) { // 갈 수 있는 칸이 비었다
				initMove(&move, startSquare, targetSquare);
				addLegalMove(l, move);
			}
			else if (isColor(b->square[targetSquare], b->turnToPlay) == FALSE) { // 갈 수 있는 칸에 상대 기물이 있다
				initMove(&move, startSquare, targetSquare);
				addLegalMove(l, move);
			}
		}
	}
}