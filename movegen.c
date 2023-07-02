// movegen.c

#include "stdio.h"
#include "defs.h"

#define SQOFFBOARD(sq) (FilesBrd[sq] == OFFBOARD) // check if a square is offboard
#define SQEMPTY(sq, pos) ( (pos -> pieces[sq]) == EMPTY)


const int PceDir[13][8] = {
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ -8, -19,	-21, -12, 8, 19, 21, 12 },
	{ -9, -11, 11, 9, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ -8, -19,	-21, -12, 8, 19, 21, 12 },
	{ -9, -11, 11, 9, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 }
}; // values of directions pieces can move in

const int NumDir[13] = {
 0, 0, 8, 4, 4, 8, 8, 0, 8, 4, 4, 8, 8
}; // number of directions a piece has

// if side is white we start at wB and go til 0, black starts at bB, just helps
// loop through sliding pieces based on side more easily
const int LoopSlidePce[8] = {
    wB, wR, wQ, 0, bB, bR, bQ, 0
}; // array to help us loop through sliding pieces

const int LoopNonSlidePce[6] = {
 wN, wK, 0, bN, bK, 0
};

const int LoopSlideIndex[2] = { 0, 4 };  // where to start in LoopSlidePce based on side
const int LoopNonSlideIndex[2] = { 0, 3 };

static void AddQuietMove (const S_BOARD *pos, int move, S_MOVELIST *list ) {
    list -> moves[list -> count].move = move;
    list -> moves[list -> count].score = 0;
    list -> count++;
}

static void AddCaptureMove (const S_BOARD *pos, int move, S_MOVELIST *list ) {
    list -> moves[list -> count].move = move;
    list -> moves[list -> count].score = 0;
    list -> count++;
}

static void AddEnPasMove (const S_BOARD *pos, int move, S_MOVELIST *list ) {
    list -> moves[list -> count].move = move;
    list -> moves[list -> count].score = 0;
    list -> count++;
}

void GenerateSlidingMoves(const S_BOARD *pos, int sq, S_MOVELIST *list) {

    // get piece on square
    int pce = pos -> pieces[sq];
    ASSERT( ( IsBQ(pce) || IsRQ(pce) ) && ( PieceCol[pce] == pos -> side)); // sanity check

    // loop in each direction
    for ( int dir = 0; dir < NumDir[pce]; dir++ ) { // NumDir just contains the number of directions, so we can easily loop over each direction
        int t_sq = sq + PceDir[pce][dir]; // PceDir contains the list of directions a piece can move based on its type

        while (SqOnBoard(t_sq)) { // keep going in a direction until we hit the end of the board
            int t_pce = pos -> pieces[t_sq];
            ASSERT(SqOnBoard(t_sq)); 
            if ( t_pce == EMPTY ) { // if the square is empty we can move there normally
                AddQuietMove(pos, MOVE(sq, t_sq, EMPTY, EMPTY, 0), list);
            }
            else if ( PieceCol[t_pce] != pos -> side) { // if the piece is an enemy piece we can capture it
                AddCaptureMove(pos, MOVE(sq, t_sq, t_pce, EMPTY, 0), list);
                break; // if we hit a piece stop searching in this direction
            }
            else break; // if the space is occupied and not capturable stop searching in that direction

            t_sq += PceDir[pce][dir];
        }
    }
}      

void GenerateNonSlidingMoves(const S_BOARD *pos, int sq, S_MOVELIST *list) {
    int pce = pos -> pieces[sq]; // get piece on square
    ASSERT(IsKn(pce) || IsKi(pce));

    for ( int dir = 0; dir < NumDir[pce]; dir++ ) {
        int t_sq = sq + PceDir[pce][dir]; // get our target square based on the direction the piece moves in
        int t_pce = pos -> pieces[t_sq]; // get piece on taret square

        if (!SqOnBoard(t_sq)) continue;

        if ( t_pce == EMPTY ) { // if the square is empty we can move normally
            AddQuietMove(pos, MOVE(sq, t_sq, EMPTY, EMPTY, 0), list);
        }
        else if ( PieceCol[t_pce] != pos -> side ) { // if the piece is an enemy piece we can capture it
            AddCaptureMove(pos, MOVE(sq, t_sq, t_pce, EMPTY, 0), list);
        }
    }
}   


