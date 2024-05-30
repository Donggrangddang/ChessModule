#include "piece.h"



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