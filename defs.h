#ifndef DEFS_H
#define defs_h

#include "stdlib.h"
#include "stdio.h"

#define DEBUG

#ifndef DEBUG
#define ASSERT(n)
#else
// can use this to assert if a value is true, if it's false it will end the program and spit out diagnostics
#define ASSERT(n) \
if (!(n)) { \
    printf("%s - Failed ", #n); \
     printf("On %s ", __DATE__); \
    printf("At %s ", __TIME__); \
    printf("In File %s ", __FILE__); \
    printf("At line %d", __LINE__); \
    exit(1);};
#endif

typedef unsigned long long U64;

#define NAME "Engine 1.0"
#define BRD_SQ_NUM 120

#define MAXGAMEMOVES 2048
#define MAXPOSITIONMOVES 256

#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

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
A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQ, OFFBOARD
};

enum { FALSE, TRUE };

// castling permission for both sides
// convert to 4 bit number showing castling permission
enum { WKCA = 1, WQCA = 2, BKCA = 4, BQCA = 8 }; 


typedef struct {
    int move; // stores information for move
    int score;
} S_MOVE;

typedef struct {
    S_MOVE moves[MAXPOSITIONMOVES];
    int count; // count of moves on movelist
} S_MOVELIST;   
typedef struct {
    int move; // current move
    int castlePerm; // castling permission before move was played
    int enPas;
    int fiftyMove;
    U64 posKey; // unique position key
} S_UNDO;

typedef struct {
    int pieces[BRD_SQ_NUM]; // declare array of 120 integers
    U64 pawns[3]; // 64 bits representing 8x8 board, 0 if no pawn, 1 if pawn, 3 values for each color (white, black, both)

    int KingSq[2];  // store position of kings

    int side;    // whose side it is
    int enPas;     // en passant square, if there is one
    int fiftyMove; // counter for fifty-move draw

    int castlePerm; // four bit number representing castling permission in a situation (e.g. 1 0 0 1)
    int ply;
    int hisPly;

    U64 posKey; // unique key generated for each position

    int pceNum[13]; // array for how many of each piece
    int bigPce[2]; // store, by color, the number of non-pawn pieces
    int majPce[2]; // number of rooks/queens
    int minPce[2]; // number of bishops/knights
    int material[2]; // total material value of pieces

    S_UNDO history[MAXGAMEMOVES]; // array storing board state histories back to the start of the game indexed by move number

    int pList[13][10]; // piece list
} S_BOARD;
#endif

/* GAME MOVE */

/* 
32 Bits
0000 0000 0000 0000 0000 0111 1111 --> From Square 0x3F
0000 0000 0000 0011 1111 1000 0000 --> To >> 7, 0x3F
0000 0000 0011 1100 0000 0000 0000 --> Captured Piece >> 14, 0xF
0000 0000 0100 0000 0000 0000 0000 --> En Passant Capture? 0x40000 
0000 0000 1000 0000 0000 0000 0000 --> Pawn Start? 0x80000
0000 1111 0000 0000 0000 0000 0000 --> Promoted Piece >> 20, 0xF    
0001 0000 0000 0000 0000 0000 0000 --> Castling >> 0x1000000
*/

// appropriate shifting and bitwise operations to get from sq, to sq, captured+promoted piece from move int
#define FROMSQ(m) ((m) & 0x7F)    
#define TOSQ(m) ((m >> 7) & 0x7F)    
#define CAPTURED(m) ((m >> 14) & 0xF)    
#define PROMOTED(m) ((m >> 20) & 0xF)

// bit locations of the En Passant, Pawn Start, and castling flags
#define MFLAGEP 0x40000
#define MFLAGPS 0x80000
#define MFLAGCA 0x1000000

// bit locations of captured/promotion booleans
#define MFLAGCAP 0x7C000
#define MFLAGPROM 0xF00000



/* MACROS */
#define FR2SQ(f, r) ( (21 + (f) ) + ( r * 10 ) ) // macro to convert from rank and file to 120 board sq number
#define SQ64(sq120) (Sq120ToSq64[(sq120)]) // just shorten name for 120 to 64 matrix
#define SQ120(sq64) (Sq64ToSq120[(sq64)]) // just shorten name for 64 to 120 matrix
#define POP(b) popBit(b) // shorten name for pop function
#define CNT(b) countBits(b) // shorten name for count function
#define CLRBIT(bb, sq) ((bb) &= ClearMask[(sq)])
#define SETBIT(bb, sq) ((bb) |= SetMask[(sq)])

#define IsBQ(p) (PieceBishopQueen[(p)]) // macro shortening PieceBishopQueen[] array
#define IsRQ(p) (PieceRookQueen[(p)]) // macro shortening PieceRookQueen[] array
#define IsKi(p) (PieceKing[(p)]) // macro shortening PieceKing[] array
#define IsKn(p) (PieceKnight[(p)]) // macro shortening PieceKnight[] array

/* GLOBALS */
extern int Sq120ToSq64[BRD_SQ_NUM]; // array which converts 120 square board numbers to 64 board numbers
extern int Sq64ToSq120[64]; // array which converts 64 sq board numbers to 64 sq board numbers
extern U64 SetMask[64];
extern U64 ClearMask[64];
extern U64 PieceKeys[13][120];
extern U64 SideKey;
extern U64 CastleKeys[16];

extern char PceChar[];
extern char SideChar[];
extern char RankChar[];
extern char FileChar[];

extern int PieceBig[13];
extern int PieceMaj[13];
extern int PieceMin[13];
extern int PieceVal[13];
extern int PieceCol[13];

extern int FilesBrd[BRD_SQ_NUM];
extern int RanksBrd[BRD_SQ_NUM];

extern int PieceKnight[13]; // is piece a knight
extern int PieceKing[13]; // is piece a king
extern int PieceRookQueen[13]; // is piece rook or queen
extern int PieceBishopQueen[13]; // is piece bishop or queen

/* FUNCTIONS */ 
// init.c
extern void AllInit(); // initializes sq-conversion arrays

// bitboards.c
extern void PrintBitBoard(U64 bitBoard);
extern int popBit(U64 *bitBoard);
extern int countBits(U64 bitBoard);

// hashkeys.c
extern U64 GeneratePosKey(const S_BOARD *pos);

// board.c
extern void ResetBoard(S_BOARD *pos);   
extern int PrintBoard( S_BOARD *pos );
extern int ParseFen(char *fen, S_BOARD *pos);
extern void UpdateListsMaterial( S_BOARD *pos );
extern int CheckBoard( const S_BOARD *pos );

// check.c
extern void CheckBitBoard(S_BOARD *pos);
extern void ShowSqAttackedBySide(const int side, const S_BOARD *pos);
extern void PrintBin(int move);

// attack.c
extern int SqAttacked (const int sq, const int side, const S_BOARD *pos);

// io.c
extern char *PrSq(const int sq);
extern char *PrMove(const int move);