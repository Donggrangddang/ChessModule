#include "legalMove.h"

#ifndef interface_h
#define interface_h

char getPieceSymbol(int piece);

void printBoard(Board* b);

void printMove(Move move);

void printMoveList(MoveList* moveList);

void getSAN(char* buffer);

void isAttacked(int index, int* attackMap);

void printAttackMap(int* attackMap);

Move parseSAN(char* san);

void printFEN(Board* b);

void printCheckMate(Board* b);

void printStaleMate(Board* b);
#endif