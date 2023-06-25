#include "stdio.h"
#include "defs.h"

int main (void) {
    AllInit();
    // empty bit board
    U64 playBitBoard = 0ULL;
    
    int index = 0;
    int index2 = 0;
    for ( index = 0; index < 13; index++ ) {
        for ( index2 = 0; index2 < 120; index2++ ) {
            printf("Piece Key %d %d: %llu \n", index, index2, PieceKeys[index][index2]);
        }
    }

    ASSERT(0 == 1); 
    return 0;
}