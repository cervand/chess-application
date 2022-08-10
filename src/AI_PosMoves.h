//////////////////////////////////////////////////////////////////
/*  AI_PosMoves.h                                               */
/*  Version: 2 ; 4/30/20                                        */
/*                                                              */
/*  Header file for AI_PosMoves.c                               */
//////////////////////////////////////////////////////////////////

//#include "Square.h"
//#include "Piece.h"
//#include "Board.h"
//#include "Scoring.h"


#ifndef AI_POSMOVES_H
#define AI_POSMOVES_H

// One chess turn will NOT have more than 300 moves possible
int dataOldR[120];
int dataOldC[120];
int dataNewR[120];
int dataNewC[120];
int dataScore[120];

int AI_PosMoves(char team);


#endif