/*Checkmate.c */
/*This is the implementation for determining whether or not the king is in checkmate */
/*If all 3 conditions indicate a  checkmate flag then king is in complete checkmate  */

/*
4/29 Taylor 
    - created draft for checkmate 
    - need to fix parameters for PosMoves
    - created moveoutCheck
4/30 Taylor
    - implementing blocking check
    - created canBlock
4/30 Trenton
    - moved canBlock to checkmate.c
    - made moveoutCheck independent of AI_PosMoves
    - finished? blockCheck code
5/2 Taylor
    - debugging 
    - added pawn onto pieces that can't be blocked
5/3 Taylor
    - creating the main function of checkmate (totalCheck)
    - adding onto makefile 
    - fixing moveoutCheck to use king's coordinates and not any selected piece 
    - debugging blockCheck 
*/

#include "Square.h"
#include "Board.h"
#include "Piece.h"
#include "Checkmate.h"
#include "Movement.h"
#include "Scoring.h"
#include "Check.h"
#include <stdio.h>
#include <stdlib.h>

int totalCheckmate(char teamBias){
    int checkmate = 1; 
    //printf("\nmoveoutCheck is %d\n", moveoutCheck(teamBias));
    //printf("\nblockCheck is %d\n", blockCheck(teamBias));
    if ((moveoutCheck(teamBias) == 1) && (blockCheck(teamBias) == 1)){
        return checkmate; 
    }
    else{
        return 0;
    }

}



