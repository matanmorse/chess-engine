// io.c

#include "defs.h"
#include "stdio.h"

// print a square in algebraic notation
char *PrSq(const int sq) {
    // do some assert to make sure we're not trying to parse squares that are offboard
    int sq64 = SQ64(sq);
    ASSERT(sq64 <= 64);
    static char SqStr[3];

    int rank = RanksBrd[sq];
    int file = FilesBrd[sq];
    
    sprintf(SqStr, "%c%c", ('a' + file), ('1' + rank));

    return SqStr;   
}

// print a move
char *PrMove(const int move) {
    // do some assert to make sure we're not trying to parse squares that are offboard
    int sq64 = SQ64(FROMSQ(move));
    ASSERT(sq64 <= 64);
    sq64 = SQ64(TOSQ(move));
    ASSERT(sq64 <= 64);

    static char MvStr[6];

    int ff = FilesBrd[FROMSQ(move)];
    int rf = RanksBrd[FROMSQ(move)];
    int ft = FilesBrd[TOSQ(move)];
    int rt = RanksBrd[TOSQ(move)];

    int promoted = PROMOTED(move);

    if (promoted) {
        char pchar = 'q';
        if (IsKn(promoted)) {
            pchar = 'n';
        } else if (IsRQ(promoted) && !IsBQ(promoted)) {
            pchar = 'r';
        } else if (IsBQ(promoted) && !IsRQ(promoted)) {
            pchar = 'b';
        }
        sprintf(MvStr, "%c%c%c%c%c", 'a' + ff, '1' + rf, 'a' + ft, '1' + rt, pchar);
    } else {
        sprintf(MvStr, "%c%c%c%c", 'a' + ff, '1' + rf, 'a' + ft, '1' + rt);
    }
    return MvStr;
}

void PrintMoveList(const S_MOVELIST *list) {
    int index;
    printf("Moves: \n");
    for ( index = 0; index < list -> count; index++ ) {
        
        printf("Move: %d > %s (Score: %d)\n", index + 1, PrMove( list -> moves[index].move) , list -> moves[index].score );
    }
    printf("Movelist total %d moves", list -> count);
}