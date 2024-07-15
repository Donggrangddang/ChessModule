#include "piece.h"


/**
* @brief 기물과 색의 일치 여부를 알려주는 함수
* @param int piece 기물
* @param int color 색 (흰색 : 1, 검은색 : 0)
* @return int 일치여부(일치 : TRUE, 불일치 : FALSE)
*/
int isColor(int piece, int color) {
	if (color == 1) {
		isColor(piece, White);
	}
	else if (color == 0) {
		isColor(piece, Black);
	}
	else {
		return (piece & color) != 0;
	}
}


int isSlidingPiece(int piece) {
	if ((piece == (White | Queen)) || (piece == (Black | Queen)) || (piece == (White | Bishop)) || (piece == (Black | Bishop))) {
		return TRUE;
	}
	return FALSE;
}


int isStraightPiece(int piece) {
	if ((piece == (White | Queen)) || (piece == (Black | Queen)) || (piece == (White | Rook)) || (piece == (Black | Rook))) {
		return TRUE;
	}
	return FALSE;
}


int isKnight(int piece) {
	if ((piece == (White | Knight)) || (piece == (Black | Knight))) {
		return TRUE;
	}
	return FALSE;
}


int isKing(int piece) {
	if ((piece == (White | King)) || (piece == (Black | King))) {
		return TRUE;
	}
	return FALSE;
}


int isPawn(int piece) {
	if ((piece == (White | Pawn)) || (piece == (Black | Pawn))) {
		return TRUE;
	}
	return FALSE;
}


#define Black 0x8
#define White 0x10
#define Rook 0x4
#define Knight 0x2
#define Bishop 0x3
#define Queen 0x5
#define King 0x6
#define Pawn 0x1

int symbolToNumber(char symbol) {
	switch (symbol) {
	case 'r':
		return Black | Rook;
	case 'R':
		return White | Rook;
	case 'n':
		return Black | Knight;
	case 'N':
		return White | Knight;
	case 'b':
		return Black | Bishop;
	case 'B':
		return White | Bishop;
	case 'q':
		return Black | Queen;
	case 'Q':
		return White | Queen;
	case 'k':
		return Black | King;
	case 'K':
		return White | King;
	case 'p':
		return Black | Pawn;
	case 'P':
		return White | Pawn;
	default:
		return -1;  // 유효하지 않은 기호의 경우 -1 반환
	}
}
