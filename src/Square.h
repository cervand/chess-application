/*  Square.h header file for Board.c */
//include eventually header for piece structure

#include "Piece.h"

#ifndef SQUARE_H
#define SQUARE_H
typedef struct {
	int x;
	int y;
	PIECE * piece_ptr;
	} SQUARE;	

SQUARE * CreateSquare();

int DeleteSquare(SQUARE * square);

#endif 
