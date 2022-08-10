/*
4/28 Andres
    - added updatePiece() and members to initialize in CreatePiece
*/


/* Piece structure module */
#include "Piece.h"
#include <stdio.h>
#include <stdlib.h>
#include "Chess.h"

PIECE * CreatePiece() {                     //creating a piece
    PIECE *temp = malloc(sizeof(PIECE));
    temp->type = '\0';
    temp->team = '\0';
    temp->row = 0;
    temp->col = 0;
    temp->moves = 0;
    temp->prevCol = 0;
    temp->prevRow = 0;
    return temp;
}

PIECE * DuplicatePiece(PIECE * piece) {
    PIECE *copy = CreatePiece();
    copy->type = piece->type;
    copy->team = piece->team;
    copy->row = piece->row;
    copy->col = piece->col;
    copy->moves = piece->moves;
    copy->prevCol = piece->prevCol;
    copy->prevRow = piece->prevRow;
    return copy;

}


int getPieceRow(PIECE * p){
    return p -> row;
}

int getPieceCol(PIECE * p){
    return p -> col;
}

int DeletePiece(PIECE * piece) {            //deleting a piece if it exists
    if (piece != NULL) {
        if (piece->team == firstteam) {         //storing the first pieces information if it got captured(for Algebraic notation)
            capturedpiecetype = piece->type;
            capturedpiecerow = piece->row;
            capturedpiececol = piece->col;
            capturedpiecestartcol = piece->prevCol;
            firstteampiececaptured = 1;            
        }
        piece->team = '\0';
        piece->type = '\0';
        piece->col = piece->row = piece->prevCol = piece->prevRow = piece->moves = 0;
        free(piece);
        piece = NULL;
        return 0;
    } else {
        return 1;
    }
}

//updates all the stuff in the piece except initially defined things like
//team and symbol
void updatePiece(PIECE * p, int fRow, int fCol, int tRow, int tCol){
    p->row = tRow;
    p->col = tCol;
    p->prevRow = fRow;
    p->prevCol = fCol;
    p->moves = p->moves+1;
}