// moveoutCheck will check whether or not you can move a king out of check 
//returns int moveCheckmate 
int moveoutCheck(char teamBias){
    int kingRow;
    int kingCol;
    PIECE * King = NULL;
    int moveCheckmate = 0;
    int validsquares = 0;                   //how many squares around the king are actually movable to/from(valid row/col)
    int spacetocheckR[8];                   //stores locations that aren't a valid space
    int spacetocheckC[8];
    int counter = 0;

    //find defenders king piece 
    for(int row = 0; row< 8; row++){
        for(int col = 0; col < 8; col++){
            if(getPieceAt(row,col) != NULL){
                    if((getPieceAt(row,col)->type == 'K') && (getPieceAt(row,col)->team == teamBias)){
                        King = getPieceAt(row,col);  //save king to array 
                    }
            }
        }
    }

    // variables for king's coordinates
    kingRow = King->row;                 //seg
    kingCol = King->col;
    //checks each square around the king to see if they are under attack or have a piece there.
    //I also edited isAttacked and getPieceAt so they shouldn't seg fault if the square is out of bounds.
    for (int i = -1 + kingRow; i < 2 + kingRow; i++) {
        for (int j = -1 + kingCol; j < 2 + kingCol; j++) {
            if ((i < 0) || (i > 7) || (j < 0) || (j > 7)) {     //out of bounds
                validsquares = validsquares;
            } else  if ((kingRow == i) && (kingCol == j)) {                           
                validsquares = validsquares;                                        //ignoring king's own position
            } else {
                validsquares++; 
            }
        }
    }


    // Added if statements checking if the surroundings of the King's position is on the board - Trey
    // SHOULD we be checking if isAttacked() == 1 OR != 1 OR != NULL?
    if ((kingRow - 1 >= 0) && (kingCol - 1 >= 0)){
        if(isAttacked(teamBias, kingRow-1, kingCol-1) == 1 || getPieceAt(kingRow-1, kingCol-1) != NULL){
            moveCheckmate++;
        } else if ((getPieceAt(kingRow - 1, kingCol - 1) == NULL) && (isAttacked(teamBias, kingRow - 1, kingCol - 1) == 0)) {
            spacetocheckR[counter] = kingRow -1;
            spacetocheckC[counter] = kingCol -1;
            counter++;
        }
    }
    if ((kingRow - 1 >= 0) && (kingCol + 1 < 8)){
        if(isAttacked(teamBias, kingRow-1, kingCol+1) == 1 || getPieceAt(kingRow-1, kingCol+1) != NULL){
            moveCheckmate++;
        } else if ((getPieceAt(kingRow - 1, kingCol + 1) == NULL) && (isAttacked(teamBias, kingRow -1, kingCol+1) == 0)){
            spacetocheckR[counter] = kingRow -1;
            spacetocheckC[counter] = kingCol +1;
            counter++;
        }
    }
    if (kingRow - 1 >= 0){
        if(isAttacked(teamBias, kingRow-1, kingCol) == 1 || getPieceAt(kingRow-1, kingCol) != NULL){ 
            moveCheckmate++;
        } else if ((getPieceAt(kingRow - 1, kingCol) == NULL) && (isAttacked(teamBias, kingRow -1, kingCol) == 0)){
            spacetocheckR[counter] = kingRow -1;
            spacetocheckC[counter] = kingCol;
            counter++;
        }
    }
    if ((kingRow + 1 < 8) && (kingCol - 1 >= 0)){
        if(isAttacked(teamBias, kingRow+1, kingCol-1) == 1 || getPieceAt(kingRow+1, kingCol-1) != NULL){
            moveCheckmate++;
        } else if ((getPieceAt(kingRow + 1, kingCol - 1) == NULL) && (isAttacked(teamBias, kingRow +1, kingCol-1) == 0)){
            spacetocheckR[counter] = kingRow + 1;
            spacetocheckC[counter] = kingCol - 1;
            counter++;            
        }
    }
    if ((kingRow + 1 < 8) && (kingCol + 1 < 8)){
        if(isAttacked(teamBias, kingRow+1, kingCol+1) == 1 || getPieceAt(kingRow+1, kingCol+1) != NULL){
            moveCheckmate++;
        } else if ((getPieceAt(kingRow + 1, kingCol + 1) == NULL) && (isAttacked(teamBias, kingRow +1, kingCol+1) == 0)){
            spacetocheckR[counter] = kingRow + 1;
            spacetocheckC[counter] = kingCol + 1;
            counter++;            
        }
    }
    if (kingRow + 1 < 8){
        if(isAttacked(teamBias, kingRow+1, kingCol) == 1 || getPieceAt(kingRow+1, kingCol) != NULL){ 
            moveCheckmate++;
        } else if ((getPieceAt(kingRow + 1, kingCol) == NULL) && (isAttacked(teamBias, kingRow +1, kingCol) == 0)){
            spacetocheckR[counter] = kingRow + 1;
            spacetocheckC[counter] = kingCol;
            counter++;                
        }
    }
    if (kingCol - 1 >= 0){
        if(isAttacked(teamBias, kingRow, kingCol-1) == 1 || getPieceAt(kingRow, kingCol-1) != NULL){
            moveCheckmate++;
        } else if ((getPieceAt(kingRow, kingCol - 1) == NULL) && (isAttacked(teamBias, kingRow, kingCol-1) == 0)){
            spacetocheckR[counter] = kingRow;
            spacetocheckC[counter] = kingCol - 1;
            counter++;               
        }
    }
    if (kingCol + 1 < 8){
        if(isAttacked(teamBias, kingRow, kingCol+1) == 1 || getPieceAt(kingRow, kingCol+1) != NULL){
            moveCheckmate++;
        } else if ((getPieceAt(kingRow, kingCol + 1) == NULL) && (isAttacked(teamBias, kingRow, kingCol+1) == 0)){
            spacetocheckR[counter] = kingRow;
            spacetocheckC[counter] = kingCol + 1;
            counter++;               
        }
    }
    boardArray[kingRow][kingCol]->piece_ptr = NULL;                        //if king wasn't there 
    for (int i = 0; i < counter; i++) {        
        if (isAttacked(teamBias, spacetocheckR[i], spacetocheckC[i]) == 1) {        //if that's now a valid space to attack, (king can't block himself)
            moveCheckmate++;
        }
    }
    setPieceAt(King, kingRow, kingCol);                     //putting king back

    if ((moveCheckmate == validsquares) && (isAttacked(teamBias, kingRow, kingCol) == 1)) {                   //all  valid squares around the king are being attacked/a piece is occupying them so he can't move 
        return 1;                                                                                  //and king is being attacked
    } else {
        return 0;
    }

}

