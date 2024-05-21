#ifndef piece_h
#define piece_h

#define None 0
#define King 1
#define Pawn 2
#define Knight 3
#define Bishop 4
#define Rook 5
#define Queen 6

#define White 8
#define Black 16

#define TRUE 1
#define FALSE 0

int isColor(int piece, int color);

int isSlidingPiece(int piece);

int isStraightPiece(int piece);

int isKnight(int piece);
#endif
