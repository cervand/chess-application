/*
4/28/20 Andres
    - added new BOARD_DATA functions

4/28 + 4/29 Bradley 
    - added CustomBoardSetup() and fixed bugs/seg faults with the boardData stuff
*/

/* Board.c */
#include "Square.h"
#include "Piece.h"
#include "Board.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//test comment for commit part 2

//structure acts like a type for the array
int row, col, success;
BOARD_DATA * boardData = NULL;



/*for loop to initialize starting board*/
void CreateBoard() {
    for (row=0;row<8;row++){
        for(col=0;col<8;col++){
            SQUARE * tempsquare = CreateSquare();       //creating square
            tempsquare->x = row;
            tempsquare->y = col;                        //setting coordinates of square
            tempsquare->piece_ptr = NULL;               //no pieces being set yet
            boardArray[row][col] = tempsquare;
        }/*ROF*/
    }/*ROF*/
    boardData = CreateBoardData();
}

int DeleteBoard() {         //goes through each square on the board and deletes the pieces if they exist
//    int pieces = 0;
//    int squares = 0;
    for (row = 0; row < 8; row++) {
        for (col = 0; col < 8; col++) {
            SQUARE * temp = getSquareAt(row, col);
            if (temp->piece_ptr != NULL) {
                DeletePiece(temp->piece_ptr);
//                pieces++;
            }
            if (temp != NULL) {
                DeleteSquare(temp);
//                squares++;
            }
        }
    }
//    printf("\nPieces Freed: %d\nSquares Freed: %d", pieces, squares);
    boardData->previouslyBlackMovedPiecePtr = NULL;
    boardData->previouslyWhiteMovedPiecePtr = NULL;
    DeleteBoardData();
    return 0;
}


//now setting pieces
void DefaultPieceSetup() {
    int counter = 0;
    for(row=0;row<2;row++){ //for 1st 2 rows 
        for(col=0;col<8;col++){
            PIECE * piece = CreatePiece();
            if (row == 0) {
                if ((counter == 0) || (counter == 7)) {		//setting rooks
                    piece->type = 'R';
                } else if ((counter == 1) || (counter == 6)) {	//setting knights
                    piece->type = 'N';
                } else if ((counter == 2) || (counter == 5)) {  //setting bishops
                    piece->type = 'B';
                } else if (counter == 3) {			//setting king
                    piece->type = 'Q';
                } else if (counter == 4) {			//setting queen
                    piece->type = 'K';
                }
            } else if (row == 1) {				//setting pawns
                piece->type = 'P';
            }
            counter++;
            piece->team = 'B';
            setPieceAt(piece, row, col); //will point to a piece struct
        }/*ROF*/
    }/*ROF*/
    counter = 0;
    for(row=6;row<8;row++){ //for last 2 rows 
        for(col=0;col<8;col++){
            PIECE * piece = CreatePiece();\
            if (row == 6) {
                piece->type = 'P';				//setting pawns
            } else if (row == 7) {
                    if ((counter == 0) || (counter == 7)) {         //setting rooks
                            piece->type = 'R';
                    } else if ((counter == 1) || (counter == 6)) {  //setting knights
                            piece->type = 'N';
                    } else if ((counter == 2) || (counter == 5)) {  //setting bishops
                            piece->type = 'B';
                    } else if (counter == 3) {                      //setting king
                            piece->type = 'Q';
                    } else if (counter == 4) {                      //setting queen
                            piece->type = 'K';
                    }
                    counter++;
            }
	        piece->team = 'W';
            setPieceAt(piece, row, col); //will point to a piece struct
        }/*ROF*/
    }/*ROF*/
}