// dumb silly pawn stuff
void GenerateWhitePawnCapMove(const S_BOARD *pos, S_MOVELIST *list, int sq, int targ_sq) {
    int t_pce;
    t_pce = pos -> pieces[targ_sq]; // get piece on target square
    ASSERT(PieceValidEmpty(t_pce));
    ASSERT(SqOnBoard(sq));
    ASSERT(SqOnBoard(targ_sq));

    // check for promotion moves
    if ( RanksBrd[sq] == RANK_7 ) { // this move is a promotion
        AddCaptureMove(pos, MOVE(sq, targ_sq, t_pce, wN, MFLAGPROM), list); // add capture moves for each piece type
        AddCaptureMove(pos, MOVE(sq, targ_sq, t_pce, wR, MFLAGPROM), list); // add capture moves for each piece type
        AddCaptureMove(pos, MOVE(sq, targ_sq, t_pce, wB, MFLAGPROM), list); // add capture moves for each piece type
        AddCaptureMove(pos, MOVE(sq, targ_sq, t_pce, wQ, MFLAGPROM), list); // add capture moves for each piece type
    } else {
        // otherwise, just add normal capture move
        AddCaptureMove(pos, MOVE(sq, targ_sq, t_pce, EMPTY, 0), list);
    }

}

// given a piece and a board, generate all moves for that piece
void GenerateWhitePawnMoves(const S_BOARD *pos, S_MOVELIST *list, int sq) {
    // check pawn on that square
    ASSERT (pos -> pieces[sq] == wP);

    // generate simple forward move
    int targ_sq, move;
    targ_sq = sq + 10;
    if ( pos -> pieces[sq + 10] == EMPTY && SqOnBoard(sq)) { // if the square is free
        // check for promotion
        if ( RanksBrd[sq] == RANK_7 ) {
            AddQuietMove(pos, MOVE(sq, targ_sq, EMPTY, wN, MFLAGPROM), list); // add each type of promotion
            AddQuietMove(pos, MOVE(sq, targ_sq, EMPTY, wR, MFLAGPROM), list);
            AddQuietMove(pos, MOVE(sq, targ_sq, EMPTY, wB, MFLAGPROM), list);
            AddQuietMove(pos, MOVE(sq, targ_sq, EMPTY, wQ, MFLAGPROM), list);

        } else { // make normal forward move
            move = MOVE(sq, targ_sq, EMPTY, EMPTY, 0 );
            AddQuietMove(pos, move, list);
        }
    }

    // generate double moves if on correct rank
    if ( RanksBrd[sq] == RANK_2 ) { // can do double move
        targ_sq = sq + 20; // look two ranks ahead
        if ( SQEMPTY(targ_sq, pos) && SQEMPTY(sq + 10, pos) ) {
            move = MOVE (sq, targ_sq, EMPTY, EMPTY, MFLAGPS);
            AddQuietMove(pos, move, list);
        }
    }

    // generate capture moves
    targ_sq = sq + 11; 
    if ( SqOnBoard(targ_sq) && ( pos -> pieces[targ_sq] != EMPTY) ) { // valid capture square
        if ( PieceCol[ pos -> pieces[targ_sq]] == BLACK) { // if piece is enemy piece
             GenerateWhitePawnCapMove(pos, list, sq, targ_sq);
        }
    }
    else if ( pos -> enPas == targ_sq && ( RanksBrd[sq] == RANK_5)) { // if the target square is en passant and correct rank we can en passant capture
        int capturedPce = pos -> pieces[targ_sq - 10]; // the en passant piece is just a rank ahead
        ASSERT (capturedPce != EMPTY); // sanity check
        move = MOVE(sq, targ_sq, capturedPce, EMPTY, MFLAGEP); 
        AddEnPasMove(pos, move, list);
    }

    targ_sq = sq + 9; 
    if ( SqOnBoard(targ_sq) && ( pos -> pieces[targ_sq] != EMPTY) ) { // valid capture square
        if ( PieceCol[ pos -> pieces[targ_sq]] == BLACK) { // if piece is enemy piece
             GenerateWhitePawnCapMove(pos, list, sq, targ_sq);
        }
    }
    else if ( pos -> enPas == targ_sq && ( RanksBrd[sq] == RANK_5)) { // if the target square is en passant and correct rank we can en passant capture
        int capturedPce = pos -> pieces[targ_sq - 10]; // the en passant piece is just a rank ahead
        ASSERT (capturedPce != EMPTY); // sanity check
        move = MOVE(sq, targ_sq, capturedPce, EMPTY, MFLAGEP); 
        AddEnPasMove(pos, move, list);
    }
    
}

