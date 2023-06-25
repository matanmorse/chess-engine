#include "stdio.h"
#include "defs.h"

int main (void) {
    AllInit();
    // empty bit board
    U64 playBitBoard = 0ULL;

    printf("Start \n");
    PrintBitBoard(playBitBoard);


    return 0;
}