#include "legalMove.h"

#ifndef move_h
#define move_h

short convertToIndex(char file, char rank);

void applyMove(Board* b, MoveList* l, Move move);
#endif