// init.c

#include "defs.h"
int Sq120ToSq64[BRD_SQ_NUM]; // array which converts 120 square board numbers to 64 board numbers
int Sq64ToSq120[64]; // array which converts 64 sq board numbers to 64 sq board numbers
U64 ClearMask[64];
U64 SetMask[64];

// initialize masks used to set and clear bits
void InitBitMasks() {
    int index = 0;

    // start off by setting all values in masks to empty bitboards
    for (index = 0; index < 64; index++ ) {
        SetMask[index] = 0ULL;
        ClearMask[index] = 0ULL;
    }

    for ( index = 0; index < 64; index++ ) {
        SetMask[index] |= ( 1ULL << index );
        // the ClearMask index will be the complement of the SetMask index
        // so wherever SetMask has a 1 at a certain index, ClearMask will have a 0, vice-versa
        ClearMask[index] = ~SetMask[index];
    }

    
}

int InitSq120to64 (void) {
    int index = 0;
    int file = FILE_A;
    int rank = RANK_1;
    int sq = A1;
    int sq64 = 0; 

    // set all numbers in both arrays to empty spaces so they can be overwritten later, leaving only invalid spaces with these numbers
    for (index = 0; index < BRD_SQ_NUM; ++index) {
        Sq120ToSq64[index] = 65;
    }

    for (index = 0; index < 64; ++index) {
        Sq64ToSq120[index] = 120;
    }

    // iterate over all squares
    for(rank = RANK_1; rank <= RANK_8; ++rank) {
        for (file = FILE_A; file <= FILE_H; file++) {
            // basically just relate the 64-square values and the 120-square values using their associated file and rank
            // at the end this generates two arrays which can be used to convert between 120-square and 64-square integers

            // get 120 square num from file and rank
            sq = FR2SQ(file, rank);

            // set the 120 square (based on rank and file) to the position in the 64 -> 120 array 
            Sq64ToSq120[sq64] = sq;

            // use that 120 square value to index the 120 -> 64 array and set it to the current 1-64 number
            Sq120ToSq64[sq] = sq64;

            // move on to the next number in the 64 num array
            sq64++;
        }
    }
};

void AllInit() {
    InitBitMasks();
    InitSq120to64();
}