//blockCheck determines whether or not you can block check with another piece 
//finds attacking pieces from isAttacked function, then sees if that piece is in line of one of our own pieces 
//if attacker is queen, rook, bishop -------> first detect if you can capture ------> second try to block
//returns 0 if attacker can be captured, returns 1 if attacker cannot be captured 
int blockCheck(char teamBias){
    int i, j, k; 
    int pos = 0; 
    int capCheck = 1;
    int blockCheckmate = 1;
    int finalCheck = 0;
    int maxIndex = 0;
    PIECE * KingPos;
    PIECE * enemyQueenRookBish[5]; 
    PIECE * ourPieces[16]; 
    PIECE * attackingPieces[16];
    int KingRow, KingCol;


    if (Check(teamBias) == 1){
        //find king position of team 
        for(int row = 0; row<= 8; row++){
            for(int col = 0; col <= 8; col++){
                if(getPieceAt(row,col) != NULL){
                    if (getPieceAt(row,col)->type == 'K' && getPieceAt(row,col)->team == teamBias){
                        KingPos = getPieceAt(row,col);  //save king of same team to array 
                    }
                }
            }
        }
        KingRow = getKingRow(teamBias);
        KingCol = getKingCol(teamBias);


        //find attacking pieces
        int tempcounter1 = 0;
        getEnemies(teamBias);
        for(int i = 0; i < 16; i++){
            if(enemyPieces[i] != NULL){
                if((isValidPieceMove(enemyPieces[i], KingRow, KingCol)) == 1){
                    attackingPieces[tempcounter1] = enemyPieces[i];
                    tempcounter1++;
                }
            }
        }
        if (tempcounter1 < 16) {
            for (int a = tempcounter1; a < 16; a++) {               //setting rest to NULL
                attackingPieces[a] = NULL;
            }
        }

        //find queen, rook, bishop that are attacking
        for(int i = 0; i < 16; i++){
            if(attackingPieces[i] != NULL){
                if(attackingPieces[i]->type == 'Q' || attackingPieces[i]->type == 'R' || attackingPieces[i]->type == 'B'){
                    enemyQueenRookBish[pos] = attackingPieces[i];
                    pos++;
                }
            }
        }
        if (pos < 5){
            for (int a = pos; a < 5; a++) {
                enemyQueenRookBish[a] = NULL;                   //setting rest to NULL
            }

        }
        /*
        for(int row = 0; row < 8; row++){
            for(int col = 0; col < 8; col++){
                if(getPieceAt(row,col) != NULL){
                    if(getPieceAt(row,col)->type == 'Q' || getPieceAt(row,col)->type == 'R' || getPieceAt(row,col)->type == 'B'){
                        if(getPieceAt(row,col)->team != teamBias){
                            enemyQueenRookBish[pos] = getPieceAt(row,col);  //save queen, rook, bishop of same team to array 
                            pos++;
                        }
                    }
                }
            }
        }
        if (pos < 5){
            for (int a = pos; a < 5; a++) {
                enemyQueenRookBish[a] = NULL;                   //setting rest to NULL
            }

        }*/
        //store attacking queen,rook,bishop to attacking array
        /*
        int tempcounter = 0;
        for(int i = 0; i < (sizeof(enemyQueenRookBish))/(sizeof(PIECE *)); i++){
            if (enemyQueenRookBish[i] != NULL) {
                if((isValidPieceMove(enemyQueenRookBish[i], KingRow, KingCol) == 1)){
                    attackingQRB[tempcounter] = enemyQueenRookBish[i];
                } else {
                    attackingQRB[tempcounter] = NULL;
                }    
                tempcounter++;            
            }
        }

        if (tempcounter < 5) {
            for (int a = tempcounter; a < 5; a++) {
                attackingQRB[a] = NULL;                             //setting rest to NULL
            }
        }*/
        //put all of our teams pieces in array
        for(int r = 0; r < 8; r++){
            for(int c = 0; c < 8; c++){
                if(getPieceAt(r,c) != NULL){
                    if(getPieceAt(r,c)->team == teamBias){
                        ourPieces[maxIndex] = getPieceAt(r,c);
                        maxIndex++;   
                    }
                }
            }
        }
        if (maxIndex < 16) {
            for (int a = maxIndex; a < 16; a++) {                   //setting the rest of the pieces to NULL
            ourPieces[a] = NULL;
            }
        }
        /*
        //now determine if we can capture attacking piece 
        for(piece = 0; piece < 16; piece++) //for each of our pieces 
        {
            for(qrb = 0; qrb < 16; qrb++) //for each queen, rook, bishop
            {
                if ((attackingPieces[qrb] != NULL) && (ourPieces[piece] != NULL)) {
                    if ((isValidPieceMove(ourPieces[piece], attackingPieces[qrb]->row, attackingPieces[qrb]->col)) == 1) //check if QRB piece can capture attacker
                    {
                        counter = counter + 0;// can capture attacking piece -- not in check
                    }
                    else //if piece cannot capture attacking piece
                    {
                        counter++; // king is still in check
                    }                   
                }
            }
            
        }

        if (counter >= sizeof(attackingPieces)/(sizeof(PIECE *))) //none of QRB can capture attackers
        {
            capCheck = 1; //check flag up 
        }
        else {
            capCheck = 0; //some piece can capture still ---> check flag down
        }
        */
        for(i = 0; i < 16; i++){
            for(j = 0; j < 16; j++) {
                if(attackingPieces[j] != NULL) {
                    if(ourPieces[i] != NULL) {
                        if(ourPieces[i]->type != 'K'){
                            if(isValidPieceMove(ourPieces[i], attackingPieces[j]->row, attackingPieces[j]->col)){
                                capCheck = 0;
                            }
                        }
                    }
                }
            }
        }
        //BLOCKING
        //now determine if our team piece can block the attacker
        for(i=0; i<sizeof(attackingPieces)/(sizeof(PIECE *)); i++){
            //knight and pawn can't be blocked so blockCheck returns 1
            if (attackingPieces[i] != NULL) {
                if(attackingPieces[i]->type == 'N' || attackingPieces[i]->type == 'P'){
                    blockCheckmate = 1;
                }

                if(attackingPieces[i]->type == 'R' || (attackingPieces[i]->type == 'Q' && (attackingPieces[i]->col == KingCol || attackingPieces[i]->row == KingRow))) {
                    if((attackingPieces[i]->col) > KingCol){ //checking squares in between when column of rook is greater
                        for(j = KingCol; j<attackingPieces[i]->col; j++) { 
                            blockCheckmate = blockCheckmate & canBlock(teamBias, KingRow, j); //using bitwise AND operator. Since blockCheck is initialized as 1, if it can be blocked in any of the squares (canBlock = 0), it'll pass as 0.

                        }
                    }
                    if((attackingPieces[i]->col) < KingCol){ //checking squares in between when column of rook is less
                        for(j = attackingPieces[i]->col; j < KingCol; j++) { 
                            blockCheckmate = blockCheckmate & canBlock(teamBias, KingRow, j);

                        }
                    }
                    if((attackingPieces[i]->row) > KingRow){ //checking squares in between when row of rook is greater
                        for(j = KingRow; j<attackingPieces[i]->row; j++){ 
                            blockCheckmate = blockCheckmate & canBlock(teamBias, j, KingCol);

                        }
                    }
                    if(attackingPieces[i]->row < KingRow){ //checking squares in between when row of rook is less
                        for(j = attackingPieces[i]->row; j< KingRow; j++) { 
                            blockCheckmate = blockCheckmate & canBlock(teamBias, j, KingCol);

                        }
                    }
                } //end of line check

                if(attackingPieces[i]->type == 'B' || (attackingPieces[i]->type == 'Q' && (attackingPieces[i]->row != KingRow && attackingPieces[i]->col != KingCol))){
                    k = KingRow;
                    if(attackingPieces[i]->col > KingCol && attackingPieces[i]->row > KingRow){ //checking squares in between when col and row of bish is greater
                        for(j = KingCol; j<attackingPieces[i]->col; j++) { 
                            blockCheckmate = blockCheckmate & canBlock(teamBias, k, j); //using bitwise AND operator. Since blockCheck is initialized as 1, if it can be blocked in any of the squares (canBlock = 0), it'll pass as 0.
                            k++;
                        }
                    }
                    if(attackingPieces[i]->col > KingCol && attackingPieces[i]->row < KingRow){ //checking squares in between when col of bish is greater and row is less
                        for(j = KingCol; j<attackingPieces[i]->col; j++) { 
                            blockCheckmate = blockCheckmate & canBlock(teamBias, k, j);
                            k--;
                        }
                    }
                    if(attackingPieces[i]->col < KingCol && attackingPieces[i]->row > KingRow){ //checking squares in between when col of bish is less and row is greater
                        for(j = KingCol; j<attackingPieces[i]->col; j--) { 
                            
                            blockCheckmate = blockCheckmate & canBlock(teamBias, k, j);
                            k++;

                        }
                    }
                    if(attackingPieces[i]->col < KingCol && attackingPieces[i]->row < KingRow){ //checking squares in between when col and row of bish is less
                        for(j = KingCol; j<attackingPieces[i]->col; j--) { 
                            blockCheckmate = blockCheckmate & canBlock(teamBias, k, j);
                            k--;
                        }
                    }
                } // end of diagonal check
            }
            
        } /* rof */

    }/* fi */ 
    if((capCheck == 1) && (blockCheckmate == 1)){
        finalCheck = 1;
    }
    else{
        finalCheck = 0;
    }
    return finalCheck;
} /*end of block check */

