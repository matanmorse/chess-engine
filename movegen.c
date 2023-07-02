// movegen.c

#include "stdio.h"
#include "defs.h"

#define SQOFFBOARD(sq) (FilesBrd[sq] == OFFBOARD) // check if a square is offboard
#define SQEMPTY(sq, pos) ( (pos -> pieces[sq]) == EMPTY)

void AddQuietMove (const S_BOARD *pos, int move, S_MOVELIST *list ) {
    list -> moves[list -> count].move = move;
    list -> moves[list -> count].score = 0;
    list -> count++;
    printf("Added Quiet move: %s\n", PrMove(move));
}

void AddCaptureMove (const S_BOARD *pos, int move, S_MOVELIST *list ) {
    list -> moves[list -> count].move = move;
    list -> moves[list -> count].score = 0;
    list -> count++;
    printf("Added Capture move: %s\n", PrMove(move));
}

void AddEnPasMove (const S_BOARD *pos, int move, S_MOVELIST *list ) {
    list -> moves[list -> count].move = move;
    list -> moves[list -> count].score = 0;
    list -> count++;
    printf("Added enpas move: %s\n", PrMove(move));
}

void GenerateAllMoves(const S_BOARD *pos, S_MOVELIST *list) {

    list -> count = 0;
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
    if ( pos -> pieces[sq + 10] == EMPTY && !SQOFFBOARD(sq)) { // if the square is free
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
    if ( !SQOFFBOARD(targ_sq) && ( pos -> pieces[targ_sq] != EMPTY) ) { // valid capture square
        if ( PieceCol[ pos -> pieces[targ_sq]] == BLACK) { // if piece is enemy piece
             GenerateWhitePawnCapMove(pos, list, sq, targ_sq);
        }
    }
    else if ( pos -> enPas == targ_sq && ( RanksBrd[sq] == RANK_5)) { // if the target square is en passant and correct rank we can en passant capture
        int capturedPce = pos -> pieces[targ_sq - 10]; // the en passant piece is just a rank ahead
        move = MOVE(sq, targ_sq, capturedPce, EMPTY, MFLAGEP); 
        AddEnPasMove(pos, move, list);
    }

    targ_sq = sq + 9; 
    if ( !SQOFFBOARD(targ_sq) && ( pos -> pieces[targ_sq] != EMPTY) ) { // valid capture square
        if ( PieceCol[ pos -> pieces[targ_sq]] == BLACK) { // if piece is enemy piece
             GenerateWhitePawnCapMove(pos, list, sq, targ_sq);
        }
    }
    else if ( pos -> enPas == targ_sq && ( RanksBrd[sq] == RANK_5)) { // if the target square is en passant and correct rank we can en passant capture
        int capturedPce = pos -> pieces[targ_sq - 10]; // the en passant piece is just a rank ahead
        move = MOVE(sq, targ_sq, capturedPce, EMPTY, MFLAGEP); 
        AddEnPasMove(pos, move, list);
    }
    
}