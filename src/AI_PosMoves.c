//////////////////////////////////////////////////////////////////////
/*  AI_PosMoves.c                                                   */
/*  Version: 2 ; 4/30/20                                            */
/*                                                                  */
/*  Function calculates all of the possible moves for a particular  */
/*  team based on the inputed board.*/
//////////////////////////////////////////////////////////////////

#include "AI_PosMoves.h"
#include "Scoring.h"
#include "Piece.h"
#include "Square.h"
#include "Board.h"
#include "Movement.h"
#include <stdio.h>
#include <time.h>

int AI_PosMoves(char aTeam){   // takes 'W' or 'B' and board

    int row, col, i, j;
    int counter = 0;    
    int Rblock1 = 0;   // determine if rook's path is blocked (Save Compile TIME!)
    int Rblock2 = 0;
    int Rblock3 = 0;
    int Rblock4 = 0;
    int Bblock1 = 0;    // bishops
    int Bblock2 = 0;
    int Bblock3 = 0;
    int Bblock4 = 0;
    int Qblock1 = 0;    // queen
    int Qblock2 = 0;
    int Qblock3 = 0;
    int Qblock4 = 0;
    int Qblock5 = 0;
    int Qblock6 = 0;
    int Qblock7 = 0;
    int Qblock8 = 0;
    int dir;    // used for pond to determine moving up or down the board
    int tmp;
    int tempScore = 0;
    
    // assign current team and opponent team
    char bTeam;
    if (aTeam == 'W'){
        bTeam = 'B';
        dir = -1;   // used to move up the board
    }
    else if (aTeam == 'B'){
        bTeam = 'W';
        dir = 1;    // used to move down the board
    }

    for (row = 0; row < 8; row++) { // assume AI is black team (top to bottom of board)
        for (col = 0; col < 8; col++) {
            PIECE * piece = NULL;
            piece = getPieceAt(row, col);
            if (piece != NULL){
                if (piece -> team == aTeam) { // filter through remaining AI pieces            
                    if (piece -> type == 'P') {  // check ponds
                        // en passant to the right
                        if ((row + dir < 8) && (row + dir >= 0)){
                            if(col+1 < 8){
                                if(isValidEnPassant(piece, row, col+1, dir)){
                                    PIECE * tmpPiece = NULL;
                                    tmpPiece = getPieceAt(row+dir, col);
                                    if (tmpPiece == NULL){
                                        dataOldR[counter] = row;
                                        dataOldC[counter] = col;
                                        dataNewR[counter] = row + dir;
                                        dataNewC[counter] = col + 1;
                                        dataScore[counter] = Scoring(piece, tmpPiece, row, col, row + dir, col + 1);
                                        counter++;  // update counter
                                    }
                                }
                            }   
                        }
                        // en passant to the left
                        if ((row + dir < 8) && (row + dir >= 0)){
                            if(col-1 >= 0){
                                if(isValidEnPassant(piece, row, col-1, dir)){
                                    PIECE * tmpPiece = NULL;
                                    tmpPiece = getPieceAt(row+dir, col);
                                    if (tmpPiece == NULL){
                                        dataOldR[counter] = row;
                                        dataOldC[counter] = col;
                                        dataNewR[counter] = row + dir;
                                        dataNewC[counter] = col - 1;
                                        dataScore[counter] = Scoring(piece, tmpPiece, row, col, row + dir, col - 1);
                                        counter++;
                                    }
                                }
                            }   
                        }
                        // front 1 space
                        if ((row + dir < 8) && (row + dir >= 0)){
                            PIECE * tmpPiece = NULL;
                            tmpPiece = getPieceAt(row + dir, col);
                            if (tmpPiece == NULL){
                                dataOldR[counter] = row;
                                dataOldC[counter] = col;
                                dataNewR[counter] = row + dir;
                                dataNewC[counter] = col;
                                dataScore[counter] = Scoring(piece, tmpPiece, row, col, row + dir, col);
                                counter++;  // update counter
                            }
                        }
                        // front 2 spaces
                        if ((row == 1) && (aTeam == 'B')){
                            PIECE * tmpPiece1 = NULL;
                            tmpPiece1 = getPieceAt(row + 1, col);
                            if (tmpPiece1 == NULL){
                                PIECE * tmpPiece2 = NULL;
                                tmpPiece2 = getPieceAt(row + 2, col);
                                if (tmpPiece2 == NULL){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row + 2;
                                    dataNewC[counter] = col;
                                    dataScore[counter] = Scoring(piece, tmpPiece2, row, col, row + 2, col);
                                    counter++;
                                }
                            }
                        }
                        if ((row == 6) && (aTeam == 'B')){
                            PIECE * tmpPiece1 = NULL;
                            tmpPiece1 = getPieceAt(row - 1, col);
                            if (tmpPiece1 == NULL){
                                PIECE * tmpPiece2 = NULL;
                                tmpPiece2 = getPieceAt(row - 2, col);
                                if (tmpPiece2 == NULL){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row - 2;
                                    dataNewC[counter] = col;
                                    dataScore[counter] = Scoring(piece, tmpPiece2, row, col, row - 2, col);
                                    counter++;
                                }
                            }
                        }

                        if ((row == 1) && (aTeam == 'W')){
                            PIECE * tmpPiece1 = NULL;
                            tmpPiece1 = getPieceAt(row + 1, col);
                            if (tmpPiece1 == NULL){
                                PIECE * tmpPiece2 = NULL;
                                tmpPiece2 = getPieceAt(row + 2, col);
                                if (tmpPiece2 == NULL){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row + 2;
                                    dataNewC[counter] = col;
                                    dataScore[counter] = Scoring(piece, tmpPiece2, row, col, row + 2, col);
                                    counter++;
                                }
                            }
                        }
                        if ((row == 6) && (aTeam == 'W')){
                            PIECE * tmpPiece1 = NULL;
                            tmpPiece1 = getPieceAt(row - 1, col);
                            if (tmpPiece1 == NULL){
                                PIECE * tmpPiece2 = NULL;
                                tmpPiece2 = getPieceAt(row - 2, col);
                                if (tmpPiece2 == NULL){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row - 2;
                                    dataNewC[counter] = col;
                                    dataScore[counter] = Scoring(piece, tmpPiece2, row, col, row - 2, col);
                                    counter++;
                                }
                            }
                        }
                        //  diagonal
                        if ((row + dir < 8) && (row + dir >= 0)){
                            if ((col - dir < 8) && (col - dir >= 0)){
                                PIECE * tmpPiece = NULL;
                                tmpPiece = getPieceAt(row + dir, col - dir);
                                if (tmpPiece != NULL){
                                    if (tmpPiece -> team == bTeam){
                                        dataOldR[counter] = row;
                                        dataOldC[counter] = col;
                                        dataNewR[counter] = row + dir;
                                        dataNewC[counter] = col - dir;
                                        dataScore[counter] = Scoring(piece, tmpPiece, row, col, row + dir, col - dir);
                                        counter++;
                                    }
                                }
                            }
                            if ((col + dir < 8) && (col + dir >= 0)){
                                PIECE * tmpPiece = NULL;
                                tmpPiece = getPieceAt(row + dir, col + dir);
                                if (tmpPiece != NULL){
                                    if (tmpPiece -> team == bTeam){
                                        dataOldR[counter] = row;
                                        dataOldC[counter] = col;
                                        dataNewR[counter] = row + dir;
                                        dataNewC[counter] = col + dir;
                                        dataScore[counter] = Scoring(piece, tmpPiece, row, col, row + dir, col + dir);
                                        counter++;
                                    }
                                }
                            }
                        }//end diagonal
                    }//end pawn 

                    if (piece -> type == 'R') {  // check rooks
                        for (i = 1; i < 8; i++){
                            // check vertical up
                            if ((Rblock1 == 0) && (row - i >= 0)){
                                PIECE * tmpPiece = NULL;
                                tmpPiece = getPieceAt(row - i, col);
                                if (tmpPiece == NULL){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row - i;
                                    dataNewC[counter] = col;
                                    dataScore[counter] = Scoring(piece, tmpPiece, row, col, row - i, col);
                                    counter++;
                                }
                                if (tmpPiece != NULL){   // same as: (getSquareAt(row - i, col) != NULL)
                                    if (tmpPiece -> team == bTeam){
                                        dataOldR[counter] = row;
                                        dataOldC[counter] = col;
                                        dataNewR[counter] = row - i;
                                        dataNewC[counter] = col;
                                        dataScore[counter] = Scoring(piece, tmpPiece, row, col, row - i, col);
                                        counter++;
                                    }
                                    Rblock1 = 1;    // update variable so AI won't keep checking past blocked path (SAVE TIME!)
                                } 
                            }
                            // check vertical down
                            if ((Rblock2 == 0) && (row + i < 8)){
                                PIECE * tmpPiece = NULL;
                                tmpPiece = getPieceAt(row + i, col);
                                if (tmpPiece == NULL){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row + i;
                                    dataNewC[counter] = col;
                                    dataScore[counter] = Scoring(piece, tmpPiece, row, col, row + i, col);
                                    counter++;
                                }
                                if (tmpPiece != NULL){
                                    if (tmpPiece -> team == bTeam){
                                        dataOldR[counter] = row;
                                        dataOldC[counter] = col;
                                        dataNewR[counter] = row + i;
                                        dataNewC[counter] = col;
                                        dataScore[counter] = Scoring(piece, tmpPiece, row, col, row + i, col);
                                        counter++;
                                    }
                                    Rblock2 = 1;
                                } 
                            }
                            // check horizontal left
                            if ((Rblock3 == 0) && (col - i >= 0)){
                                PIECE * tmpPiece = NULL;
                                tmpPiece = getPieceAt(row, col - i);
                                if (tmpPiece == NULL){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row;
                                    dataNewC[counter] = col - i;
                                    dataScore[counter] = Scoring(piece, tmpPiece, row, col, row, col - i);
                                    counter++;
                                }
                                if (tmpPiece != NULL){
                                    if (tmpPiece -> team == bTeam){
                                        dataOldR[counter] = row;
                                        dataOldC[counter] = col;
                                        dataNewR[counter] = row;
                                        dataNewC[counter] = col - i;
                                        dataScore[counter] = Scoring(piece, tmpPiece, row, col, row , col - i);
                                        counter++;
                                    }
                                    Rblock3 = 1;
                                }
                            }
                            // check horizontal right
                            if ((Rblock4 == 0) && (col + i < 8)){
                                PIECE * tmpPiece = NULL;
                                tmpPiece = getPieceAt(row, col + i);
                                if (tmpPiece == NULL){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row;
                                    dataNewC[counter] = col + i;
                                    dataScore[counter] = Scoring(piece, tmpPiece, row, col, row, col + i);
                                    counter++;
                                }
                                if (tmpPiece != NULL){
                                    if (tmpPiece -> team == bTeam){
                                        dataOldR[counter] = row;
                                        dataOldC[counter] = col;
                                        dataNewR[counter] = row;
                                        dataNewC[counter] = col + i;
                                        dataScore[counter] = Scoring(piece, tmpPiece, row, col, row, col + i);
                                        counter++;
                                    }
                                    Rblock4 = 1;
                                } 
                            }
                        }   // exit for loop
                    }   // exit rook
                    if (piece -> type == 'N') {  // check knights
                        // L1
                        if ((row - 2 >= 0) && (col + 1 < 8)){
                            PIECE * tmpPiece = NULL;
                            tmpPiece = getPieceAt(row - 2, col + 1);
                            if (tmpPiece == NULL){
                                dataOldR[counter] = row;
                                dataOldC[counter] = col;
                                dataNewR[counter] = row - 2;
                                dataNewC[counter] = col + 1;
                                dataScore[counter] = Scoring(piece, tmpPiece, row, col, row - 2, col + 1);
                                counter++;
                            }
                            if (tmpPiece != NULL){
                                if (tmpPiece -> team == bTeam){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row - 2;
                                    dataNewC[counter] = col + 1;
                                    dataScore[counter] = Scoring(piece, tmpPiece, row, col, row - 2, col + 1);
                                    counter++;
                                }
                            }
                        }
                        // L2
                        if ((row - 1 >= 0) && (col + 2 < 8)){
                            PIECE * tmpPiece = NULL;
                            tmpPiece = getPieceAt(row - 1, col + 2);
                            if (tmpPiece == NULL){
                                dataOldR[counter] = row;
                                dataOldC[counter] = col;
                                dataNewR[counter] = row - 1;
                                dataNewC[counter] = col + 2;
                                dataScore[counter] = Scoring(piece, tmpPiece, row, col, row - 1, col + 2);
                                counter++;
                            }
                            if (tmpPiece != NULL){
                                if (tmpPiece -> team == bTeam){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row - 1;
                                    dataNewC[counter] = col + 2;
                                    dataScore[counter] = Scoring(piece, tmpPiece, row, col, row - 1, col + 2);
                                    counter++;
                                }
                            }
                        }
                        // L3
                        if ((row + 1 < 8) && (col + 2 < 8)){
                            PIECE * tmpPiece = NULL;
                            tmpPiece = getPieceAt(row + 1, col + 2);
                            if (tmpPiece == NULL){
                                dataOldR[counter] = row;
                                dataOldC[counter] = col;
                                dataNewR[counter] = row + 1;
                                dataNewC[counter] = col + 2;
                                dataScore[counter] = Scoring(piece, tmpPiece, row, col, row + 1, col + 2);
                                counter++;
                            }
                            if (tmpPiece != NULL){
                                if (tmpPiece -> team == bTeam){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row + 1;
                                    dataNewC[counter] = col + 2;
                                    dataScore[counter] = Scoring(piece, tmpPiece, row, col, row + 1, col + 2);
                                    counter++;
                                }
                            }
                        }
                        // L4
                        if ((row + 2 < 8) && (col + 1 < 8)){
                            PIECE * tmpPiece = NULL;
                            tmpPiece = getPieceAt(row + 2, col + 1);
                            if (tmpPiece == NULL){
                                dataOldR[counter] = row;
                                dataOldC[counter] = col;
                                dataNewR[counter] = row + 2;
                                dataNewC[counter] = col + 1;
                                dataScore[counter] = Scoring(piece, tmpPiece, row, col, row + 2, col + 1);
                                counter++;
                            }
                            if (tmpPiece != NULL){
                                if (tmpPiece -> team == bTeam){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row + 2;
                                    dataNewC[counter] = col + 1;
                                    dataScore[counter] = Scoring(piece, tmpPiece, row, col, row + 2, col + 1);
                                    counter++;
                                }
                            }
                        }
                        // L5
                        if ((row + 2 < 8) && (col - 1 >= 0)){
                            PIECE * tmpPiece = NULL;
                            tmpPiece = getPieceAt(row + 2, col - 1);
                            if (tmpPiece == NULL){
                                dataOldR[counter] = row;
                                dataOldC[counter] = col;
                                dataNewR[counter] = row + 2;
                                dataNewC[counter] = col - 1;
                                dataScore[counter] = Scoring(piece, tmpPiece, row, col, row + 2, col - 1);
                                counter++;
                            }
                            if (tmpPiece != NULL){
                                if (tmpPiece -> team == bTeam){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row + 2;
                                    dataNewC[counter] = col - 1;
                                    dataScore[counter] = Scoring(piece, tmpPiece, row, col, row + 2, col - 1);
                                    counter++;
                                }
                            }
                        }
                        // L6
                        if ((row + 1 < 8) && (col - 2 >= 0)){
                            PIECE * tmpPiece = NULL;
                            tmpPiece = getPieceAt(row + 1, col - 2);
                            if (tmpPiece == NULL){
                                dataOldR[counter] = row;
                                dataOldC[counter] = col;
                                dataNewR[counter] = row + 1;
                                dataNewC[counter] = col - 2;
                                dataScore[counter] = Scoring(piece, tmpPiece, row, col, row + 1, col - 2);
                                counter++;
                            }
                            if (tmpPiece != NULL){
                                if (tmpPiece -> team == bTeam){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row + 1;
                                    dataNewC[counter] = col - 2;
                                    dataScore[counter] = Scoring(piece, tmpPiece, row, col, row + 1, col - 2);
                                    counter++;
                                }
                            }
                        }
                        // L7
                        if ((row - 1 >= 0) && (col - 2 >= 0)){
                            PIECE * tmpPiece = NULL;
                            tmpPiece = getPieceAt(row - 1, col - 2);
                            if (tmpPiece == NULL){
                                dataOldR[counter] = row;
                                dataOldC[counter] = col;
                                dataNewR[counter] = row - 1;
                                dataNewC[counter] = col - 2;
                                dataScore[counter] = Scoring(piece, tmpPiece, row, col, row - 1, col - 2);
                                counter++;
                            }
                            if (tmpPiece != NULL){
                                if (tmpPiece -> team == bTeam){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row - 1;
                                    dataNewC[counter] = col - 2;
                                    dataScore[counter] = Scoring(piece, tmpPiece, row, col, row - 1, col - 2);
                                    counter++;
                                }
                            }
                        }
                        // L8
                        if ((row - 2 >= 0) && (col - 1 >= 0)){
                            PIECE * tmpPiece = NULL;
                            tmpPiece = getPieceAt(row - 2, col - 1);
                            if (tmpPiece == NULL){
                                dataOldR[counter] = row;
                                dataOldC[counter] = col;
                                dataNewR[counter] = row - 2;
                                dataNewC[counter] = col - 1;
                                dataScore[counter] = Scoring(piece, tmpPiece, row, col, row - 2, col - 1);
                                counter++;
                            }
                            if (tmpPiece != NULL){
                                if (tmpPiece -> team == bTeam){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row - 2;
                                    dataNewC[counter] = col - 1;
                                    dataScore[counter] = Scoring(piece, tmpPiece, row, col, row - 2, col - 1);
                                    counter++;
                                }
                            }
                        }
                    }   // exit knights
                    if (piece -> type == 'B') {  // check bishops
                        for (i = 1; i < 8; i++){
                            // check top left diagonal
                            if ((Bblock1 == 0) && (row - i >= 0) && (col - i >= 0)){
                                PIECE * tmpPiece = NULL;
                                tmpPiece = getPieceAt(row - i, col - i);
                                if (tmpPiece == NULL){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row - i;
                                    dataNewC[counter] = col - i;
                                    dataScore[counter] = Scoring(piece, tmpPiece, row, col, row - i, col - i);
                                    counter++;
                                }
                                if (tmpPiece != NULL){
                                    if (tmpPiece -> team == bTeam){
                                        dataOldR[counter] = row;
                                        dataOldC[counter] = col;
                                        dataNewR[counter] = row - i;
                                        dataNewC[counter] = col - i;
                                        dataScore[counter] = tempScore;
                                        counter++;
                                    }
                                    Bblock1 = 1;    
                                }
                            }
                            // check bottom left diagonal
                            if ((Bblock2 == 0) && (row + i < 8) && (col - i >= 0)){
                                PIECE * tmpPiece = NULL;
                                tmpPiece = getPieceAt(row + i, col - i);
                                if (tmpPiece == NULL){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row + i;
                                    dataNewC[counter] = col - i;
                                    dataScore[counter] = Scoring(piece, tmpPiece, row, col, row + i, col - i);
                                    counter++;
                                }
                                if (tmpPiece != NULL){
                                    if (tmpPiece -> team == bTeam){
                                        dataOldR[counter] = row;
                                        dataOldC[counter] = col;
                                        dataNewR[counter] = row + i;
                                        dataNewC[counter] = col - i;
                                        dataScore[counter] = Scoring(piece, tmpPiece, row, col, row + i, col - i);
                                        counter++;
                                    }
                                    Bblock2 = 1;    
                                }
                            }
                            // check top right diagonal
                            if ((Bblock3 == 0) && (row - i >= 0) && (col + i < 8)){
                                PIECE * tmpPiece = NULL;
                                tmpPiece = getPieceAt(row - i, col + i);
                                if (tmpPiece == NULL){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row - i;
                                    dataNewC[counter] = col + i;
                                    dataScore[counter] = Scoring(piece, tmpPiece, row, col, row - i, col + i);
                                    counter++;
                                }
                                if (tmpPiece != NULL){
                                    if (tmpPiece -> team == bTeam){
                                        dataOldR[counter] = row;
                                        dataOldC[counter] = col;
                                        dataNewR[counter] = row - i;
                                        dataNewC[counter] = col + i;
                                        dataScore[counter] = Scoring(piece, tmpPiece, row, col, row - i, col + i);
                                        counter++;
                                    }
                                    Bblock3 = 1;    
                                }
                            }
                            // check bottom right diagonal
                            if ((Bblock4 == 0) && (row + i < 8) && (col + i < 8)){
                                PIECE * tmpPiece = NULL;
                                tmpPiece = getPieceAt(row + i, col + i);
                                if (tmpPiece == NULL){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row + i;
                                    dataNewC[counter] = col + i;
                                    dataScore[counter] = Scoring(piece, tmpPiece, row, col, row + i, col + i);
                                    counter++;
                                }
                                if (tmpPiece != NULL){
                                    if (tmpPiece -> team == bTeam){
                                        dataOldR[counter] = row;
                                        dataOldC[counter] = col;
                                        dataNewR[counter] = row + i;
                                        dataNewC[counter] = col + i;
                                        dataScore[counter] = Scoring(piece, tmpPiece, row, col, row + i, col + i);
                                        counter++;
                                    }
                                    Bblock4 = 1;    
                                }
                            }
                        }   // exit for loop
                    }   // exit bishop
                    if (piece -> type == 'Q') {  // check queen
                        for (i = 1; i < 8; i++){
                            // check vertical up
                            if ((Qblock1 == 0) && (row - i >= 0)){
                                PIECE * tmpPiece = NULL;
                                tmpPiece = getPieceAt(row - i, col);
                                if (tmpPiece == NULL){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row - i;
                                    dataNewC[counter] = col;
                                    dataScore[counter] = Scoring(piece, tmpPiece, row, col, row - i, col);
                                    counter++;
                                }
                                if (tmpPiece != NULL){   // same as: (getSquareAt(row - i, col) != NULL)
                                    if (tmpPiece -> team == bTeam){
                                        dataOldR[counter] = row;
                                        dataOldC[counter] = col;
                                        dataNewR[counter] = row - i;
                                        dataNewC[counter] = col;
                                        dataScore[counter] = Scoring(piece, tmpPiece, row, col, row - i, col);
                                        counter++;
                                    }
                                    Qblock1 = 1;    // update variable so AI won't keep checking past blocked path (SAVE TIME!)
                                } 
                            }
                            // check vertical down
                            if ((Qblock2 == 0) && (row + i < 8)){
                                PIECE * tmpPiece = NULL;
                                tmpPiece = getPieceAt(row + i, col);
                                if (tmpPiece == NULL){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row + i;
                                    dataNewC[counter] = col;
                                    dataScore[counter] = Scoring(piece, tmpPiece, row, col, row + i, col);
                                    counter++;
                                }
                                if (tmpPiece != NULL){   // same as: (getSquareAt(row - i, col) != NULL)
                                    if (tmpPiece -> team == bTeam){
                                        dataOldR[counter] = row;
                                        dataOldC[counter] = col;
                                        dataNewR[counter] = row + i;
                                        dataNewC[counter] = col;
                                        dataScore[counter] = Scoring(piece, tmpPiece, row, col, row + i, col);
                                        counter++;
                                    }
                                    Qblock2 = 1;    // update variable so AI won't keep checking past blocked path (SAVE TIME!)
                                }
                            }
                            // check horizontal left
                            if ((Qblock3 == 0) && (col - i >= 0)){
                                PIECE * tmpPiece = NULL;
                                tmpPiece = getPieceAt(row, col - i);
                                if (tmpPiece == NULL){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row;
                                    dataNewC[counter] = col - i;
                                    dataScore[counter] = Scoring(piece, tmpPiece, row, col, row, col - i);
                                    counter++;
                                }
                                if (tmpPiece != NULL){   // same as: (getSquareAt(row - i, col) != NULL)
                                    if (tmpPiece -> team == bTeam){
                                        dataOldR[counter] = row;
                                        dataOldC[counter] = col;
                                        dataNewR[counter] = row;
                                        dataNewC[counter] = col - i;
                                        dataScore[counter] = Scoring(piece, tmpPiece, row, col, row, col - i);
                                        counter++;
                                    }
                                    Qblock3 = 1;    // update variable so AI won't keep checking past blocked path (SAVE TIME!)
                                }
                            }
                            // check horizontal right
                            if ((Qblock4 == 0) && (col + i >= 0)){
                                PIECE * tmpPiece = NULL;
                                tmpPiece = getPieceAt(row, col + i);
                                if (tmpPiece == NULL){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row;
                                    dataNewC[counter] = col + i;
                                    dataScore[counter] = Scoring(piece, tmpPiece, row, col, row, col + i);
                                    counter++;
                                }
                                if (tmpPiece != NULL){   // same as: (getSquareAt(row - i, col) != NULL)
                                    if (tmpPiece -> team == bTeam){
                                        dataOldR[counter] = row;
                                        dataOldC[counter] = col;
                                        dataNewR[counter] = row;
                                        dataNewC[counter] = col + i;
                                        dataScore[counter] = Scoring(piece, tmpPiece, row, col, row, col + i);
                                        counter++;
                                    }
                                    Qblock4 = 1;    // update variable so AI won't keep checking past blocked path (SAVE TIME!)
                                }
                            }
                            // check top left diagonal
                            if ((Qblock5 == 0) && (row - i >= 0) && (col - i >= 0)){
                                PIECE * tmpPiece = NULL;
                                tmpPiece = getPieceAt(row - i, col - i);
                                if (tmpPiece == NULL){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row - i;
                                    dataNewC[counter] = col - i;
                                    dataScore[counter] = Scoring(piece, tmpPiece, row, col, row - i, col - i);
                                    counter++;
                                }
                                if (tmpPiece != NULL){   // same as: (getSquareAt(row - i, col) != NULL)
                                    if (tmpPiece -> team == bTeam){
                                        dataOldR[counter] = row;
                                        dataOldC[counter] = col;
                                        dataNewR[counter] = row - i;
                                        dataNewC[counter] = col - i;
                                        dataScore[counter] = Scoring(piece, tmpPiece, row, col, row - i, col - i);
                                        counter++;
                                    }
                                    Qblock5 = 1;    // update variable so AI won't keep checking past blocked path (SAVE TIME!)
                                }
                            }
                            // check bottom left diagonal
                            if ((Qblock6 == 0) && (row + i < 8) && (col - i >= 0)){
                                PIECE * tmpPiece = NULL;
                                tmpPiece = getPieceAt(row + i, col - i);
                                if (tmpPiece == NULL){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row + i;
                                    dataNewC[counter] = col - i;
                                    dataScore[counter] = tempScore;
                                    counter++;
                                }
                                if (tmpPiece != NULL){   // same as: (getSquareAt(row - i, col) != NULL)
                                    if (tmpPiece -> team == bTeam){
                                        dataOldR[counter] = row;
                                        dataOldC[counter] = col;
                                        dataNewR[counter] = row + i;
                                        dataNewC[counter] = col - i;
                                        dataScore[counter] = tempScore;
                                        counter++;
                                    }
                                    Qblock6 = 1;    // update variable so AI won't keep checking past blocked path (SAVE TIME!)
                                }
                            }
                            // check top right diagonal
                            if ((Qblock7 == 0) && (row - i >= 0) && (col + i < 8)){
                                PIECE * tmpPiece = NULL;
                                tmpPiece = getPieceAt(row - i, col + i);
                                if (tmpPiece == NULL){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row - i;
                                    dataNewC[counter] = col + i;
                                    dataScore[counter] = Scoring(piece, tmpPiece, row, col, row - i, col + i);
                                    counter++;
                                }
                                if (tmpPiece != NULL){   // same as: (getSquareAt(row - i, col) != NULL)
                                    if (tmpPiece -> team == bTeam){
                                        dataOldR[counter] = row;
                                        dataOldC[counter] = col;
                                        dataNewR[counter] = row - i;
                                        dataNewC[counter] = col + i;
                                        dataScore[counter] = Scoring(piece, tmpPiece, row, col, row - i, col + i);
                                        counter++;
                                    }
                                    Qblock7 = 1;    // update variable so AI won't keep checking past blocked path (SAVE TIME!)
                                }
                            }
                            // check bottom right diagonal
                            if ((Qblock8 == 0) && (row + i < 8) && (col + i < 8)){
                                PIECE * tmpPiece = NULL;
                                tmpPiece = getPieceAt(row + i, col + i);
                                if (tmpPiece == NULL){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row + i;
                                    dataNewC[counter] = col + i;
                                    dataScore[counter] = Scoring(piece, tmpPiece, row, col, row + i, col + i);
                                    counter++;
                                }
                                if (tmpPiece != NULL){   // same as: (getSquareAt(row - i, col) != NULL)
                                    if (tmpPiece -> team == bTeam){
                                        dataOldR[counter] = row;
                                        dataOldC[counter] = col;
                                        dataNewR[counter] = row + i;
                                        dataNewC[counter] = col + i;
                                        dataScore[counter] = Scoring(piece, tmpPiece, row, col, row + i, col + i);
                                        counter++;
                                    }
                                    Qblock8 = 1;    // update variable so AI won't keep checking past blocked path (SAVE TIME!)
                                }
                            }
                        }   // exit for loop
                    }   // exit Queen
                    if (piece -> type == 'K') {  // check king
                        // L1, above
                        if (row - 1 >= 0){
                            PIECE * tmpPiece = NULL;
                            tmpPiece = getPieceAt(row - 1, col);
                            if (tmpPiece == NULL){
                                dataOldR[counter] = row;
                                dataOldC[counter] = col;
                                dataNewR[counter] = row - 1;
                                dataNewC[counter] = col;
                                dataScore[counter] = Scoring(piece, tmpPiece, row, col, row - 1, col);
                                counter++;
                            }
                            if (tmpPiece != NULL){
                                if (tmpPiece -> team == bTeam){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row - 1;
                                    dataNewC[counter] = col;
                                    dataScore[counter] = Scoring(piece, tmpPiece, row, col, row - 1, col);
                                    counter++;
                                }
                            }
                        }
                        // L2, top right
                        if ((row - 1 >= 0) && (col + 1 < 8)){
                            PIECE * tmpPiece = NULL;
                            tmpPiece = getPieceAt(row - 1, col + 1);
                            if (tmpPiece == NULL){
                                dataOldR[counter] = row;
                                dataOldC[counter] = col;
                                dataNewR[counter] = row - 1;
                                dataNewC[counter] = col + 1;
                                dataScore[counter] = Scoring(piece, tmpPiece, row, col, row - 1, col + 1);
                                counter++;
                            }
                            if (tmpPiece != NULL){
                                if (tmpPiece -> team == bTeam){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row - 1;
                                    dataNewC[counter] = col + 1;
                                    dataScore[counter] = Scoring(piece, tmpPiece, row, col, row - 1, col + 1);
                                    counter++;
                                }
                            }
                        }
                        // L3, right
                        if (col + 1 < 8){
                            PIECE * tmpPiece = NULL;
                            tmpPiece = getPieceAt(row, col + 1);
                            if (tmpPiece == NULL){
                                dataOldR[counter] = row;
                                dataOldC[counter] = col;
                                dataNewR[counter] = row;
                                dataNewC[counter] = col + 1;
                                dataScore[counter] = Scoring(piece, tmpPiece, row, col, row, col + 1);
                                counter++;
                            }
                            if (tmpPiece != NULL){
                                if (tmpPiece -> team == bTeam){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row;
                                    dataNewC[counter] = col + 1;
                                    dataScore[counter] = Scoring(piece, tmpPiece, row, col, row, col + 1);
                                    counter++;
                                    
                                }
                            }
                        }
                        // L4, bottom right
                        if ((row + 1 < 8) && (col + 1 < 8)){
                            PIECE * tmpPiece = NULL;
                            tmpPiece = getPieceAt(row + 1, col + 1);
                            if (tmpPiece == NULL){
                                dataOldR[counter] = row;
                                dataOldC[counter] = col;
                                dataNewR[counter] = row + 1;
                                dataNewC[counter] = col + 1;
                                dataScore[counter] = Scoring(piece, tmpPiece, row, col, row + 1, col + 1);
                                counter++;
                            }
                            if (tmpPiece != NULL){
                                if (tmpPiece -> team == bTeam){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row + 1;
                                    dataNewC[counter] = col + 1;
                                    dataScore[counter] = Scoring(piece, tmpPiece, row, col, row + 1, col + 1);
                                    counter++;
                                }
                            }
                        }
                        // L5, under
                        if (row + 1 < 8){
                            PIECE * tmpPiece = NULL;
                            tmpPiece = getPieceAt(row + 1, col);
                            if (tmpPiece == NULL){
                                dataOldR[counter] = row;
                                dataOldC[counter] = col;
                                dataNewR[counter] = row + 1;
                                dataNewC[counter] = col;
                                dataScore[counter] = Scoring(piece, tmpPiece, row, col, row + 1, col);
                                counter++;
                            }
                            if (tmpPiece != NULL){
                                if (tmpPiece -> team == bTeam){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row + 1;
                                    dataNewC[counter] = col;
                                    dataScore[counter] = Scoring(piece, tmpPiece, row, col, row + 1, col);
                                    counter++;
                                }
                            }
                        }
                        // L6, bottom left
                        if ((row + 1 < 8) && (col - 1 >= 0)){
                            PIECE * tmpPiece = NULL;
                            tmpPiece = getPieceAt(row + 1, col - 1);
                            if (tmpPiece == NULL){
                                dataOldR[counter] = row;
                                dataOldC[counter] = col;
                                dataNewR[counter] = row + 1;
                                dataNewC[counter] = col - 1;
                                dataScore[counter] = Scoring(piece, tmpPiece, row, col, row + 1, col - 1);
                                counter++;
                                
                            }
                            if (tmpPiece != NULL){
                                if (tmpPiece -> team == bTeam){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row + 1;
                                    dataNewC[counter] = col - 1;
                                    dataScore[counter] = Scoring(piece, tmpPiece, row, col, row + 1, col - 1);
                                    counter++;
                                }
                            }
                        }
                        // L7, left
                        if (col - 1 >= 0){
                            PIECE * tmpPiece = NULL;
                            tmpPiece = getPieceAt(row, col - 1);
                            if (tmpPiece == NULL){
                                dataOldR[counter] = row;
                                dataOldC[counter] = col;
                                dataNewR[counter] = row;
                                dataNewC[counter] = col - 1;
                                dataScore[counter] = Scoring(piece, tmpPiece, row, col, row, col - 1);
                                counter++;
                            }
                            if (tmpPiece != NULL){
                                if (tmpPiece -> team == bTeam){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row;
                                    dataNewC[counter] = col - 1;
                                    dataScore[counter] = Scoring(piece, tmpPiece, row, col, row, col - 1);
                                    counter++;
                                }
                            }
                        }
                        // L8, top left
                        if ((row - 1 >= 0) && (col - 1 >= 0)){
                            PIECE * tmpPiece = NULL;
                            tmpPiece = getPieceAt(row - 1, col - 1);
                            if (tmpPiece == NULL){
                                dataOldR[counter] = row;
                                dataOldC[counter] = col;
                                dataNewR[counter] = row - 1;
                                dataNewC[counter] = col - 1;
                                dataScore[counter] = Scoring(piece, tmpPiece, row, col, row - 1, col - 1);
                                counter++;
                            }
                            if (tmpPiece != NULL){
                                if (tmpPiece -> team == bTeam){
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row - 1;
                                    dataNewC[counter] = col - 1;
                                    dataScore[counter] = Scoring(piece, tmpPiece, row, col, row - 1, col - 1);
                                    counter++;
                                }  
                            }
                        }
                        //L9, two units right (castle to the right)
                        if (col + 2 == 7){
                            PIECE * tmpPiece = NULL;
                            //printf("Checking for right castle");
                            tmpPiece = getPieceAt(row, col + 2);
                            if (tmpPiece == NULL){
                                if(isValidCastling(piece, row, col+2)){ //this function just checks the movements, it doesnt move anything
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row;
                                    dataNewC[counter] = col + 2;
                                    dataScore[counter] = Scoring(piece, tmpPiece, row, col, row, col + 2);
                                    counter++;
                                }
                            }
                        }
                        //L10, two units left (castle to the left)
                        if (col - 2 == 3){
                            PIECE * tmpPiece = NULL;
                            tmpPiece = getPieceAt(row, col - 2);
                            if (tmpPiece == NULL){
                                if(isValidCastling(piece, row, col+2)){ //this function just checks the movements, it doesnt move anything
                                    dataOldR[counter] = row;
                                    dataOldC[counter] = col;
                                    dataNewR[counter] = row;
                                    dataNewC[counter] = col - 2;
                                    dataScore[counter] = Scoring(piece, tmpPiece, row, col, row, col - 2);
                                    counter++;
                                }
                            }
                        } 
                    }   // exit king
                }   // exit aTeam
            }   // exit != NULL
        }   // exit col
    }   // exit row
    
    // organize the arrays from least to greatest if BLACK
    if (aTeam == 'B'){
        for (i = 0; i < counter; i++){
            for (j = 0; j < counter; j++){
                if (dataScore[j] > dataScore[i]){
                    tmp = dataScore[i];
                    dataScore[i] = dataScore[j];
                    dataScore[j] = tmp;
                    tmp = dataOldR[i];
                    dataOldR[i] = dataOldR[j];
                    dataOldR[j] = tmp;
                    tmp = dataOldC[i];
                    dataOldC[i] = dataOldC[j];
                    dataOldC[j] = tmp;
                    tmp = dataNewR[i];
                    dataNewR[i] = dataNewR[j];
                    dataNewR[j] = tmp;
                    tmp = dataNewC[i];
                    dataNewC[i] = dataNewC[j];
                    dataNewC[j] = tmp;
                }
            }
        }
    }
    // organize the arrays from least to greatest if WHITE
    else if (aTeam == 'W'){
        for (i = 0; i < counter; i++){
            for (j = 0; j < counter; j++){
                if (dataScore[j] < dataScore[i]){
                    tmp = dataScore[i];
                    dataScore[i] = dataScore[j];
                    dataScore[j] = tmp;
                    tmp = dataOldR[i];
                    dataOldR[i] = dataOldR[j];
                    dataOldR[j] = tmp;
                    tmp = dataOldC[i];
                    dataOldC[i] = dataOldC[j];
                    dataOldC[j] = tmp;
                    tmp = dataNewR[i];
                    dataNewR[i] = dataNewR[j];
                    dataNewR[j] = tmp;
                    tmp = dataNewC[i];
                    dataNewC[i] = dataNewC[j];
                    dataNewC[j] = tmp;
                }
            }
        }
    }
    
    
    return (counter);


}//  EOF