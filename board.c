// board.c

#include "stdio.h"
#include "defs.h"

void ResetBoard( S_BOARD *pos ) {
    int index = 0;
    // first, set all squares on the board to offboard.
    for ( index = 0; index < BRD_SQ_NUM; index++ ) {
        pos -> pieces[index] = OFFBOARD;
    }
    // then, loop 0 - 64, convert to 120 index and set to empty
    // this way we set all valid squares on the board to empty.
    for ( index = 0; index < 64; index++ ){
        pos -> pieces[SQ120(index)] = EMPTY;
    } 
    // set all piece counts for white, black, and both to zero
    for ( index = 0; index < 3; index++ ){
        pos -> bigPce[index] = 0;
        pos -> majPce[index] = 0;
        pos -> minPce[index] = 0;
        pos -> pawns[index] = 0ULL; // pawns must be set to U64 type bc bitboards
    }

    for ( index = 0; index < 13; index++ ) {
        pos -> pceNum[index] = 0;
    }
    // reset everything else
    pos -> KingSq[WHITE] = pos -> KingSq[BLACK] = NO_SQ; // neither kings are on a square

    pos -> side = BOTH;
    pos -> enPas = NO_SQ;
    pos -> fiftyMove = 0;

    pos -> ply = 0;
    pos -> hisPly = 0;

    pos -> castlePerm = 0;

    pos -> posKey = 0ULL; // there is no position key
}