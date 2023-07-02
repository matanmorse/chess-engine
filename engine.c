#include "stdio.h"
#include "defs.h"
#include "stdlib.h"

#define FEN1 "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define FEN2 "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define FEN3 "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"
#define FEN4 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
#define FENTEST "7k/1p6/R1p5/1N6/8/8/6p1/K4N1N b KQkq - 0 1"
#define PAWNMOVES "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w - e6 0 2"
#define FENATTACK "8/8/8/2R2rp1/2P5/8/8/8 w - - 0 2"

int main (void) {
    AllInit();

    // empty playBoard
    S_BOARD board[1];
    S_MOVELIST list[1];
  

    ParseFen(FENTEST, board);
    PrintBoard(board);

    GenerateAllMoves(board, list);
    PrintMoveList(list);
    return 0;
}