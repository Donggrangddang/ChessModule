#include "legalMove.h"

#ifndef move_h
#define move_h

short convertToIndex(char file, char rank);

int doMove(Board* b, MoveList* l, Move move);

void undoMove(Board* b, Move lastMove);
#endif