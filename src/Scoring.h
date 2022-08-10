//////////////////////////////////////////////////////////////////
/*  Scoring.h                                                   */
/*  Version: 2 ; 4/30/20                                        */
/*                                                              */
/*  Header file for Scoring.c                                   */
//////////////////////////////////////////////////////////////////

#include "Piece.h"

#ifndef SCORING_H
#define SCORING_H

/*  Original score values:   */
/*
int wkP = 10;    // white piece kills an opponent's Pond
int wkR = 30;    // white piece kills an opponent's Rook
int wkB = 30;
int wkN = 50;
int wkQ = 90;
int wpsP = 5;      // white piece protects a fellow Pond
int wpsR = 10;     // white piece protects a fellow Rook
int wpsB = 10;
int wpsN = 10;
int wpsQ = 20;
int wpedP = 15;      // white piece protected by a fellow Pond
int wpedR = 10;      // white piece protected by a fellow Rook
int wpedB = 10;
int wpedN = 10;
int wpedQ = 10;

int bkP = 10;    // black piece kills an opponent's Pond
int bkR = 30;    // black piece kills an opponent's Rook
int bkB = 30;
int bkN = 50;
int bkQ = 90;
int bpsP = 5;      // black piece protects a fellow Pond
int bpsR = 10;     // black piece protects a fellow Rook
int bpsB = 10;
int bpsN = 10;
int bpsQ = 20;
int bpedP = 15;      // black piece protected by a fellow Pond
int bpedR = 10;      // black piece protected by a fellow Rook
int bpedB = 10;
int bpedN = 10;
int bpedQ = 10;
*/

// Function should take in the piece (regarding team and type), and new coordinates
// The function will return a score based on the given move
//int Scoring(PIECE * piece, int fRow, int fCol);  
int Scoring(PIECE * piece, PIECE * futurePiece, int oRow, int oCol, int fRow, int fCol);


#endif