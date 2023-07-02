// movegen.c

#include "stdio.h"
#include "defs.h"

#define SQOFFBOARD(sq) (FilesBrd[sq] == OFFBOARD) // check if a square is offboard
#define SQEMPTY(sq, pos) ( (pos -> pieces[sq]) == EMPTY)

void AddQuietMove (const S_BOARD *pos, int move, S_MOVELIST *list ) {
    list -> moves[list -> count].move = move;
    list -> moves[list -> count].score = 0;
    list -> count++;
}

void AddCaptureMove (const S_BOARD *pos, int move, S_MOVELIST *list ) {
    list -> moves[list -> count].move = move;
    list -> moves[list -> count].score = 0;
    list -> count++;
}

void AddEnPasMove (const S_BOARD *pos, int move, S_MOVELIST *list ) {
    list -> moves[list -> count].move = move;
    list -> moves[list -> count].score = 0;
    list -> count++;
}

void GenerateAllMoves(const S_BOARD *pos, S_MOVELIST *list) {
    ASSERT ( CheckBoard (pos) );

    list -> count = 0;

    int pce = EMPTY;
    int side = pos -> side;
    int sq = 0; int t_sq = 0;
    int pceNum = 0;

    if ( side == WHITE) {
        for ( pceNum = 0; pceNum < pos -> pceNum[wP]; pceNum++ ) { // loop over white pawns
                sq = pos -> pList[wP][pceNum]; // get the square the pawn is on
                ASSERT(SqOnBoard(sq)); // check square is on board
                GenerateWhitePawnMoves(pos, list, sq); // generate the moves for this piece
            }
    }
    else {
        for ( pceNum = 0; pceNum < pos -> pceNum[bP]; pceNum++ ) { // loop over black pawns
                sq = pos -> pList[bP][pceNum]; // get the square the pawn is on
                ASSERT(SqOnBoard(sq)); // check square is on board
                GenerateBlackPawnMoves(pos, list, sq); // generate the moves for this piece
            }
    }
    
}

void GenerateWhitePawnCapMove(const S_BOARD *pos, S_MOVELIST *list, int sq, int targ_sq) {
    int t_pce;
    t_pce = pos -> pieces[targ_sq]; // get piece on target square
    // check for promotion moves
    if ( RanksBrd[sq] == RANK_7 ) { // this move is a promotion
        AddCaptureMove(pos, MOVE(sq, targ_sq, t_pce, wN, 0), list); // add capture moves for each piece type
        AddCaptureMove(pos, MOVE(sq, targ_sq, t_pce, wR, 0), list); // add capture moves for each piece type
        AddCaptureMove(pos, MOVE(sq, targ_sq, t_pce, wB, 0), list); // add capture moves for each piece type
        AddCaptureMove(pos, MOVE(sq, targ_sq, t_pce, wQ, 0), list); // add capture moves for each piece type
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
            AddQuietMove(pos, MOVE(sq, targ_sq, EMPTY, wN, 0), list); // add each type of promotion
            AddQuietMove(pos, MOVE(sq, targ_sq, EMPTY, wR, 0), list);
            AddQuietMove(pos, MOVE(sq, targ_sq, EMPTY, wB, 0), list);
            AddQuietMove(pos, MOVE(sq, targ_sq, EMPTY, wQ, 0), list);

        } else { // make normal forward move
            move = MOVE(sq, targ_sq, EMPTY, EMPTY, 0 );
            AddQuietMove(pos, move, list);
        }
    }

    // generate double moves if on correct rank
    if ( RanksBrd[sq] == RANK_2 ) { // can do double move
        targ_sq = sq + 20; // look two ranks ahead
        if ( SQEMPTY(targ_sq, pos) ) {
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
    // check for promotion moves
    if ( RanksBrd[sq] == RANK_2 ) { // this move is a promotion
        AddCaptureMove(pos, MOVE(sq, targ_sq, t_pce, bN, 0), list); // add capture moves for each piece type
        AddCaptureMove(pos, MOVE(sq, targ_sq, t_pce, bR, 0), list); // add capture moves for each piece type
        AddCaptureMove(pos, MOVE(sq, targ_sq, t_pce, bB, 0), list); // add capture moves for each piece type
        AddCaptureMove(pos, MOVE(sq, targ_sq, t_pce, bQ, 0), list); // add capture moves for each piece type
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
            AddQuietMove(pos, MOVE(sq, targ_sq, EMPTY, wN, 0), list); // add each type of promotion
            AddQuietMove(pos, MOVE(sq, targ_sq, EMPTY, wR, 0), list);
            AddQuietMove(pos, MOVE(sq, targ_sq, EMPTY, wB, 0), list);
            AddQuietMove(pos, MOVE(sq, targ_sq, EMPTY, wQ, 0), list);

        } else { // make normal forward move
            move = MOVE(sq, targ_sq, EMPTY, EMPTY, 0 );
            AddQuietMove(pos, move, list);
        }
    }

    // generate double moves if on correct rank
    if ( RanksBrd[sq] == RANK_7 ) { // can do double move
        targ_sq = sq - 20; // look two ranks ahead
        if ( SQEMPTY(targ_sq, pos) ) {
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
