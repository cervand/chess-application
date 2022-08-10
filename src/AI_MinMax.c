//////////////////////////////////////////////////////////////////
/*  AI_MinMax.c                                                */
/*  Version: 2 ; 4/30/20                                        */
/*                                                              */
/*  Function determines which move is best for the AI. Uses a   */
/*  MiniMax Algorithm to determine the best move possible.      */
//////////////////////////////////////////////////////////////////

#include "AI_PosMoves.h"
#include "AI_MinMax.h"
#include "Movement.h"
#include "Piece.h"
#include "Square.h"
#include "Board.h"
#include "Check.h"
#include "Checkmate.h"
#include <stdio.h>

int AI_MinMax(char aTeam){    
    int i;
    int fcounter;
    int temp = 0;
    int checkmate = 0;

    fcounter = AI_PosMoves(aTeam);  // find original possible moves for current board
    checkmate = totalCheckmate(aTeam);

    if (checkmate == 0){    // if not in checkmate
        while (temp == 0){
            for (i = 0; i < fcounter; i++){
                temp = movePieceTo(getPieceAt(dataOldR[i], dataOldC[i]), dataOldR[i], dataOldC[i], dataNewR[i], dataNewC[i]);
                if (temp == 1){
                    break;  // move is legal
                }
                else if ((temp == 0) && (i == fcounter - 1)){   // if there are no more legal moves, it is in checkmate
                    checkmate = 1;
                    temp = 1;
                    break;
                }
            }
        }
    }
    return checkmate;
}


//  EOF
