#ifndef DEFS_H
#define defs_h

typedef unsigned long long U64;

#define NAME "Engine 1.0"
#define BRD_SQ_NUM 120

#define MAXGAMEMOVES 2048

// enumerate pieces including no piece
enum { EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK };

// enumerate rank and files
enum { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE };
enum { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_NONE };

enum { WHITE, BLACK, BOTH };

// relate board coordinates with integer values
enum {
A1 = 21, B1, C1, D1, E1, F1, G1, H1, 
A2 = 31, B2, C2, D2, E2, F2, G2, H2, 
A3 = 41, B3, C3, D3, E3, F3, G3, H3, 
A4 = 51, B4, C4, D4, E4, F4, G4, H4, 
A5 = 61, B5, C5, D5, E5, F5, G5, H5, 
A6 = 71, B6, C6, D6, E6, F6, G6, H6, 
A7 = 81, B7, C7, D7, E7, F7, G7, H7, 
A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQ
};

enum { FALSE, TRUE };

// castling permission for both sides
// convert to 4 bit number showing castling permission
enum { WKCA = 1, WQKA = 2, BKCA = 4, BCKA = 8 }; 

typedef struct {
    int move; // current move
    int castlePerm; // castling permission before move was played
    int enPas;
    int fiftyMove;
    U64 posKey; // unique position key
} S_UNDO;

typedef struct {
    int pieces[BRD_SQ_NUM]; // declare array of 120 integers
    // 64 bits representing 8x8 board, 0 if no pawn, 1 if pawn, 3 values for each color (white, black, both)
    U64 pawns[3];

    int KingSq[2];  // store position of kings

    int side;    // whose side it is
    int enPas;     // en passant square, if there is one
    int fiftyMove; // counter for fifty-move draw

    int castlePerm; // four bit number representing castling permission in a situation (e.g. 1 0 0 1)
    int ply;
    int hisPly;

    U64 posKey; // unique key generated for each position

    int pceNum[13]; // array for number of each pieces
    int big[3]; // store, by color, the number of non-pawn pieces
    int majPce[3]; // number of rooks/queens
    int minPce[3]; // number of bishops/knights

    S_UNDO history[MAXGAMEMOVES]; // array storing board state histories back to the start of the game indexed by move number
} S_BOARD;
#endif