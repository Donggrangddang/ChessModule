#include "piece.h"



int isColor(int piece, int color) {
	if (color == 1) {
		isColor(piece, White);
	}
	else if (color == 0) {
		isColor(piece, Black);
	}
	else {
		if ((piece & color) != 0) {
			return TRUE;
		}
		return FALSE;
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