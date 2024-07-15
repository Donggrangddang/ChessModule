#include "board.h"
#include "piece.h"
#include <stdio.h>
#include <string.h>

/**
* @brief 보드 처음 상태로 초기화
* @param Board* b 보드의 메모리 주소
*/
void initializeBoard(Board* b) {

    b->turnToPlay = 1;
    b->numMoves = 0;

    for (int i = 0; i < BOARD_SIZE; i++) {
        b->square[i] = None;
    }

    b->square[0] = White | Rook;
    b->square[1] = White | Knight;
    b->square[2] = White | Bishop;
    b->square[3] = White | Queen;
    b->square[4] = White | King;
    b->square[5] = White | Bishop;
    b->square[6] = White | Knight;
    b->square[7] = White | Rook;

    for (int i = 8; i < 16; i++) {
        b->square[i] = White | Pawn;
    }

    b->square[56] = Black | Rook;
    b->square[57] = Black | Knight;
    b->square[58] = Black | Bishop;
    b->square[59] = Black | Queen;
    b->square[60] = Black | King;
    b->square[61] = Black | Bishop;
    b->square[62] = Black | Knight;
    b->square[63] = Black | Rook;

    for (int i = 48; i < 56; i++) {
        b->square[i] = Black | Pawn;
    }
}


void setBoardFromFEN(Board* b, const char* fen) {
    memset(b, 0, sizeof(Board)); // 보드 초기화

    int rank = 7;
    int file = 0;
    int section = 0; // FEN 문자열의 현재 섹션을 추적

    for (int i = 0; fen[i]; i++) {
        if (fen[i] == ' ') { // 섹션 구분
            section++;
            if (section == 1) {
                file = 0;
                rank = 7;
            }
            continue;
        }

        if (section == 0) { // 기물 배치
            if (fen[i] == '/') {
                rank--;
                file = 0;
            }
            else if (fen[i] >= '1' && fen[i] <= '8') {
                file += fen[i] - '0';
            }
            else {
                int square = rank * 8 + file;
                switch (fen[i]) {
                    case 'r': b->square[square] = Black | Rook; break;
                    case 'n': b->square[square] = Black | Knight; break;
                    case 'b': b->square[square] = Black | Bishop; break;
                    case 'q': b->square[square] = Black | Queen; break;
                    case 'k': b->square[square] = Black | King; break;
                    case 'p': b->square[square] = Black | Pawn; break;
                    case 'R': b->square[square] = White | Rook; break;
                    case 'N': b->square[square] = White | Knight; break;
                    case 'B': b->square[square] = White | Bishop; break;
                    case 'Q': b->square[square] = White | Queen; break;
                    case 'K': b->square[square] = White | King; break;
                    case 'P': b->square[square] = White | Pawn; break;
                }
                file++;
            }
        }
        else if (section == 1) { // 현재 턴
            b->turnToPlay = (fen[i] == 'w') ? 1 : 0;
        }

        /*

        else if (section == 2) { // 캐슬링 권한
            // 캐슬링 권한 설정 코드...
            if (fen[i] == '-') continue; // 캐슬링 권한 없음
            // 예: b->castleRights |= (fen[i] == 'K') ? CASTLE_KING_WHITE : 0;
        }
        */
        else if (section == 3) { // 앙파상 대상
            if (fen[i] >= 'a' && fen[i] <= 'h') {
                b->enPassantSquare = (fen[i] - 'a') + (fen[i + 1] - '3') * 3 - 1 ; // 'a'부터 'h'까지의 문자를 0부터 7까지의 숫자로 변환
            }
            else {
                b->enPassantSquare = -1; // 앙파상 대상이 없음을 나타내는 값
            }

        }

        else if (section == 4) { // 반 이동 횟수
            b->numMoves = (fen[i] - '0');
        }
    }
}