//canBlock will determine whether or not a check can be blocked by defender's pieces 
// returns 0 if check can be blocked and king no longer in check
// returns 1 if check cannot be blocked and king still in check 
int canBlock(char teamBias, int row, int col){ 
    PIECE * ourPieces[16];
    int maxIndex = 0;
    int i;
    

    //storing all of our pieces into array
    for(int r = 0; r < 8; r++){
        for(int c = 0; c < 8; c++){
            if(getPieceAt(r,c) != NULL){
            
                if(getPieceAt(r,c)->team == teamBias){
                    ourPieces[maxIndex] = getPieceAt(r,c);
                    maxIndex++;   
                }
                
            }
        }
    }

    if (maxIndex < 16) {
        for (int a = maxIndex; a < 16; a++) {
            ourPieces[a] = NULL;
        }
    }
//iterating through our pieces to determine if it's a valid move to go to square. If it is return 0;
    for(i = 0; i < 16; i++){
        if(ourPieces[i] != NULL) {
            if(ourPieces[i]->type != 'K'){
                if(isValidPieceMove(ourPieces[i], row, col)){
                    return 0;
                }
            }
        }
    }

    return 1;


}

void getEnemies(char teamBias){
    int maxIndex = 0;

    for(int r = 0; r < 8; r++){
            for(int c = 0; c < 8; c++){
                if(getPieceAt(r,c) != NULL){
                    if(getPieceAt(r,c)->team != teamBias){
                        enemyPieces[maxIndex] = getPieceAt(r,c);
                        maxIndex++;   
                    }
                }
            }
        }
        if (maxIndex < 16) {
            for (int a = maxIndex; a < 16; a++) {                   //setting the rest of the pieces to NULL
            enemyPieces[a] = NULL;
            }
        }
}