void GenerateBlackPawnCapMove(const S_BOARD *pos, S_MOVELIST *list, int sq, int targ_sq) {
    int t_pce;
    t_pce = pos -> pieces[targ_sq]; // get piece on target square
    ASSERT(PieceValidEmpty(t_pce));
    ASSERT(SqOnBoard(sq));
    ASSERT(SqOnBoard(targ_sq));

    // check for promotion moves
    if ( RanksBrd[sq] == RANK_2 ) { // this move is a promotion
        AddCaptureMove(pos, MOVE(sq, targ_sq, t_pce, bN, MFLAGPROM), list); // add capture moves for each piece type
        AddCaptureMove(pos, MOVE(sq, targ_sq, t_pce, bR, MFLAGPROM), list); // add capture moves for each piece type
        AddCaptureMove(pos, MOVE(sq, targ_sq, t_pce, bB, MFLAGPROM), list); // add capture moves for each piece type
        AddCaptureMove(pos, MOVE(sq, targ_sq, t_pce, bQ, MFLAGPROM), list); // add capture moves for each piece type
    } else {
        // otherwise, just add normal capture move
        AddCaptureMove(pos, MOVE(sq, targ_sq, t_pce, EMPTY, 0), list);
    }

}

void GenerateBlackPawnMoves(const S_BOARD *pos, S_MOVELIST *list, int sq) {
    // check pawn on that square
    ASSERT (pos -> pieces[sq] == bP);

    // generate simple forward move
    int targ_sq, move;
    targ_sq = sq - 10;
    if ( pos -> pieces[sq - 10] == EMPTY && SqOnBoard(sq)) { // if the square is free
        // check for promotion
        if ( RanksBrd[sq] == RANK_2 ) {
            AddQuietMove(pos, MOVE(sq, targ_sq, EMPTY, wN, MFLAGPROM), list); // add each type of promotion
            AddQuietMove(pos, MOVE(sq, targ_sq, EMPTY, wR, MFLAGPROM), list);
            AddQuietMove(pos, MOVE(sq, targ_sq, EMPTY, wB, MFLAGPROM), list);
            AddQuietMove(pos, MOVE(sq, targ_sq, EMPTY, wQ, MFLAGPROM), list);

        } else { // make normal forward move
            move = MOVE(sq, targ_sq, EMPTY, EMPTY, 0 );
            AddQuietMove(pos, move, list);
        }
    }

    // generate double moves if on correct rank
    if ( RanksBrd[sq] == RANK_7 ) { // can do double move
        targ_sq = sq - 20; // look two ranks ahead
        if ( SQEMPTY(targ_sq, pos) && SQEMPTY(sq - 10, pos) ) {
            move = MOVE (sq, targ_sq, EMPTY, EMPTY, MFLAGPS);
            AddQuietMove(pos, move, list);
        }
    }

    // generate capture moves
    targ_sq = sq - 11; 
    if ( SqOnBoard(targ_sq) && ( pos -> pieces[targ_sq] != EMPTY) ) { // valid capture square
        if ( PieceCol[ pos -> pieces[targ_sq]] == WHITE) { // if piece is enemy piece
             GenerateBlackPawnCapMove(pos, list, sq, targ_sq);
        }
    }
    else if ( pos -> enPas == targ_sq && ( RanksBrd[sq] == RANK_4)) { // if the target square is en passant and correct rank we can en passant capture
        int capturedPce = pos -> pieces[targ_sq + 10]; // the en passant piece is just a rank behind
        ASSERT (capturedPce != EMPTY); // sanity check
        move = MOVE(sq, targ_sq, capturedPce, EMPTY, MFLAGEP); 
        AddEnPasMove(pos, move, list);
    }

    targ_sq = sq - 9; 
    if ( SqOnBoard(targ_sq) && ( pos -> pieces[targ_sq] != EMPTY) ) { // valid capture square
        if ( PieceCol[ pos -> pieces[targ_sq]] == WHITE) { // if piece is enemy piece
             GenerateBlackPawnCapMove(pos, list, sq, targ_sq);
        }
    }
    else if ( pos -> enPas == targ_sq && ( RanksBrd[sq] == RANK_4)) { // if the target square is en passant and correct rank we can en passant capture
        int capturedPce = pos -> pieces[targ_sq + 10]; // the en passant piece is just a rank ahead
        ASSERT (capturedPce != EMPTY); // sanity check
        move = MOVE(sq, targ_sq, capturedPce, EMPTY, MFLAGEP); 
        AddEnPasMove(pos, move, list);
    }
    
}

