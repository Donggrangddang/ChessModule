#include "board.h"

#ifndef move_h
#define move_h

#define MAX_SIZE 100

extern const int DirectionOffsets[8];
extern const int KnightDirectionOffsets[8];

typedef struct {
	short startSquare; // readonly
	short targetSquare; // readonly
} Move;

typedef struct {
	Move movesList[MAX_SIZE];
	int size;
} MoveList;

#endif