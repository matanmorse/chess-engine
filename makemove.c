// makemove.c
#include "stdio.h"
#include "defs.h"

// simple helper macros to hash information into keys
#define HASH_PCE(pce, sq) (pos -> posKey ^= (PieceKeys[pce][sq]))
#define HASH_SIDE ( pos -> posKey ^= SideKey)
#define HASH_ENPAS ( pos -> posKey ^= PieceKeys[EMPTY][pos -> enPas])
#define HASH_CA (pos -> posKey ^= CastleKeys[pos -> castlePerm])


// helpful values for castling permissions
// when a piece moves to or from a square, the current castling permission can be bitwise &'d to the square in this array
// this will return the new castling permission, which will normally be unchanged except for specific squares
const int CastlePerm[120] = { 
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 13, 15, 15, 15, 12, 15, 15, 14, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15,  7, 15, 15, 15,  3, 15, 15, 11, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15
};

// remove a piece from the board
void ClearPce(S_BOARD *pos, const int sq) {
    int pce = pos -> pieces[sq]; // get piece on that square
    ASSERT (PieceValid(pce)); // sanity check
    ASSERT (SqOnBoard(sq));
    // take care of various S_BOARD attributes
    HASH_PCE(pce, sq); // update the position key

    int col = PieceCol[pce];

    pos -> material[col] -= PieceVal[pce];
    if (PieceBig[pce]) pos -> bigPce[col]--;
    if (PieceMaj[pce]) pos -> majPce[col]--;
    if (PieceMin[pce]) pos -> minPce[col]--;

    if ( pce == wP || pce == bP) { // handle pawn bitboards
        U64 BitBoard = pos -> pawns[col];
        U64 BothBoard = pos -> pawns[BOTH];
        CLRBIT(BitBoard, (SQ64(sq))); // remove the bit
        CLRBIT(BothBoard, (SQ64(sq)));
    }
    pos -> pieces[sq] == EMPTY; // remove the piece from the board
    
    // this loop updates the pList array
    // basically it loops through until it finds the piece in the array, then replaces it with the last piece in the array, maintaining a full array
    int t_pce = -1;
    int t_pceNum;
    for ( int index = 0; index < pos -> pceNum[pce]; index++ ) { // iterate over all pieces of that type
        if ( pos -> pList[pce][index] == sq) { // we've found the piece in the piece list
            t_pceNum = index;
            break;
        }
    }

    ASSERT(t_pce != -1);
    pos -> pceNum[pce]--; // this must be done after the pList is updated
    
    t_pce = pos -> pList[pce][pos -> pceNum[pce]];
    pos -> pList[pce][t_pceNum] == t_pce;

}

// add a piece to the board (promotion)
void AddPce(S_BOARD *pos, int pce, int sq) {

}

// move a piece from one square to another
void MovePce(S_BOARD *pos, int from, int to) {

}

// make a given move on the board
void MakeMove(S_BOARD *pos, int move) {

}