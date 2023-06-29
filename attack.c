// attack.c

#include "defs.h"
// define arrays of int values pieces can move to
const int KnDir[8] = { -8, -19, -21, -12, 8, 19, 21, 12}; // numbers of squares from which a knight can move/attack
const int RkDir[4] = { -1, -10, 1, 10 }; // squares a rook can move/attack
const int BiDir[4] = {-9, -11, 11, 9 }; // squares a bishop can move/attack from
const int KiDir[8] = { -1, -10, 1, 10, -9, -11, 11, 9 }; // squares king can attack from

// inst side is ATTACKING side not attacked side.
// to check if a square is being attacked by a given side in a given position
int SqAttacked (const int sq, const int side, const S_BOARD *pos) {
    
    int pce, index, t_sq, dir;

    ASSERT (SqOnBoard(sq));
    ASSERT (SideValid(side));
    ASSERT(CheckBoard(pos));
    
    // check if pawns are attacking
    if ( side == WHITE ) {
        // for white, pawns can only attack a square -9 or -11 squares away so we check if there is a white pawn there
        if ( pos -> pieces[sq - 11] == wP || pos -> pieces[sq - 9] == wP ) return TRUE; 
    } else { // the attacking side is black
        if ( pos -> pieces[sq + 11] == bP || pos -> pieces[ sq + 9] == bP) return TRUE; // check opposite for black side
    }

    // check knights
    for ( index = 0; index < 8; index++ ) {
       pce = pos -> pieces[sq + KnDir[index]]; // find what piece is on a knight move away from the piece
       if ( IsKn(pce) && ( PieceCol[pce] == side )) return TRUE; // if the piece is the knight and piece color matches attacking side square is attacked
    }
    
    // rooks, queens
    for ( index = 0; index < 4; index++ ) {
        dir = RkDir[index];
        t_sq = sq + dir; // t_sq is square we're searching for plus direction
        while ( t_sq != OFFBOARD ) { // we keep looping this until we move off the board
            pce = pos -> pieces[t_sq]; 

            // if there is piece on the square it must either be a rook/queen of correct color (square is attacked) or the square must be blocked
            if ( pce != EMPTY ) { 
                if ( IsRQ(pce) && PieceCol[pce] == side ) return TRUE;
                break; // square has non-rook/queen piece so this direction is blocked and move on to next direction
            }

            t_sq += dir; // move one more step in direction
        }
    }

    // bishops, queens
    for ( index = 0; index < 4; index++ ) {
        dir = BiDir[index];
        t_sq = sq + dir;
        while ( t_sq != OFFBOARD) {
            pce = pos -> pieces[t_sq];
            if ( pce != EMPTY ) {
                if ( IsBQ(pce) && PieceCol[pce] == side ) return TRUE;
                break;
            }
            
            t_sq += dir;
        }
    }

    // kings
    for ( index = 0; index < 8; index++ ) {
        t_sq = sq + KiDir[index];
        pce = pos -> pieces[t_sq];
        if ( IsKi(pce)  && PieceCol[pce] == side ) return TRUE;
    }

    return FALSE;
}