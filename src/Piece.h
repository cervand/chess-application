/*
4/28 Andres 
    - added new stuff to piece struct, new function updatePiece
*/

/* Piece header file for Piece.c */

#ifndef PIECE_H
#define PIECE_H

//Loosely added new vars into the struct so movement is easier to write 
//Piece Types: P, R, B, N(Knight), K(King), Q
typedef struct {
    char team;
    char type;
    int row;
    int col;
    int moves;
    int prevRow;
    int prevCol; 
} PIECE;

PIECE * CreatePiece();

PIECE * DuplicatePiece(PIECE * piece);

int DeletePiece(PIECE * piece);

int getPieceRow(PIECE * p);

int getPieceCol(PIECE * p);

void updatePiece(PIECE * p, int fRow, int fCol, int tRow, int tCol);

#endif
