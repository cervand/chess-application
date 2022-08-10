/* Checkmate.h*/

/* This is the header file for Checkmate.c */


#ifndef CHECKMATE_H
#define CHECKMATE_H

int totalCheckmate(char teamBias);

int moveoutCheck(char teamBias);

int blockCheck(char teamBias);

int canBlock(char teamBias, int row, int col);

void getEnemies(char teamBias);

int checkmate; 

int checkMovesR[300];

int checkMovesC[300]; 

PIECE * enemyPieces[16];

#endif 