void GenerateAllMoves(const S_BOARD *pos, S_MOVELIST *list) {
    ASSERT ( CheckBoard (pos) );

    list -> count = 0;

    int pce = EMPTY;
    int side = pos -> side;
    int sq = 0; int t_sq = 0;
    int pceNum = 0;
    int dir = 0;
    int index = 0;
    int pceIndex = 0;

    printf("\n\nSide: %c\n", SideChar[side]);

    if ( side == WHITE) {
        for ( pceNum = 0; pceNum < pos -> pceNum[wP]; pceNum++ ) { // loop over white pawns
                sq = pos -> pList[wP][pceNum]; // get the square the pawn is on
                ASSERT(SqOnBoard(sq)); // check square is on board
                GenerateWhitePawnMoves(pos, list, sq); // generate the moves for this piece
            }
        
        // generate castling moves
        if ( pos -> castlePerm & WKCA) { // if white has castle permission
        printf("White side king castling permission \n");
            if ( pos -> pieces[F1] == EMPTY && pos -> pieces[G1] == EMPTY ) { // if f1 and g1 are empty
            printf("g1 f1 empty \n");
                if (!SqAttacked(E1, BLACK, pos) && !SqAttacked(F1, BLACK, pos)) { // and neither of the squares are attacked and king is not in check
                    printf("Squares not attacked \n");
                    printf("Generating castling move \n");
                    AddQuietMove(pos, MOVE(E1, G1, EMPTY, EMPTY, MFLAGCA), list);
                }
            }
        }
        if ( pos -> castlePerm & WQCA) { // if white has castle permission
            if ( pos -> pieces[D1] == EMPTY && pos -> pieces[C1] == EMPTY && pos -> pieces[B1] == EMPTY ) { // if b1/c1/d1 are empty
                if (!SqAttacked(E1, BLACK, pos) && !SqAttacked(D1, BLACK, pos)) { // and none of the squares are attacked and king is not in check
                    printf("Generating castling move \n");                
                    AddQuietMove(pos, MOVE(E1, C1, EMPTY, EMPTY, MFLAGCA), list);
                }
            }
        }
    }
    else {
        for ( pceNum = 0; pceNum < pos -> pceNum[bP]; pceNum++ ) { // loop over black pawns
                sq = pos -> pList[bP][pceNum]; // get the square the pawn is on
                ASSERT(SqOnBoard(sq)); // check square is on board
                GenerateBlackPawnMoves(pos, list, sq); // generate the moves for this piece
            }

        // generate castling moves
        if ( pos -> castlePerm & BKCA) { // if black has castle permission
            if ( pos -> pieces[F8] == EMPTY && pos -> pieces[G8] == EMPTY ) { // if squares are empty
                if (!SqAttacked(E8, WHITE, pos) && !SqAttacked(F8, WHITE, pos)) { // and neither of the squares are attacked and king is not in check
                    printf("Generating castling move \n");            
                    AddQuietMove(pos, MOVE(E8, G8, EMPTY, EMPTY, MFLAGCA), list);
                }
            }
        }
        if ( pos -> castlePerm & BQCA) { // if white has castle permission
            if ( pos -> pieces[D8] == EMPTY && pos -> pieces[C8] == EMPTY && pos -> pieces[B8] == EMPTY ) { // if squares are empty
                if (!SqAttacked(E8, WHITE, pos) && !SqAttacked(D8, WHITE, pos)) { // and none of the squares are attacked and king is not in check
                    printf("Generating castling move \n");          
                    AddQuietMove(pos, MOVE(E8, C8, EMPTY, EMPTY, MFLAGCA), list);
                }
            }
        }
    }
    /* Loop for Slide Pieces */
    pceIndex = LoopSlideIndex[side]; // we start on the appropriate piece for the side
    pce = LoopSlidePce[pceIndex++]; // POST-Increment, so pceindex only gets incremented after we get the correct piece

    while ( pce != 0 )
    {
        ASSERT(PieceValid(pce));

        // loop over all pieces of that type
        for ( pceNum = 0; pceNum < pos -> pceNum[pce]; pceNum++) {
            int sq = pos -> pList[pce][pceNum];
            GenerateSlidingMoves(pos, sq, list);
        }

        pce = LoopSlidePce[pceIndex++];
    }
    
    /* Loop for non slide */
    pceIndex = LoopNonSlideIndex[side];
    pce = LoopNonSlidePce[pceIndex++];

    while ( pce != 0 ) {
        ASSERT(PieceValid(pce));
        
        // loop over all pieces of that type
        for ( pceNum = 0; pceNum < pos -> pceNum[pce]; pceNum++) {
            int sq = pos -> pList[pce][pceNum];
            GenerateNonSlidingMoves(pos, sq, list);
        }

        // move on to next non-slider piece
        pce = LoopNonSlidePce[pceIndex++];
    }
}
