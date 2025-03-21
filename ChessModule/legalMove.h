#include "board.h"
#include "piece.h"

#ifndef legalMove_h
#define legalMove_h

#define MAX_SIZE 100

typedef struct {
	int startSquare; // readonly
	int targetSquare; // readonly
	int promotionPiece;
} Move;


extern const int DirectionOffsets[8];
extern const int KnightDirectionOffsets[8][2];
extern const Move enPassantSquare[16];
extern const char enPassantPosition[16][4];

typedef struct {
	Move movesList[MAX_SIZE];
	int size;
} MoveList;

void addLegalMove(MoveList* list, Move move);

void initMove(Move* m, int startSquare, int targetSquare);

int* generateAttackMap(Board* b, int color);

MoveList generateLegalMoves(Board* b);

void generateSlidingMoves(int startSquare, MoveList* l, Board* b, int color);

void generateStraightMoves(int startSquare, MoveList* l, Board* b, int color);

void generateKnightMoves(int startSquare, MoveList* l, Board* b, int color);

void generateKingMoves(int startSquare, MoveList* l, Board* b, int color, int* attackMap);

void generatePawnMoves(int startSquare, MoveList* l, Board* b, int color);

void generateSlidingAttackMoves(int startSquare, MoveList* l, Board* b, int color);

void generateStraightAttackMoves(int startSquare, MoveList* l, Board* b, int color);

void generateKingAttackMoves(int startSquare, MoveList* l, Board* b, int color);

void generatePawnAttackMoves(int startSquare, MoveList* l, Board* b, int color);

void generateKingCheckedMoves(int startSquare, MoveList* l, Board* b, int color, int* attackMap);

int compareIntegers(const void* a, const void* b);

int isChecked(Board* b, int* attackMap, int kingSquare); // kingSquare이 -1이면 kingSquare 계산하는거

MoveList generateUncheckedMoves(Board* b, int* attackMap);

int isMate(Board* b);

void generatePromotionMoves(MoveList* l, Move move, Board* b);
#endif