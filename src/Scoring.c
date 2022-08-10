//////////////////////////////////////////////////////////////////
/*  Scoring.c                                                   */
/*  Version: 2 ; 4/30/20                                        */
/*                                                              */
/*  Function sets score values for different moves. It updates  */
/*  the scores throughout the chess game as pieces get removed  */
/*  from the board. (This function is called in AI_PosMoves())  */
//////////////////////////////////////////////////////////////////

#include "Scoring.h"
#include "Movement.h"
#include "Piece.h"
#include "Board.h"
#include "Check.h"
#include <stdio.h>

int Scoring(PIECE * piece, PIECE * futurePiece, int oRow, int oCol, int fRow, int fCol){
    int score = 0;
    char aTeam, bTeam;
    int dir;
    int posNeg;
    int i;
    int temp = 0;
    int friendly = 0;
    int spotprotect = 0;
    int consider = 0;
    
    if (piece -> team == 'B'){
        aTeam = 'B';
        bTeam = 'W';
        dir = 1;    // pieces are moving down the board
        posNeg = -1;
    }
    else if (piece -> team == 'W'){
        aTeam = 'W';
        bTeam = 'B';
        dir = -1;   // pieces are moving up the board
        posNeg = 1;
    }
    
    // Give scores based on piece type that was "eaten" and piece type that is "eating"
    if (futurePiece != NULL){   // check if the new location has a piece there
        if (futurePiece -> team == bTeam){  // check if that piece is an opponent
            if (futurePiece -> type == 'P'){
                score = score + (40 * posNeg);
                if (piece -> type == 'P'){
                    score = score + (10 * posNeg);
                    if (consider == 1){
                        score = score - (20 * posNeg);
                    }
                }
                else if (piece -> type == 'R'){
                    score = score + (4 * posNeg);
                    if (consider == 1){
                        score = score - (50 * posNeg);
                    }
                }
                else if (piece -> type == 'N'){
                    score = score + (4 * posNeg);
                    if (consider == 1){
                        score = score - (50 * posNeg);
                    }
                }
                else if (piece -> type == 'B'){
                    score = score + (4 * posNeg);
                    if (consider == 1){
                        score = score - (50 * posNeg);
                    }
                }
                else if (piece -> type == 'Q'){
                    score = score + (4 * posNeg);
                    if (consider == 1){
                        score = score - (150 * posNeg);
                    }
                }
                else if (piece -> type == 'K'){
                    score = score + (4 * posNeg);
                }
            }
            else if (futurePiece -> type == 'R'){
                score = score + (70 * posNeg);
                if (piece -> type == 'P'){
                    score = score + (10 * posNeg);
                    if (consider == 1){
                        score = score - (200 * posNeg);
                    }
                }
                else if (piece -> type == 'R'){
                    score = score + (4 * posNeg);
                    if (consider == 1){
                        score = score - (50 * posNeg);
                    }
                }
                else if (piece -> type == 'N'){
                    score = score + (4 * posNeg);
                    if (consider == 1){
                        score = score - (50 * posNeg);
                    }
                }
                else if (piece -> type == 'B'){
                    score = score + (4 * posNeg);
                    if (consider == 1){
                        score = score - (50 * posNeg);
                    }
                }
                else if (piece -> type == 'Q'){
                    score = score + (4 * posNeg);
                    if (consider == 1){
                        score = score - (150 * posNeg);
                    }
                }
                else if (piece -> type == 'K'){
                    score = score + (4 * posNeg);
                    if (consider == 1){
                        score = score - (50 * posNeg);
                    }
                }
            }
            else if (futurePiece -> type == 'N'){
                score = score + (90 * posNeg);
                if (piece -> type == 'P'){
                    score = score + (10 * posNeg);
                    if (consider == 1){
                        score = score - (20 * posNeg);
                    }
                }
                else if (piece -> type == 'R'){
                    score = score + (4 * posNeg);
                    if (consider == 1){
                        score = score - (50 * posNeg);
                    }
                }
                else if (piece -> type == 'N'){
                    score = score + (4 * posNeg);
                    if (consider == 1){
                        score = score - (50 * posNeg);
                    }
                }
                else if (piece -> type == 'B'){
                    score = score + (4 * posNeg);
                    if (consider == 1){
                        score = score - (50 * posNeg);
                    }
                }
                else if (piece -> type == 'Q'){
                    score = score + (4 * posNeg);
                    if (consider == 1){
                        score = score - (150 * posNeg);
                    }
                }
                else if (piece -> type == 'K'){
                    score = score + (4 * posNeg);
                }
            }
            else if (futurePiece -> type == 'B'){
                score = score + (70 * posNeg);
                if (piece -> type == 'P'){
                    score = score + (10 * posNeg);
                    if (consider == 1){
                        score = score - (20 * posNeg);
                    }
                }
                else if (piece -> type == 'R'){
                    score = score + (4 * posNeg);
                    if (consider == 1){
                        score = score - (50 * posNeg);
                    }
                }
                else if (piece -> type == 'N'){
                    score = score + (4 * posNeg);
                    if (consider == 1){
                        score = score - (50 * posNeg);
                    }
                }
                else if (piece -> type == 'B'){
                    score = score + (4 * posNeg);
                    if (consider == 1){
                        score = score - (50 * posNeg);
                    }
                }
                else if (piece -> type == 'Q'){
                    score = score + (4 * posNeg);
                    if (consider == 1){
                        score = score - (150 * posNeg);
                    }
                }
                else if (piece -> type == 'K'){
                    score = score + (4 * posNeg);
                }
            }
            else if (futurePiece -> type == 'Q'){
                score = score + (150 * posNeg);
                if (piece -> type == 'P'){
                    score = score + (10 * posNeg);
                    if (consider == 1){
                        score = score - (20 * posNeg);
                    }
                }
                else if (piece -> type == 'R'){
                    score = score + (4 * posNeg);
                    if (consider == 1){
                        score = score - (50 * posNeg);
                    }
                }
                else if (piece -> type == 'N'){
                    score = score + (4 * posNeg);
                    if (consider == 1){
                        score = score - (50 * posNeg);
                    }
                }
                else if (piece -> type == 'B'){
                    score = score + (4 * posNeg);
                    if (consider == 1){
                        score = score - (50 * posNeg);
                    }
                }
                else if (piece -> type == 'Q'){
                    score = score + (4 * posNeg);
                    if (consider == 1){
                        score = score - (150 * posNeg);
                    }
                }
                else if (piece -> type == 'K'){
                    score = score + (4 * posNeg);
                }
            }
            else if (futurePiece -> type == 'K'){
                score = score + (125 * posNeg);
                consider = isAttacked(piece -> team, futurePiece -> row, futurePiece -> col);
                if (piece -> type == 'P'){
                    score = score + (10 * posNeg);
                    if (consider == 1){
                        score = score - (20 * posNeg);
                    }
                }
                else if (piece -> type == 'R'){
                    score = score + (4 * posNeg);
                    if (consider == 1){
                        score = score - (50 * posNeg);
                    }
                }
                else if (piece -> type == 'N'){
                    score = score + (4 * posNeg);
                    if (consider == 1){
                        score = score - (50 * posNeg);
                    }
                }
                else if (piece -> type == 'B'){
                    score = score + (4 * posNeg);
                    if (consider == 1){
                        score = score - (50 * posNeg);
                    }
                }
                else if (piece -> type == 'Q'){
                    score = score + (4 * posNeg);
                    if (consider == 1){
                        score = score - (150 * posNeg);
                    }
                }
                else if (piece -> type == 'K'){
                    score = score + (4 * posNeg);
                }
            }
        }
        temp = isAttacked(piece -> team, futurePiece -> row, futurePiece -> col);   // check if new location is being attacked
    }
    
    // Check if new location is being protected by other teammates
    for (i = 1; i < 8; i++){
        if ((fRow - (i * dir) >= 0) && (fRow - (i * dir) < 8)){
            if ((fCol - (i * dir) >= 0) && (fCol - (i * dir))){
                PIECE * tmpPiece = NULL;
                tmpPiece = getPieceAt(fRow - (i * dir), fCol - (i * dir));
                if ((tmpPiece != NULL) && (friendly >= 0)){
                    if (tmpPiece->type == aTeam){
                        if (tmpPiece -> type == 'P'){
                            score = score + (30 * posNeg);
                        }
                        else if (tmpPiece -> type == 'B'){
                            score = score + (20 * posNeg);
                        }
                        else if (tmpPiece -> type == 'Q'){
                            score = score + (20 * posNeg);
                        }
                        else if (tmpPiece -> type == 'K'){
                            score = score + (20 * posNeg);
                        }
                        friendly++;
                        spotprotect++;
                    }
                    else if (tmpPiece -> type == bTeam){
                        friendly = -1;
                    }
                }
                else if ((tmpPiece != NULL) && (friendly < 0)){
                    if (tmpPiece->type == aTeam){
                        if (tmpPiece -> type == 'B'){
                            score = score + (20 * posNeg);
                        }
                        else if (tmpPiece -> type == 'Q'){
                            score = score + (20 * posNeg);
                        }
                        friendly = 1;
                        spotprotect++;
                    }
                } 
                score = score + (friendly * (30 * posNeg));
            }
        }

        if ((fRow - (i * dir) >= 0) && (fRow - (i * dir) < 8)){
            if ((fCol + (i * dir) >= 0) && (fCol + (i * dir))){
                PIECE * tmpPiece = NULL;
                tmpPiece = getPieceAt(fRow - (i * dir), fCol + (i * dir));
                if ((tmpPiece != NULL) && (friendly >= 0)){
                    if (tmpPiece->type == aTeam){
                        if (tmpPiece -> type == 'P'){
                            score = score + (30 * posNeg);
                        }
                        else if (tmpPiece -> type == 'B'){
                            score = score + (20 * posNeg);
                        }
                        else if (tmpPiece -> type == 'Q'){
                            score = score + (20 * posNeg);
                        }
                        else if (tmpPiece -> type == 'K'){
                            score = score + (20 * posNeg);
                        }
                        friendly++;
                        spotprotect++;
                    }
                    else if (tmpPiece -> type == bTeam){
                        friendly = -1;
                    }
                }
                else if ((tmpPiece != NULL) && (friendly < 0)){
                    if (tmpPiece->type == aTeam){
                        if (tmpPiece -> type == 'B'){
                            score = score + (20 * posNeg);
                        }
                        else if (tmpPiece -> type == 'Q'){
                            score = score + (20 * posNeg);
                        }
                        friendly = 1;
                        spotprotect++;
                    }
                } 
                score = score + (friendly * (30 * posNeg));
            }
        }
        
        if ((fRow - (i * dir) >= 0) && (fRow - (i * dir) < 8)){
            PIECE * tmpPiece = NULL;
            tmpPiece = getPieceAt(fRow - (i * dir), fCol);
            if ((tmpPiece != NULL) && (friendly >= 0)){
                if (tmpPiece->type == aTeam){
                    if (tmpPiece -> type == 'R'){
                        score = score + (20 * posNeg);
                    }
                    else if (tmpPiece -> type == 'Q'){
                        score = score + (20 * posNeg);
                    }
                    else if (tmpPiece -> type == 'K'){
                        score = score + (10 * posNeg);
                    }
                    friendly++;
                    spotprotect++;
                }
                else if (tmpPiece -> type == bTeam){
                    friendly = -1;
                }
            }
            else if ((tmpPiece != NULL) && (friendly < 0)){
                if (tmpPiece->type == aTeam){
                    if (tmpPiece -> type == 'R'){
                        score = score + (20 * posNeg);
                    }
                    else if (tmpPiece -> type == 'Q'){
                        score = score + (20 * posNeg);
                    }
                    friendly = 1;
                    spotprotect++;
                }
            } 
            score = score + (friendly * (30 * posNeg));
        }
    }

    if (temp == 1){ // if the location is being attacked, give points based on if a teammate can protect that location
        if (spotprotect > 0){
            score = score + (spotprotect * (10 * posNeg));
        }
        else if (spotprotect == 0){
            if (piece -> type == 'P'){
                score = score - (10 * posNeg);
            }
            else if (piece -> type == 'R'){
                score = score - (50 * posNeg);
            }
            else if (piece -> type == 'N'){
                score = score - (50 * posNeg);
            }
            else if (piece -> type == 'B'){
                score = score - (50 * posNeg);
            }
            else if (piece -> type == 'Q'){
                score = score - (100 * posNeg);
            }
        }
    }
    else if (temp == 0){    // if the new location is not being attacked give it additional points
        score = score + (60 * posNeg);
    }

    return (score);

}


//  EOF
