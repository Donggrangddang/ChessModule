#include "board.h"
#include "legalMove.h"
#include "piece.h"
#include <stdio.h>

/**
* @brief 문자로 입력된 움직임을 숫자로 바꾸는 함수
* @param char file 가로
* @param char rank 세로
* @return short 바뀐 숫자
*/
short convertToIndex(char file, char rank) {
	return (rank - '1') * 8 + (file - 'a');
}

/**
* @brief Board에 움직임을 반영하는 함수 (turnToPlay 바꾸는 걸 main.c로 뺌)
* @param Board* b 움직임을 반영할 Board의 메모리 주소
* @param MoveList* l 가능한 움직임인지 여부를 판단할 MoveList의 메모리 주소
* @param Move move 취할 움직임
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