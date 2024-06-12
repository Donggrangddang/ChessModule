#include "legalMove.h"

#ifndef interface_h
#define interface_h

char getPieceSymbol(int piece);

void printBoard(Board* b);

void printMoveList(MoveList* moveList);

void printFEN(Board* b);

void getSAN(char* buffer);

Move paresSAN(char* san);
#endif