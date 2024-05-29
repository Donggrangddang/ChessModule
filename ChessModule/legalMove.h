#include "board.h"
#include "piece.h"

#ifndef legalMove_h
#define legalMove_h

#define MAX_SIZE 100

extern const int DirectionOffsets[8];
extern const int KnightDirectionOffsets[8][2];

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

int generateAttackMap(Board* b, int color);

MoveList generateLegalMoves(Board* b);

void generateSlidingMoves(int startSquare, MoveList* l, Board* b);

void generateStraightMoves(int startSquare, MoveList* l, Board* b);

void generateKnightMoves(int startSquare, MoveList* l, Board* b);

void generateKingMoves(int startSquare, MoveList* l, Board* b);

void generatePawnMoves(int startSquare, MoveList* l, Board* b);

int compareIntegers(const void* a, const void* b);

int isChecked(Board* b, int* attackMap);
#endif