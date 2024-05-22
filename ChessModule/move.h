#include "board.h"
#include "piece.h"

#ifndef move_h
#define move_h

#define MAX_SIZE 100

extern const int DirectionOffsets[8];
extern const int KnightDirectionOffsets[8];

typedef struct {
	short startSquare; // readonly
	short targetSquare; // readonly
} Move;

typedef struct {
	Move movesList[MAX_SIZE];
	int size;
} MoveList;

void addLegalMove(MoveList* list, Move move);

void initMove(Move* m, int startSquare, int targetSquare);

MoveList generateLegalMoves(Board* b);

void generateSlidingMoves(int startSquare, MoveList* l, Board* b);

void generateStraightMoves(int startSquare, MoveList* l, Board* b);

void generateKnightMoves(int startSquare, MoveList* l, Board* b);

void generateKingMoves(int startSquare, MoveList* l, Board* b);
#endif