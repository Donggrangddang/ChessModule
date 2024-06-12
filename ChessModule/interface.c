#define _CRT_SECURE_NO_WARNINGS
#include "piece.h"
#include "board.h"
#include "legalMove.h"
#include "move.h"
#include <stdio.h>


/**
* @brief 숫자로 표시된 기물을 알파벳으로 변경
* @param int piece 기물
* @return 알파벳으로 표기된 기물
*/
char getPieceSymbol(int piece) {
    switch (piece & 7) {
    case King: return (piece & White) ? 'K' : 'k';  
    case Pawn: return (piece & White) ? 'P' : 'p';
    case Knight: return (piece & White) ? 'N' : 'n';
    case Bishop: return (piece & White) ? 'B' : 'b';
    case Rook: return (piece & White) ? 'R' : 'r';
    case Queen: return (piece & White) ? 'Q' : 'q';
    default: return '.';
    }
}

/**
* @brief 보드의 현 상태 출력
* @param Board* b 보드의 메모리 주소
*/
void printBoard(Board * b) {
    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
            int index = rank * 8 + file;
            char piece = getPieceSymbol(b->square[index]);
            printf("%c ", piece);
        }
        printf("\n");
    }
}


void printMoveList(MoveList* moveList) {
    printf("[");
    for (int i = 0; i < moveList->size; i++) {
        char startSquare[3] = { 'a' + (moveList->movesList[i].startSquare % 8), '1' + (moveList->movesList[i].startSquare / 8), '\0' };
        char targetSquare[3] = { 'a' + (moveList->movesList[i].targetSquare % 8), '1' + (moveList->movesList[i].targetSquare / 8), '\0' };
        printf(" %s%s,", startSquare, targetSquare);
    }
    printf("]\n");
}


void getSAN(char* buffer) {
    printf("Enter your Move: ");

    if (scanf("%4s", buffer) != 1) { // 최대 4글자 입력
        printf("Failed to read input.\n");
        buffer[0] = '\0'; // 실패 시 빈 문자열 반환
    }

    // 입력 버퍼 지우기
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

/**
* @brief 움직임을 쪼개 bitBoard에 맞는 형식으로 바꿔주는 함수
* @param char* san 변환시킬 움직임의 메모리 주소
* @return Move move 변환된 움직임
*/
Move paresSAN(char* san) {
    Move move = { convertToIndex(san[0], san[1]), convertToIndex(san[2], san[3]) };
    return move;
}


void printFEN(Board* b) {
    for (int rank = 7; rank >= 0; rank--) {
        int empty = 0;
        for (int file = 0; file < 8; file++) {
            int index = rank * 8 + file;
            if (b->square[index] == None) {
                empty++;
            }
            else {
                if (empty > 0) {
                    printf("%d", empty);
                    empty = 0;
                }
                printf("%c", getPieceSymbol(b->square[index]));
            }
        }
        if (empty > 0) {
            printf("%d", empty);
        }
        if (rank > 0) {
            printf("/");
        }
    }
    printf(" %s", b->turnToPlay ? "w" : "b");
    printf(" - -");
    printf(" %d %d\n", b->numMoves, (1 + b->numMoves / 2));
}
