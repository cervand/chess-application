/* Check.c */
/* This is the implementation for checking for check */
/*
4/49 Taylor
    - set up outline of check
5/5 Taylor
    - change parameters 
*/

#include "Square.h"
#include "Board.h"
#include "Piece.h"
#include "Check.h"
#include "Movement.h"
#include <stdio.h>
#include <stdlib.h>
//test


//change parameters to 
int Check(char teamBias)
{
    //int i=0; 
    int check = 0; 
    int KingRow, KingCol;
    PIECE * King = NULL; 
    
    //find king position of team 
    for(int row = 0; row < 8; row++){
        for(int col = 0; col < 8; col++){
            if(getPieceAt(row,col) != NULL){
                if (getPieceAt(row,col)->type == 'K' && getPieceAt(row,col)->team == teamBias){
                    King = getPieceAt(row,col);  //save king of same team to array 
                }
            }
        }
    }

    KingRow = King->row;
    KingCol = King->col;
    if (isAttacked(teamBias, KingRow, KingCol) == 1) //if king current position in attacked position
    {
        check = 1; //check flag up 
    }
    else
    {
        check = 0; //check flag down
    }
    
    return check; 
    
}

int isAttacked(char teamBias, int row, int col){
    //Here I am supposedly putting all enemy pieces into a one dimensional array
    PIECE * enemyPieces[16];
    int maxIndex = 0;
    int counter = 0;
    //If out of bounds, return 1 (using for check) (used for movement isCheckmate)
    if((row<0) || (col<0) || (row>7) || (col>7)){
        return 1;
    }
    for(int r = 0; r < 8; r++){
        for(int c = 0; c < 8; c++){
            if(getPieceAt(r,c) != NULL){
                if (getPieceAt(r,c)->team != teamBias){
                    enemyPieces[maxIndex] = getPieceAt(r,c);
                    maxIndex++;
                }   
            }
        }
    }    
    if (maxIndex < 16) {
        for (int a = maxIndex; a < 16; a++) {                   //setting rest of pieces to NULL
            enemyPieces[a] = NULL;
        }
    }
    //Now I am checking if the enemy pieces are in the trajectory of row/col or could move to row and col (the function parameters)
    for(int i = 0; i < maxIndex; i++){
        if (enemyPieces[i] != NULL) {
            if(isValidPieceMove(enemyPieces[i], row, col) ){ //seg
//                printf("\n %c%c at (%d, %d) satisfied condition", enemyPieces[i]->team, enemyPieces[i]->type, enemyPieces[i]->row, enemyPieces[i]->col);
                counter = 1;
            }
        }
    }
    if (counter == 1) {
        return 1;
    }
    else{
    return 0;
    } 

}//end isAttacked



