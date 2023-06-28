// io.c

#include "defs.h"
#include "stdio.h"

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