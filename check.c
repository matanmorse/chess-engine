// check.c
// functions to check if certain features are working correctly

#include "defs.h"

// check if bitboards are correct
void CheckBitBoard(S_BOARD *pos) {
    PrintBoard(pos);
    printf("\nwP:\n");
    PrintBitBoard(pos -> pawns[WHITE]);
    printf("\nbP:\n");
    PrintBitBoard(pos -> pawns[BLACK]);
    printf("\nBOTH:\n");
    PrintBitBoard(pos -> pawns[BOTH]);

}
