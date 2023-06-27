# Chess Engine Notes
These notes are basically just for myself to keep up and understand some of the functions and algorithims used in the Chess engine, sort of like a doccumentation, but for myself because I'm sure I'll begin to forget things as things become more complicated.

## Pawn Bitboards
Bitboards are a 64-bit unsigned value, where each bit corresponds to if there is a pawn present on that square or not. For example, a starting position would look like:
```bin
0 0 0 0 0 0 0 0
1 1 1 1 1 1 1 1
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 
1 1 1 1 1 1 1 1
0 0 0 0 0 0 0 0 
```
Since there are pawns on the 2nd and 7th rank, all bits in that rank are 1s, and no other pawns exist so the rest of them are zero. These bitboards work on a 64-index, so the number 0 corresponds to the A1 square.

### SETBIT and CLRBIT, SetMask and ClearMask
SetMask and ClearMask are two arrays of 64 Unsigned 64 bit numbers (U64), essentially 64 bitboards. At each index, the SetMask contains a bitboard with a pawn at that index. For example, SetMask[0] is a bitboard with a pawn on A1. The ClearMask is just the opposite of SetMask, so ClearMask[0] contains a bitboard with a pawn everywhere but A1. These two arrays are initialized in the init.c file. 

The SETBIT and CLRBIT macros, defined in defs.h, just use an AND or OR bitwise operator respectively to combine the mask at a given square with a given bitboard. This has the effect of adding or removing a pawn from the bitboard. For example with setmask:
Bitboard:
```bin
0 0 0 0 0 0 0 0        0 0 0 0 0 0 0 0    0 0 0 0 0 0 0 0
0 0 1 0 0 0 0 0        0 0 0 0 0 0 0 0    0 0 1 0 0 0 0 0
0 0 0 0 0 0 0 0        0 0 0 0 0 0 0 0    0 0 0 0 0 0 0 0
0 0 0 0 1 0 0 0        0 0 0 0 0 0 0 0    0 0 0 0 1 0 0 0
0 0 0 0 0 0 0 0    |   0 0 0 0 0 0 0 0 =  0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0        0 0 0 0 0 0 0 0    0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0        0 0 0 0 0 0 0 0    0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0        0 0 0 0 0 0 0 0    0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0        1 0 0 0 0 0 0 0    1 0 0 0 0 0 0 0
```

Effectively having added the piece to the bitboard.
CLRBIT just works the same, but essentially in reverse.

## Position Key Hashing
There is a function in `hashkeys.c` called `GeneratePosKey()` which, given a position on the board, will generate a unique key for the position. This first works by, in `init.c`, populating a 2-dimensional array `PieceKeys[13][120]` where the first dimension is the type of piece, from White pawn to Black king, with a unique, random 64 bit unsigned number for each square on the board it could be in. In `GeneratePosKey()`, the function loops over the given position and hashes every piece on the board into the key using bitwise XOR to add the random 64 bit number to the key. This way a unique key can be generated for each position.
