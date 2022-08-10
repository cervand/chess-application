/* Board.h Header File for Board.c */

/*
4/20 Andres
    - Removed argument "SQUARE *boardArray" in PrintBoard() - not used in Board.c file or Chess. 
    - Added functions addPieceAt, and removePieceAt

4/28 Andres
    - Added BOARD_DATA struct
    
4/29 Bradley 
    - Fixed many bugs/seg faults with the new boardData struct 
    - added CreateBoardData function to allocate memory for it
*/
#include "stdio.h"
#include "Square.h"
#include "Piece.h"
#include "Chess.h"

#ifndef BOARD_H
#define BOARD_H

//Holds previous move data
typedef struct {
    PIECE * previouslyWhiteMovedPiecePtr;
    PIECE * previouslyBlackMovedPiecePtr;
    int netMoves;
}BOARD_DATA;

SQUARE * boardArray[8][8];      //8x8 board of square pointers

char chessText[1500];

FILE* fp;

char * PrintGTKBoard();

void CreateBoard();

int DeleteBoard();

void DefaultPieceSetup();

void CustomPieceSetup();

void PrintBoard(int WriteToFile);

void PrintPiece(PIECE * piece, char * chessText);

PIECE * getPieceAt(int row, int col);

void setPieceAt(PIECE * p, int row, int col);

void removePieceAt(int row, int col);

SQUARE * getSquareAt(int row, int col);

BOARD_DATA * CreateBoardData();

int DeleteBoardData();

BOARD_DATA * getBoardData();

void updateBoardData(PIECE * p);

PIECE * getPreviouslyMovedWhitePiece();

PIECE * getPreviouslyMovedBlackPiece();

void setPreviouslyMovedWhitePiece(PIECE * pieceMoved);

void setPreviouslyMovedBlackPiece(PIECE * pieceMoved);

void deletePreviouslyMovedWhitePiece();

void deletePreviouslyMovedBlackPiece();

#endif
