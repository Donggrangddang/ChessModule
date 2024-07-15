#include "legalMove.h"

#ifndef move_h
#define move_h

typedef struct MoveResult {
    int capturedPiece;
    int lastEnPassantSquare;
    int promotionPieceColor; // default : 0
} MoveResult;


int convertToIndex(char file, char rank);

char convertToSAN(int square);

int convertPieceToIndex(char piece);

MoveResult doMove(Board* b, MoveList* l, Move move);

void undoMove(Board* b, Move lastMove, MoveResult moveResult);
#endif