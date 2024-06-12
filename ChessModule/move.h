#include "legalMove.h"

#ifndef move_h
#define move_h

int convertToIndex(char file, char rank);

char convertToSAN(int square);

int doMove(Board* b, MoveList* l, Move move);

void undoMove(Board* b, Move lastMove);
#endif