void CustomPieceSetup() {
    char pieceinput[2], locationinput[2], teambuffer, input;
    char pieces[6] = {'P', 'R', 'B', 'N', 'K', 'Q'};
    int row, col, i, validpiece;
    int whitepieceplaced = 0;
    int blackpieceplaced = 0;
    int whitekingplaced = 0;
    int blackkingplaced = 0;
    printf("\nWelcome to the custom board setup, here you can set any piece to be anywhere on the board.");
    printf("\nTo select a piece, type the piece's name into the prompt (ex. bP for a black pawn).");
    printf("\nAfter selecting a piece, you will then place it by entering the coordinates of the desired location (a-h, 1-8) Some example inputs: a2, b7, c6)");
    printf("\nTo de-select a piece enter 00 in the location prompt.");
    printf("\nTo finish and exit the custom game board enter XX in the piece prompt\n");
    printf("\n---NOTE: Due to how pawns can move in one direction, white pawns can only move upwards and black pawns can only move downwards");
    printf("\n---NOTE 2: You must also have the white king and black king on the board, you can't add more than one of each team king to the board\n");
    while (1) {                                                         //outer loop
        pieceinput:while (1) {                                          //piece loop
            PrintBoard(0);
            printf("\nPieces: bP  bR  bN  bB  bK  bQ   |   wP  wR  wN  wB  wK  wQ\n");
            printf("\nSelect a piece (Enter XX to exit custom setup): ");
            scanf(" %2s", pieceinput);
            if (strncmp(pieceinput, "XX", 2) == 0) {                    //exiting custom board setup    
                if ((whitepieceplaced == 1) && (blackpieceplaced == 1)) {
                    if (whitekingplaced == 0) {
                        printf("\nYou didn't place the white king! Please place him down, he wants to play too!");
                        continue;
                    } else if (blackkingplaced == 0) {
                        printf("\nYou didn't place the black king! Please place him down, he wants to play too!");
                        continue;
                    } else {
                        return;
                    }
                } else if ((whitepieceplaced == 0) && (blackpieceplaced == 0)) {
                    printf("\nYou didn't place any pieces! Do you want to play classic or continue custom board setup? (C for classic, S to stay in custom board setup)");
                    scanf(" %c", &input);
                    while (1) {
                        if (input == 'C') {                                             //if user wants to go back to classic
                            DefaultPieceSetup();
                            return;
                        } else if (input == 'S') {                                      //if user wants to stay awhile and listen and continue custom setup
                            goto pieceinput;
                        } else {
                            continue;
                        }
                    }
                } else if ((whitepieceplaced == 0)) {
                    printf("\nYou didn't place a white piece!");
                    continue;
                } else if ((blackpieceplaced == 0)) {
                    printf("\nYou didn't place a black piece!");
                    continue;
                }
            }

            if (pieceinput[0] == 'b') {                                 //checking first letter to see if it's a valid team
                teambuffer = 'B';
            } else if (pieceinput[0] == 'w') {
                teambuffer = 'W';
            } else {
                printf("\nInvalid piece input!");
                continue;
            } 
            for (i = 0; i < 6; i++) {
                if (pieceinput[1] == pieces[i]) {
                    validpiece = 1;                         //if the second letter is one of the 6 pieces then it's valid
                    break;
                } else {
                    validpiece = 0;                         //if it isn't then invalid
                }
            }  
            if (validpiece == 0) {
                printf("\nInvalid piece input!");
                continue;
            }
            break;                                          //piece input is valid so now can break out of loop
        }
        PIECE * piece = CreatePiece();
        piece->team = teambuffer;
        piece->type = pieceinput[1];                        //setting the piece's team and type to put on the board
        while (1) {                                         //location loop
            printf("\nNow please enter the location (Enter 00 to de-select the current piece):");
            scanf(" %2s", locationinput);
            if (strncmp(locationinput, "00", 2) == 0) {           
                goto pieceinput;                            //goto used here to jump to other while loop for piece selection
            } 
            row = locationinput[0] - 97;
            col = 56 - locationinput[1];
            if ((row < 0) || (row > 7) || (col < 0) || (col > 7)) {             //if location is out of bounds
 			    printf("\nOut of bounds! ");
			    continue;               
            } else {
                setPieceAt(piece, col, row);                //setting piece at valid location
                piece->row = col;
                piece->col = row;
                if (piece->team == 'W') {
                    whitepieceplaced = 1;                   //white piece placed
                    if(piece->type == 'K') {
                        whitekingplaced = 1;                //white king placed
                    }
                } else if (piece->team =='B') {             
                    blackpieceplaced = 1;                   //black piece placed
                    if (piece->type == 'K') {
                        blackkingplaced = 1;                //black king placed
                    }
                }
            }
            break;                                      //location is set now goes back to outer while loop to continue again
        }
    }
}

