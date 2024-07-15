#include "board.h"
#include "piece.h"
#include <stdio.h>
#include <string.h>

/**
* @brief ���� ó�� ���·� �ʱ�ȭ
* @param Board* b ������ �޸� �ּ�
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
    memset(b, 0, sizeof(Board)); // ���� �ʱ�ȭ

    int rank = 7;
    int file = 0;
    int section = 0; // FEN ���ڿ��� ���� ������ ����

    for (int i = 0; fen[i]; i++) {
        if (fen[i] == ' ') { // ���� ����
            section++;
            if (section == 1) {
                file = 0;
                rank = 7;
            }
            continue;
        }

        if (section == 0) { // �⹰ ��ġ
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
        else if (section == 1) { // ���� ��
            b->turnToPlay = (fen[i] == 'w') ? 1 : 0;
        }

        /*

        else if (section == 2) { // ĳ���� ����
            // ĳ���� ���� ���� �ڵ�...
            if (fen[i] == '-') continue; // ĳ���� ���� ����
            // ��: b->castleRights |= (fen[i] == 'K') ? CASTLE_KING_WHITE : 0;
        }
        */
        else if (section == 3) { // ���Ļ� ���
            if (fen[i] >= 'a' && fen[i] <= 'h') {
                b->enPassantSquare = (fen[i] - 'a') + (fen[i + 1] - '3') * 3 - 1 ; // 'a'���� 'h'������ ���ڸ� 0���� 7������ ���ڷ� ��ȯ
            }
            else {
                b->enPassantSquare = -1; // ���Ļ� ����� ������ ��Ÿ���� ��
            }

        }

        else if (section == 4) { // �� �̵� Ƚ��
            b->numMoves = (fen[i] - '0');
        }
    }
}
