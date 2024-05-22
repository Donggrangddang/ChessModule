#include "piece.h"



int isColor(int piece, int color) {
	if ((piece & color) != 0) {
		return TRUE;
	}
	return FALSE;
}

int isSlidingPiece(int piece) {
	if (piece == Bishop || piece == Queen) {
		return TRUE;
	}
	return FALSE;
}

int isStraightPiece(int piece) {
	if (piece == Rook || piece == Queen) {
		return TRUE;
	}
	return FALSE;
}

int isKnight(int piece) {
	if (piece == Knight) {
		return TRUE;
	}
	return FALSE;
}

int isKing(int piece) {
	if (piece == King) {
		return TRUE;
	}
	return FALSE;
}