/*printing out the board*/
char * PrintGTKBoard()
{   
    //char buffer[1500];
    char number = 8;
    char numMinRow[10];
    int strNum = 0;

    //NOTE REMOVED "\N" FROM THE LINE BELOW THIS ONE, WAS PLACED AS THE VERY FIRST THING
    strcpy(chessText, "\n           a       b    c      d      e      f       g      h     \n"); 
    for (row = 0; row < 8; row++) {
        strcat(chessText, "      +----+----+----+----+---+----+----+----+\n");
            strNum = number-row;
            sprintf(numMinRow, "%d", strNum);
        strcat(chessText, numMinRow);
        strcat(chessText, "     "); 
        for (col = 0; col < 8; col++) {
            if (boardArray[row][col]->piece_ptr) {                  //if there's a piece then prints it to the board
                PrintPiece(boardArray[row][col]->piece_ptr, chessText);               
            } else {                                                //if there's no piece then prints out a blank space
                strcat(chessText,"|       "); //added 3 spaces
            }
        }

        strcpy(numMinRow, "\0");
        strNum = number-row;
        sprintf(numMinRow, "%d", strNum);
        strcat(chessText, "|    ");
        strcat(chessText, numMinRow);
        strcat(chessText, "\n");
    }
    strcat(chessText,"      +----+----+----+----+---+----+----+----+\n"); 
    strcat(chessText,"           a       b      c      d      e      f       g      h\n"); 

    return chessText;

}

/*printing out the board*/
void PrintBoard(int WriteToFile)
{   
    char number = 8;
    printf("\n         a     b     c     d     e     f     g     h     \n"); 
    if (WriteToFile == 1) {                                                         //for PrintBoard and PrintPiece, if WriteToFile == 1 then it writes to Gamelog.txt
        fprintf(fp,"\n         a     b     c     d     e     f     g     h     \n");
    }
    for (row = 0; row < 8; row++) {
        printf("      +-----+-----+-----+-----+-----+-----+-----+-----+\n");
        if (WriteToFile == 1) {
            fprintf(fp,"      +-----+-----+-----+-----+-----+-----+-----+-----+\n");            
        }
        printf("%d     ", number - row);
        if (WriteToFile == 1) {
            fprintf(fp,"%d     ", number - row);            
        }   
        for (col = 0; col < 8; col++) {
            if (boardArray[row][col]->piece_ptr) {                  //if there's a piece then prints it to the board
                //PrintPiece(boardArray[row][col]->piece_ptr, WriteToFile);               
            } else {                                                //if there's no piece then prints out a blank space
                printf("|     ");
                if (WriteToFile == 1) {
                    fprintf(fp,"|     ");
                }
            }
        }
        printf("|    %d\n", number - row);
        if (WriteToFile == 1) {
            fprintf(fp,"|    %d\n", number - row);
        }
    }
    printf("      +-----+-----+-----+-----+-----+-----+-----+-----+\n"); 
    if (WriteToFile == 1){
        fprintf(fp,"      +-----+-----+-----+-----+-----+-----+-----+-----+\n");
    }
    printf("         a     b     c     d     e     f     g     h     \n"); 
    if (WriteToFile == 1) {
        fprintf(fp,"         a     b     c     d     e     f     g     h     \n"); 
    } 
}

void PrintPiece(PIECE * piece, char * chessText) {        //prints out the piece according to the type
    switch (piece->type) {
        case 'P':
            if(piece->team == 'B') {      //if black team then print out black symbol
                strcat(chessText,"|");
                //strcat(buffer,"\033[0;31m");       //setting text color to red to print black team as red
                strcat(chessText,"  ♟︎  ");
                //strcat(buffer,"\033[0m");          //resetting to white
            } else {                            //else prints out white symbol
                strcat(chessText,"|  ♙  ");
            }   
            break;
        case 'R':
            if(piece->team == 'B') {
                strcat(chessText,"|");
                //strcat(buffer,"\033[0;31m");
                strcat(chessText,"  ♜  ");
                //strcat(buffer,"\033[0m");
            } else {
                strcat(chessText,"|  ♖  ");
            }   
            break;
        case 'N':
            if(piece->team == 'B') {
                strcat(chessText,"|");
                //strcat(buffer,"\033[0;31m");
                strcat(chessText,"  ♞  ");
                //strcat(buffer,"\033[0m");
            } else {
                strcat(chessText,"|  ♘  ");
            }   
            break;
        case 'B':
            if(piece->team == 'B') {
                strcat(chessText,"|");
                //strcat(buffer,"\033[0;31m");
                strcat(chessText,"  ♝  ");
                //strcat(buffer,"\033[0m");
            } else {
                strcat(chessText,"|  ♗  ");
            }   
            break;
        case 'Q':
            if(piece->team == 'B') {
                strcat(chessText,"|");
                //strcat(buffer,"\033[0;31m");
                strcat(chessText,"  ♛  ");
                //strcat(buffer,"\033[0m");
            } else {
                strcat(chessText,"|  ♕  ");
            }   
            break;
        case 'K':
            if(piece->team == 'B') {
                strcat(chessText,"|");
                //strcat(buffer,"\033[0;31m");
                strcat(chessText,"  ♚  ");
                //strcat(buffer,"\033[0m");
            } else {
                strcat(chessText,"|  ♔  ");
            }   
            break;
            
    }
    //strcat(buffer,"\033[0m");
}

