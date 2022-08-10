/* Header file for Chess.c */

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#ifndef CHESS_H
#define CHESS_H

char playerteam;
char otherteam;
char otherplayer;                   //other player type
char player;                        //player type
char firstteam;                     //first team that gets printed in algebraic notation
int firstteaminducedcheck;          //first team put other team in check
int secondteaminducedcheck;         //second team put first team in check
int firstteampawnpromotion;         //first team promoted their pawn
int secondteampawnpromotion;        //second team promoted their pawn
int firstteampiececaptured;         //first team's piece got captured 
char capturedpiecetype;             //stores the first team's captured piece type
int capturedpiecerow;               //stores the first team's captured piece row
int capturedpiececol;               //stores the first team's captured piece col
int capturedpiecestartcol;          //stores the first team's captured piece previous col (used specifically for En Passant Alg notation)

//edit: changed from void to int type 
void PlayerTurn(char team, int turnnumber);         //used to execute a human player's turn

void PrintMove(char team, PIECE* piece, int turnnumber, char player);

void PrintAlgebraicMove(char firstteam);
#endif

/* EOF */