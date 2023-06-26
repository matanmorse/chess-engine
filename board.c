// board.c

#include "stdio.h"
#include "defs.h"

void UpdateListsMaterial( S_BOARD *pos ) {
    int piece, sq, index, color;

    for ( index = 0; index < BRD_SQ_NUM; index++ ) {
        sq = index;
        piece = pos -> pieces[sq];

        // if the piece exists, update the piece lists based on what kind of piece it is
        // using the arrays defined in data.c which are indexed by piece number
        if ( piece != OFFBOARD && piece != EMPTY ) {
            color = PieceCol[piece];

            if ( PieceBig[piece] == TRUE ) pos -> bigPce[color]++;
            if ( PieceMaj[piece] == TRUE ) pos -> majPce[color]++;
            if ( PieceMin[piece] == TRUE ) pos -> minPce[color]++;

            pos -> material[color] += PieceVal[piece];
            
            // the pList array is a 2d array where the first array contains the type of piece
            // and the second array is a list of the square where each piece of that type resides
            // we index this list using the number of pieces, then increment it to move on to the next item in the array.
            pos -> pList[piece][pos -> pceNum[piece]] = sq;
            pos -> pceNum[piece]++;

            if ( piece == wK ) pos -> KingSq[WHITE] = sq;
            if ( piece == bK ) pos -> KingSq[BLACK] = sq;
        }
    }
}

int ParseFen( char *fen, S_BOARD *pos) {
    ASSERT( fen != NULL );
    ASSERT( pos != NULL ); // make sure neither given pointers are null

    int rank = RANK_8; // because FEN notation starts from the back rank we start at rank 8
    int file = FILE_A;
    int piece = 0;
    int count = 0; // count number of empty squares if they exist
    int i = 0;
    int sq64 = 0;
    int sq120 = 0;

    ResetBoard( pos );
    
    while ( ( rank >= RANK_1 ) && *fen ) { // *fen is a DEREFERENCE, so it's talking about the character currently being pointed to.
        count = 1;
        switch (*fen) // change from FEN notation to our piece enumeration
        {   
            case 'p': piece = bP; break;
            case 'r': piece = bR; break;
            case 'n': piece = bN; break;
            case 'b': piece = bB; break;
            case 'k': piece = bK; break;
            case 'q': piece = bQ; break;
            case 'P': piece = wP; break;
            case 'R': piece = wR; break;
            case 'N': piece = wN; break;
            case 'B': piece = wB; break;
            case 'K': piece = wK; break;
            case 'Q': piece = wQ; break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
                piece = EMPTY;
                count = *fen - '0'; // subtract correct ascii value to get integer value
                break;

            case '/':
            case ' ': // if forward slash or space, move on to next rank
                rank--;
                file = FILE_A;
                fen++; // use pointer arithmatic to move to next char
                continue;
            
            default: 
                printf("FEN error \n");
                return -1;
        }

        // if count is greater than 1, i.e. the fen encountered a number, it will skip that many files, leaving them EMPTY
        // if FEN found a piece, count will remain 1 as defined at the top of the loop and the current square based on rank and file will be set to it
        // after each loop move on to next file, and after the pieces are set move on to the next letter
        for ( i = 0; i < count; i++ ) {
            sq64 = rank * 8 + file;
            sq120 = SQ120(sq64);
            if ( piece != EMPTY ) {
                pos -> pieces[sq120] = piece;
            }
            file++;
        }
        fen++;
    }
    // in FEN notation there will be 7 slashes before a space, representing each rank, so when the loop encounters the space RANK will already be equal to 1.
    // so when the loop encounters the space it will be rank 0 and break out of the loop

    ASSERT(*fen == 'w' || *fen == 'b'); // double check FEN is at the right part of the string
    pos -> side = (*fen == 'w') ? WHITE : BLACK;
    fen += 2;
    for ( i = 0; i < 4; i++ ) {
        if ( *fen == ' ') { // if we encounter a space we are at the end of the castling characters
            break;
        }
        switch ( *fen ) {   // use bitwise or to add the castling permissions based on the string
            case 'K': pos -> castlePerm |= WKCA;
            case 'Q': pos -> castlePerm |= WQCA;
            case 'k': pos -> castlePerm |= BKCA;
            case 'q': pos -> castlePerm |= BQCA;
            default: break;
        }

        fen++;
    }
    fen++; // move on to en passant square

    ASSERT( pos -> castlePerm >=0 && pos -> castlePerm <= 15 ); // verify value of castlePerm

    if ( *fen != '-' ) { // if not dash, there is an en passant square
        // we assume here the next two letters in the string are the file and rank of the en passant square, so we use some funny ascii math 
        // to get the integer values of the rank and file and use that to compute the number of the EP square on the 120 board index     
        file = fen[0] - 'a'; 
        rank = fen[1] - '1';

        ASSERT( file >= FILE_A && file <= FILE_H ); // double check en passant square is actually on the board
        ASSERT( rank >= RANK_1 && file <= RANK_8 );

        pos -> enPas = FR2SQ(file, rank);
    }

    pos -> posKey = GeneratePosKey(pos); // call the function and pray to god it works

    // update material lists
    UpdateListsMaterial(pos);
    return 0;
}


