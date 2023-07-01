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

    list -> count = 0;
}

// given a piece and a board, generate all moves for that piece
void GenerateWhitePawnMove(const S_BOARD *pos, S_MOVELIST *list, int sq) {
    // check pawn on that square
    ASSERT (pos -> pieces[sq] == wP);
    // generate simple forward move
    int targ_sq, move;
    targ_sq = sq + 10;
    if ( pos -> pieces[sq + 10] == EMPTY && !SQOFFBOARD(sq)) { // if the square is free
        move = MOVE(sq, targ_sq, EMPTY, EMPTY, 0 );
        AddQuietMove(pos, move, list);
        printf("Added move: %s\n", PrMove(move));
    }

    // generate double moves if on correct rank
    if ( RanksBrd[sq] == RANK_2 ) { // can do double move
        targ_sq = sq + 20; // look two ranks ahead
        if ( SQEMPTY(targ_sq, pos) ) {
            move = MOVE (sq, targ_sq, EMPTY, EMPTY, 0);
            AddQuietMove(pos, move, list);
            printf("Added move: %s\n", PrMove(move));
        }
    }
}