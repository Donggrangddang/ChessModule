#include "piece.h"


/**
* @brief �⹰�� ���� ��ġ ���θ� �˷��ִ� �Լ�
* @param int piece �⹰
* @param int color �� (��� : 1, ������ : 0)
* @return int ��ġ����(��ġ : TRUE, ����ġ : FALSE)
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

/**
* @brief �⹰�� �밢�� �������� �ϴ� �⹰���� �˷��ִ� �Լ�
* @param int piece �⹰
* @return int �������� �ϴ��� ���ϴ��� (��ġ : TRUE, ����ġ : FALSE)
*/
int isSlidingPiece(int piece) {
	if ((piece == (White | Queen)) || (piece == (Black | Queen)) || (piece == (White | Bishop)) || (piece == (Black | Bishop))) {
		return TRUE;
	}
	return FALSE;
}

/**
* @brief �⹰�� ���� �������� �ϴ� �⹰���� �˷��ִ� �Լ�
* @param int piece �⹰
* @return int �������� �ϴ��� ���ϴ��� (��ġ : TRUE, ����ġ : FALSE)
*/
int isStraightPiece(int piece) {
	if ((piece == (White | Queen)) || (piece == (Black | Queen)) || (piece == (White | Rook)) || (piece == (Black | Rook))) {
		return TRUE;
	}
	return FALSE;
}

/**
* @brief �⹰�� ����Ʈ���� �˷��ִ� �Լ�
* @param int piece �⹰
* @return int ����Ʈ ���� (��ġ : TRUE, ����ġ : FALSE)
*/
int isKnight(int piece) {
	if ((piece == (White | Knight)) || (piece == (Black | Knight))) {
		return TRUE;
	}
	return FALSE;
}

/**
* @brief �⹰�� ŷ���� �˷��ִ� �Լ�
* @param int piece �⹰
* @return int ŷ ���� (��ġ : TRUE, ����ġ : FALSE)
*/
int isKing(int piece) {
	if ((piece == (White | King)) || (piece == (Black | King))) {
		return TRUE;
	}
	return FALSE;
}