void ResetBoard( S_BOARD *pos ) {
    int index = 0;
    // first, set all squares on the board to offboard.
    for ( index = 0; index < BRD_SQ_NUM; index++ ) {
        pos -> pieces[index] = OFFBOARD;
    }
    // then, loop 0 - 64, convert to 120 index and set to empty
    // this way we set all valid squares on the board to empty.
    for ( index = 0; index < 64; index++ ){
        pos -> pieces[SQ120(index)] = EMPTY;
    } 
    // set all piece counts for white, black, and both to zero
    for ( index = 0; index < 3; index++ ){
        pos -> bigPce[index] = 0;
        pos -> majPce[index] = 0;
        pos -> minPce[index] = 0;
        pos -> pawns[index] = 0ULL; // pawns must be set to U64 type bc bitboards
    }

    for ( index = 0; index < 13; index++ ) {
        pos -> pceNum[index] = 0;
    }
    // reset everything else
    pos -> KingSq[WHITE] = pos -> KingSq[BLACK] = NO_SQ; // neither kings are on a square

    pos -> side = BOTH;
    pos -> enPas = NO_SQ;
    pos -> fiftyMove = 0;

    pos -> ply = 0;
    pos -> hisPly = 0;

    pos -> castlePerm = 0;

    pos -> posKey = 0ULL; // there is no position key
}

// simple print board out
int PrintBoard( S_BOARD *pos ) {
    int rank, sq, file, piece;

    printf("\nGame Board: \n\n");

    for ( rank = RANK_8; rank >= RANK_1; rank-- ) {
        printf("%d  ", rank + 1);
        for (file = FILE_A; file <= FILE_H; file++ ) {
            sq = FR2SQ( file, rank );
            piece = pos -> pieces[sq];
            printf("%3c", PceChar[piece]); // use pcechar index to print out correct piece
        }
        printf("\n");
    }
    printf("\n   ");

    for ( file = FILE_A; file <= FILE_H; file++ ) {
        printf("%3c", 'a' + file);
    }
    printf("\n");

    printf("Side: %c\n", SideChar[pos -> side]);
    printf("enPas: %d\n", pos -> enPas);
    printf("castle: %c%c%c%c \n",
    pos -> castlePerm & WKCA ? 'K' : '-',
    pos -> castlePerm & WQCA ? 'Q' : '-',
    pos -> castlePerm & BKCA ? 'k' : '-',
    pos -> castlePerm & BQCA ? 'q' : '-'
    );

    printf("PosKey: %llX \n", pos -> posKey);
}