/* Removes/Deletes a piece at a specified pos. in boardArray[][] */
void removePieceAt(int row, int col){
    PIECE *tempPiece = boardArray[row][col]->piece_ptr;
    DeletePiece(tempPiece);
    boardArray[row][col]->piece_ptr = NULL;
}

/* Setter for boardArray[][] */
void setPieceAt(PIECE * p, int row, int col){
    boardArray[row][col]->piece_ptr = p;
    p->row = row;
    p->col = col;
}

/* getter for boardArray[][] */
PIECE * getPieceAt(int row, int col){
    //If out of bounds, return 1 (using for check)
    if(row<0 || col<0 || row>7 || col>7){
        return NULL;
    }
    if (boardArray[row][col]->piece_ptr != NULL) {
        return boardArray[row][col]->piece_ptr;
    } else {
        return NULL;
    }
}

/* gets SQUARE from boardArray[row][col] */
SQUARE * getSquareAt(int row, int col) {
    return boardArray[row][col];
}

BOARD_DATA * CreateBoardData() {
    BOARD_DATA * temp = malloc(sizeof(BOARD_DATA));
    temp->previouslyWhiteMovedPiecePtr = NULL;
    temp->previouslyBlackMovedPiecePtr = NULL;
    temp->netMoves = 0;
    return temp;
}

int DeleteBoardData() {
    if (boardData) {
        deletePreviouslyMovedBlackPiece();
        deletePreviouslyMovedWhitePiece();
        free(boardData);
        boardData = NULL;     
        return 0;  
    } else {
        return 1;
    }

}
//get boardData
BOARD_DATA * getBoardData(){
    return boardData;
}

//updates board data, like the piece struct function
// I made this in case we add more stuff to this struct so we dont have
// to type what is inside over and over again
// simply sets the new piece ptr and increments global move count
void updateBoardData(PIECE * p){
    if(p->team == 'W'){
        setPreviouslyMovedWhitePiece(p);
    }
    if(p->team == 'B'){
        setPreviouslyMovedBlackPiece(p);
    }
    boardData->netMoves = boardData->netMoves+1;
}

//returns the previouslyMovedPiece which is stored in BOARD_DATA, used for en passant 
PIECE * getPreviouslyMovedWhitePiece(){
    return boardData->previouslyWhiteMovedPiecePtr;
}

//returns the previouslyMovedPiece which is stored in BOARD_DATA, used for en passant 
PIECE * getPreviouslyMovedBlackPiece(){
    return boardData->previouslyBlackMovedPiecePtr;
}

//sets a new previouslyMovedPiece in BOARD_DATA, used in updateBoardData
void setPreviouslyMovedWhitePiece(PIECE * pieceMoved){
    boardData->previouslyWhiteMovedPiecePtr = pieceMoved;
}

//sets a new previouslyMovedPiece in BOARD_DATA, used in updateBoardData
void setPreviouslyMovedBlackPiece(PIECE * pieceMoved){
    boardData->previouslyBlackMovedPiecePtr = pieceMoved;
}

void deletePreviouslyMovedWhitePiece(){
    if (boardData->previouslyWhiteMovedPiecePtr) {
        DeletePiece(boardData->previouslyWhiteMovedPiecePtr);
    }
}

void deletePreviouslyMovedBlackPiece(){
    if (boardData->previouslyBlackMovedPiecePtr) {
        DeletePiece(boardData->previouslyBlackMovedPiecePtr);
    }
}




