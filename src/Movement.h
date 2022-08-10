/*
4/28 Andres 
    - Merged changePieceTo and movePieceTo, would be confusing and useless to have them seperate?	     	                
*/

//Movement.h  -   Header file for Movement.h   
#include "Square.h"
#include "Piece.h"

#ifndef MOVEMENT_H
#define MOVEMENT_H

int firstteammove, secondteammove;

int movePieceTo(PIECE * p, int fromRow, int fromCol, int toRow, int toCol);

int isValidPieceMove(PIECE * p, int toRow, int toCol);

int isValidPawnMove(PIECE * p, int desRow, int desCol);

int isValidKingMove(PIECE * p, int desRow, int desCol);

int isValidBishopMove(PIECE * p, int toRow, int toCol);

int isValidEnPassant(PIECE * p, int tRow, int tCol, int revFactor);

int getKingRow(char teamBias);

int getKingCol(char teamCol);

int isCheckmate(char teamBias);

int isStillCheck(PIECE * p, int tRow, int tCol, char teamBias);

int Limit(int input);

int isValidRookMove(PIECE * p, int toRow, int toCol);

int isValidKnightMove(PIECE * p, int toRow, int toCol);

int isValidCastling(PIECE * p, int toRow, int toCol);

int isValidQueenMove(PIECE * p, int toRow, int toCol);


#endif
