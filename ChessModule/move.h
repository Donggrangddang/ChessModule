#include "legalMove.h"

#ifndef move_h
#define move_h

void clearInputBuffer();

char getSAN(char* buffer);

int convertToIndex(char file, char rank);

Move paresSAN(char* san);

void applyMove(Board* b, MoveList* l, Move move);
#endif