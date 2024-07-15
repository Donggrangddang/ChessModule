#include "board.h"
#include "legalMove.h"
#include "piece.h"
#include "move.h"
#include <stdio.h>



int convertToIndex(char file, char rank) {
	return (rank - '1') * 8 + (file - 'a');
}


char convertToSAN(int square) {
	return (square % 8) + 'a', (square / 8) + '1';
}

int convertPieceToIndex(char piece) {
    switch (piece) {
    case 'r': return Black | Rook;
    case 'n': return Black | Knight;
    case 'b': return Black | Bishop;
    case 'q': return Black | Queen;
    case 'k': return Black | King;
    case 'p': return Black | Pawn;
    case 'R': return White | Rook;
    case 'N': return White | Knight;
    case 'B': return White | Bishop;
    case 'Q': return White | Queen;
    case 'K': return White | King;
    case 'P': return White | Pawn;
    }
}


MoveResult doMove(Board* b, MoveList* l, Move move) {
    MoveResult result = { 0, -1, 0 }; // 초기화, -1은 enPassantSquare가 설정되지 않았음을 나타냄
    for (int i = 0; i < l->size; i++) {
        if ((move.startSquare == l->movesList[i].startSquare) && (move.targetSquare == l->movesList[i].targetSquare)) {
            int lastEnPassantSquare = b->enPassantSquare;
            int foundEnPassant = 0;

            for (int j = 0; j < 16; j++) {
                if (move.startSquare == enPassantSquare[j].startSquare && move.targetSquare == enPassantSquare[j].targetSquare) {
                    b->enPassantSquare = j;
                    foundEnPassant = 1;
                    break;
                }
            }

            if (!foundEnPassant) {
                b->enPassantSquare = -1;
            }

            int piece = b->square[move.startSquare];

            if (move.promotionPiece == -1) {
                b->square[move.targetSquare] = b->square[move.startSquare];
                result.promotionPieceColor = -1;
            }
            else {
                b->square[move.targetSquare] = move.promotionPiece;
                result.promotionPieceColor = isColor(b->square[move.startSquare], 1) ? White : Black;
            }
            
            b->square[move.startSquare] = None;
            b->turnToPlay = !(b->turnToPlay);
            b->numMoves++;
            result.capturedPiece = piece; // 값을 설정
            result.lastEnPassantSquare = lastEnPassantSquare; // 값을 설정

            return result;
        }
    }
    printf("illegal move\n");
    result = (MoveResult){ 0, -1, 0 };
    return result; // 이 경우, 초기화된 값(0, -1)을 반환
}



void undoMove(Board* b, Move lastMove, MoveResult moveResult) {
    if (moveResult.promotionPieceColor == -1) {
        b->square[lastMove.startSquare] = b->square[lastMove.targetSquare];
    }
    else {
        b->square[lastMove.startSquare] = moveResult.promotionPieceColor | Pawn;
    }
	b->square[lastMove.targetSquare] = moveResult.capturedPiece;
    b->enPassantSquare = moveResult.lastEnPassantSquare;
	b->turnToPlay = !(b->turnToPlay);
	b->numMoves--;
}