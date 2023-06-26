#include "stdio.h"
#include "defs.h"
#include "stdlib.h"

#define FEN1 "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define FEN2 "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define FEN3 "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"

int main (void) {
    AllInit();
    // empty bit board
    U64 playBitBoard = 0ULL;
    // empty playBoard
    S_BOARD playBoard;

    // ResetBoard(&playBoard);
    // ParseFen(START_FEN, &playBoard);
    // printf("Starting Position: \n");
    // PrintBoard(&playBoard);

    // ResetBoard(&playBoard);
    // ParseFen(FEN1, &playBoard);
    // printf("Fen 1: \n");
    // PrintBoard(&playBoard);

    // ResetBoard(&playBoard);
    // ParseFen(FEN2, &playBoard);
    // printf("Starting Position: \n");
    // PrintBoard(&playBoard);

    //  ResetBoard(&playBoard);
    // ParseFen(FEN3, &playBoard);
    // printf("Starting Position: \n");
    // PrintBoard(&playBoard);

    int rank, file, sq;
   
    ASSERT(1 == 0);
    return 0;
}