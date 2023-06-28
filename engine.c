#include "stdio.h"
#include "defs.h"
#include "stdlib.h"

#define FEN1 "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define FEN2 "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define FEN3 "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"
#define FEN4 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
#define FENATTACK "8/8/8/2R2rp1/2P5/8/8/8 w - - 0 2"

int main (void) {
    AllInit();

    // empty playBoard
    S_BOARD board[1];

    ParseFen(FEN4, board);    
    PrintBoard(board);
    ASSERT(CheckBoard(board));

    int move = 0; 
    int from = 6;
    int to = 12; 
    int captured = wR;
    int promoted = bR;

    move = ( from ) | ( to << 7  ) | ( captured << 14 ) | ( promoted << 20 ) ;

    printf("Decimal: %d Hex: %X\n", move, move);
    PrintBin(move);

    printf("From: %d To: %d Captured: %d Promoted: %d Is Pawn Start: %s",
    FROMSQ(move), TOSQ(move), CAPTURED(move), PROMOTED(move), (move & MFLAGPS) ? "yes" : "no");
    return 0;
}