/* Check.h*/

/* This is the header file for Check.h */

#include "Square.h"
#include "Piece.h"
#include "Board.h"
#include "Movement.h"

#ifndef CHECK_H
#define CHECK_H

int Check(char teamBias);

int isAttacked(char teamBias, int row, int col);

#endif