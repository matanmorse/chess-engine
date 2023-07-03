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
static void ClearPce(S_BOARD *pos, const int sq) {
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

    pos -> pieces[sq] = EMPTY;
}

// add a piece to the board (promotion)
static void AddPce(S_BOARD *pos, const int pce, const int sq) {
    ASSERT (PieceValid(pce));
    ASSERT (SqOnBoard(sq));
    ASSERT ( (RanksBrd[sq] == RANK_1 && PieceCol[pce] == BLACK) || (RanksBrd[sq] == RANK_8 && PieceCol[pce] == WHITE)); // make sure the right color is promoting on the last rank

    HASH_PCE(pce, sq); // hash piece into key

    int col = PieceCol[pce];

    pos -> material[col] += PieceVal[pce];
    if (PieceBig[pce]) pos -> bigPce[col]++;
    if (PieceMaj[pce]) pos -> majPce[col]++;
    if (PieceMin[pce]) pos -> minPce[col]++;

    if ( pce == wP || pce == bP) { // handle pawn bitboards
        U64 BitBoard = pos -> pawns[col];
        U64 BothBoard = pos -> pawns[BOTH];
        SETBIT(BitBoard, (SQ64(sq))); // set the bit
        SETBIT(BothBoard, (SQ64(sq)));
    }
    
    pos -> pList[pce][pos -> pceNum[pce]] = sq;

    pos -> pieces[sq] == pce; // add the piece
    pos -> pceNum[pce]++; // need to increment after because pcenum is not zero indexed, so pList[pce][pceNum[pce]] actually gives the first empty value as intended
}

// move a piece from one square to another
void MovePce(S_BOARD *pos, const int from, const int to) {
    ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));

    int pce = pos -> pieces[from];
    HASH_PCE(pce, from); // hash out the old value
    pos -> pieces[from] = EMPTY; // remove the piece from its old square
    
    HASH_PCE(pce, to); // hash in the new value
    pos -> pieces[to] = pce; // add it to its new square

    // handle pawn bitboards
    if ( pce == wP || pce == bP ) {
        U64 BitBoard = pos -> pawns[PieceCol[pce]];
        U64 BitBoth = pos -> pawns[BOTH];
        // clear and set appropriate bits on bitboards
        CLRBIT(BitBoard, SQ64(from));
        SETBIT(BitBoard, SQ64(to));

        CLRBIT(BitBoth, SQ64(from));
        SETBIT(BitBoth, SQ64(to));
    }

    // handle king positions
    if (IsKi(pce)) {
        pos -> KingSq[PieceCol[pce]] == to;
    }

    

#ifdef DEBUG 
    int t_pceNum = FALSE; // when debugging we want to check if we found a piece
#endif

    // handle pList
    for ( int index = 0; index < pos -> pceNum[pce]; index++ ) {
        if ( pos -> pList[pce][index] == from ) { // find old piece in plist
            pos -> pList[pce][index] == to; // replace it with its new position
        #ifdef DEBUG
            int t_pceNum = TRUE; // we found a piece like we should so set to true
        #endif
        }
    }
    ASSERT(t_pceNum);
}

// make a given move on the board
int MakeMove(S_BOARD *pos, int move) {

    ASSERT(CheckBoard(pos));

    int from = FROMSQ(move);
    int to = TOSQ(move);
    int side = pos -> side;

    // validate move data
    ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));
    ASSERT(SideValid(side));
    ASSERT(PieceValid(pos -> pieces[from]));    
    // hisply is number of half-moves played in a game
    pos -> history[ pos -> hisPly].posKey = pos -> posKey; // set posKey into history poskey before we hash in/out any pieces

    // handle flags
    if ( move & MFLAGEP ) { // if the move is an en passant move
        if (side == WHITE) {
            ClearPce(pos, to - 10); // remove the piece behind the target square
        }
        else {
            ClearPce(pos, to + 10); // remove the piece ahead the target square
        }
    }
    else if ( move & MFLAGCA) { // when we're making a castling move, we must also move the appropriate rook to the correct square
        switch(to) {
            case C1:
                MovePce(pos, A1, D1); // white queen side castle
                break;
            case C8: 
                MovePce(pos, A8, D8); // black queen side castle
                break;
            case G1: 
                MovePce(pos, H1, F1); // white king side castle
                break;
            case G8:
                MovePce(pos, H8, F8); // black king side castle
                break;
            default: ASSERT(FALSE); break;
        }
    }

    if ( pos -> enPas != NO_SQ ) HASH_ENPAS; // we hash out the old en passant square because we are about to remove it 
    HASH_CA; // hash out old castling permissions

    // before we start to change the position's values, we store old values in the history array
    pos -> history[pos -> hisPly].move = move;
    pos -> history[pos -> hisPly].fiftyMove = pos -> fiftyMove;
    pos -> history[pos -> hisPly].enPas = pos -> enPas;
    pos -> history[pos -> hisPly].castlePerm = pos -> castlePerm;

    pos -> castlePerm &= CastlePerm[from]; // use the CastlePerm array to hash out castling permissions if any rooks/kings have moved
    pos -> castlePerm &= CastlePerm[to];
    pos -> enPas = NO_SQ; // unless we made a double pawn move there is no enpas square

    HASH_CA; // hash in new castling state

    int captured = CAPTURED(move);
    pos -> fiftyMove++; // if there is no capture increment fifty-move rule
    if ( captured != EMPTY ) {
        ASSERT(PieceValid(captured)); // sanity check
        ClearPce(pos, to); // remove the captured piece from the square
        pos -> fiftyMove = 0; // if a capture was made reset the fiftymove counter
    }
    
    pos -> hisPly++; // now that we're done saving info we can increment the number of moves
    pos -> ply++; // increment ply used during search

    // code for setting new en passant square
    if (pos -> pieces[from] == wP || pos -> pieces[from] == bP) { // if the piece thats moving is a pawn
        pos -> fiftyMove = 0; // pawn moves reset fiftymove counter
        if ( move & MFLAGPS ) {
            if ( side == WHITE ) {
                pos -> enPas = from + 10;
                ASSERT(RanksBrd[pos -> enPas] == RANK_3);
            }
            else {
                pos -> enPas = from - 10;
                ASSERT(RanksBrd[pos -> enPas] == RANK_6);
            }
            HASH_ENPAS; // hash new en passant square
        }
    }

    MovePce(pos, from, to);

    int prPce = PROMOTED(move);

    if (prPce != EMPTY) {
        if (side == WHITE) {
            ASSERT(RanksBrd[to] == RANK_8);
            ClearPce(pos, to);
            AddPce(pos, prPce, to);
        }
        else {
            ASSERT (RanksBrd[to] == RANK_1);
            ClearPce(pos, to);
            AddPce(pos, prPce, to);
        }
    }

    if (PieceKing[pos -> pieces[to]]) {
        pos -> KingSq[side] = to; // if we moved a king, update the KingSq array
    }

    pos -> side ^= 1; // change the side
    HASH_SIDE;

    ASSERT(CheckBoard(pos)); // pray

    if (SqAttacked(pos -> KingSq[side], pos -> side, pos)) { // if the king square is under attack the move is illegal
        // TakeMove(pos);
        return FALSE; 
    }

    return TRUE;

}