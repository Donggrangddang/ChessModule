#include "legalMove.h"

#ifndef move_h
#define move_h

char getSAN();

int convertToIndex(char file, char rank);

Move paresSAN(char* san);

void applyMove(Board* b, MoveList* l, Move move);
#endif