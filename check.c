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

// check if SqAttacked function is correct
void ShowSqAttackedBySide(const int side, const S_BOARD *pos) {
    int rank = 0;
    int file = 0;
    int sq = 0;

    printf("\nSquares Attacked By %c: \n\n", SideChar[side]);
    for ( rank = RANK_8; rank >= RANK_1; rank-- ) {
        for ( file = FILE_A; file <= FILE_H; file++ ) {
            sq = FR2SQ( file, rank );
            if ( SqAttacked(sq, side, pos)) printf("x ");
            else printf("- ");
        }
        printf("\n");
    }
    printf("\n\n");
}