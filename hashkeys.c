// hashkeys.c

#include "defs.h"

U64 GeneratePosKey(const S_BOARD *pos) {
    int sq = 0;
    U64 finalKey = 0;
    int piece = EMPTY;

    // pieces
    for ( sq = 0; sq < BRD_SQ_NUM; sq++ ) {
        // get integer value of piece (including no piece and squares outside the board) from the board struct
        piece = pos -> pieces[sq];
        // if the piece is not empty or outside bounds
        if ( piece != NO_SQ && piece != EMPTY ) {
            ASSERT( piece >= wP && piece <= bK); // double check integer value of piece is between 1-12
            finalKey ^= PieceKeys[piece][sq]; // hash the piece into the final key
        }

        if ( pos -> side == WHITE) {
            finalKey ^= SideKey; // we only hash the side key if it's white to play
        }

        if ( pos -> enPas != NO_SQ ) {
            ASSERT( pos -> enPas >= 0 && pos -> enPas < BRD_SQ_NUM); // double check en passant square is within bounds
            finalKey ^= PieceKeys[EMPTY][pos -> enPas]; // hash in the key
        }

        ASSERT( pos -> castlePerm >= 0 && pos -> castlePerm <= 15 ); // double check castling permission values

        finalKey ^= CastleKeys[pos -> castlePerm]; // hash in castling permissions
    }
}