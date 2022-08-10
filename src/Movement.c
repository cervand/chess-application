/* Capture & movement module 
April 23, 3:22AM Andres: 
    -   added bishop, rook, knight, and queen checkers.
    -   added the official amazing brand new function that covers movement: movePieceTo() 
   
April 24, 2:39AM Trenton: 
    -   Removed all of the reversefactors besides the pawns. 
    -   Added removePieceAt(toRow, toCol); to represent capture in changePieceTo function.
    -   Added capture logic for all pieces. 
    -   Changed some of the bishop logics, in order to prevent it from getting locked down by adjacent pieces (hard to explain without showing)
    -   Made it so rook cannot hop over units.
    -   King and knight untouched, should be ok??

April 24, 1:39PM Trenton: 
    -   Made friendly fire off. 
    -   Cleaned rook code.

April 27, 8:29PM Trenton: 
    -   Implemented en passant, move counter (whoops lol) 
    -   Made pawns unable to jump over units.

April 28, 9:42PM Andres: 
    -   Merged changePieceTo and movePieceTo, completed enPassant 

April 29, 11:27 A.M Bradley: 
    -   Tried to optimize movePieceTo, removed need to create a duplicate piece

April 29 12:47AM Andres: (should we stop the times lol?)
    - Modified reverse factor to match with current board setup
    - Modified en passant
    - Added isAttacked(PIECE* p), checks if a piece is being attacked, synonymous with check?
    - Added Castling abilities,  isValidCastling
    - Modified isValidKnight
    - Added return "codes" 0 = invalid move, 1 = valid move/capture, 2 = en passant valid, 3 = castling valid
    - Modified movePieceTo to work with the codes mentioned in the above line.

April 30 12:58AM Taylor:
    - Modified isAttacked to also create array of pieces that are attacking the king specifically 

May 3, 1:06AM Taylor:
    - do we need to modify so that spots in check are invalid moves or print out a message that tells user in check? (good for testing too)
*/
#include "Square.h"
#include "Board.h"
#include "Piece.h"
#include "Movement.h"
#include "Check.h"
#include "Chess.h"
#include <stdio.h>
#include <stdlib.h>

/* Returns 1 if a move was made
 * Returns 0 if a move was NOT made
 * This function simply moves a piece, given that it is a valid move
 */
int movePieceTo(PIECE *p, int fromRow, int fromCol, int toRow, int toCol)
{
    int captured;
    if (isAttacked(p->team, getKingRow(p->team), getKingCol(p->team)))
    {
        if (isStillCheck(p, toRow, toCol, p->team))
        {
            return 0;
        }
    }
    switch (isValidPieceMove(p, toRow, toCol))
    {
        case 1: //regular move/capture

            //promotion logic
            if (p->type == 'P')
            {
                if (toRow == 7 || toRow == 0)
                {
                    p->type = 'Q';
                    if (p->team == firstteam)
                    {
                        firstteampawnpromotion = 1;
                    }
                    else
                    {
                        secondteampawnpromotion = 1;
                    }
                }
            }
            captured = DeletePiece(getPieceAt(toRow, toCol));
            if (captured == 0)
            {
                if (p->team == firstteam)
                {
                    firstteammove = 1;
                }
                else
                {
                    secondteammove = 1;
                }
            }
            setPieceAt(p, toRow, toCol);
            boardArray[fromRow][fromCol]->piece_ptr = NULL;
            updatePiece(p, fromRow, fromCol, toRow, toCol);
            updateBoardData(p);
            return 1;
            break;

        case 2: //en passant
            //setPieceAt(NULL, fromRow, fromCol);
            //            printf("this is an en passant!!!!!**********************");
            captured = DeletePiece(getPieceAt(fromRow, toCol)); //deletes adjacent piece
            if (captured == 0)
            {
                if (p->team == firstteam)
                {
                    firstteammove = 2;
                }
                else
                {
                    secondteammove = 2;
                }
            }
            setPieceAt(p, toRow, toCol); //set the piece to the new location
            //setPieceAt(NULL, fromRow, toCol);
            boardArray[fromRow][fromCol]->piece_ptr = NULL; //deletes old ptr to piece being moved
            boardArray[fromRow][toCol]->piece_ptr = NULL;   //deletes old ptr to adjacent piece
            setPieceAt(p, toRow, toCol);                    //set the pieceptr to new location
            updatePiece(p, fromRow, fromCol, toRow, toCol);
            updateBoardData(p);
            return 1;
            break;

        case 3: //castling (remember toRow == fromRow)
            if (fromCol > toCol)
            {                                                                                      //left side of board
                setPieceAt(getPieceAt(fromRow, toCol - 2), toRow, toCol + 1);                      //sets the rook to its new position, rightside of king
                updatePiece(getPieceAt(fromRow, toCol - 2), toRow, toCol - 2, fromRow, toCol + 1); //updates rook struct to match new position
                boardArray[toRow][toCol - 2]->piece_ptr = NULL;                                    //sets piece_ptr of past rook location to NULL
            }
            else if (fromCol < toCol)
            {                                                                                        //right side of board
                setPieceAt(getPieceAt(fromRow, toCol + 1), toRow, toCol - 1);                        //sets the rook to its new position
                updatePiece(getPieceAt(fromRow, toCol + 1), fromRow, toCol + 1, fromRow, toCol - 1); //updates rook struct to match new position (the corner is still point )
                boardArray[toRow][toCol + 1]->piece_ptr = NULL;                                      //sets piece_ptr of past rook location to NULL
            }
            setPieceAt(p, toRow, toCol);                    //sets the king to the new "castled" position
            boardArray[fromRow][fromCol]->piece_ptr = NULL; //removes piece_ptr pointing to the king at its old position
            updatePiece(p, fromRow, fromCol, toRow, toCol);
            updateBoardData(p);
            if (p->team == firstteam)
            {
                if (fromCol > toCol)
                { //queenside castling
                    firstteammove = 32;
                }
                else
                {
                    firstteammove = 31;
                }
            }
            else
            {
                if (fromCol > toCol)
                { //queenside castling
                    secondteammove = 32;
                }
                else
                {
                    secondteammove = 31;
                }
            }
            return 1;
            break;

        default:
            return 0;
            break;
    } //end swithc
} //end movePieceTo

/* return row posittion of the teamBias King
 * 
 */
int getKingRow(char teamBias)
{
    //Getting the teamBias king's position
    int row = 0;
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            if (getPieceAt(r, c) != NULL)
            {
                //printf("\nThere is some type of piece at row %d and col %d", r, c);
                if (getPieceAt(r, c)->team == teamBias)
                {
                    //printf("\nThe piece at R:%d and C:%d is in team %c", r, c, teamBias);
                    if (getPieceAt(r, c)->type == 'K')
                    {
                        row = r;
                        //printf("\nIssa king of team %c. This is the king we want to protect\n\n", teamBias);
                        //return r;
                    }
                }
            }
        }
    } //end for - getting King position
    return row;
}

/* return column posittion of the teamBias King
 * 
 */
int getKingCol(char teamBias)
{
    int col = 0;
    //Getting the teamBias king's position
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            if (getPieceAt(r, c) != NULL)
            {
                //printf("\nThere is some type of piece at row %d and col %d", r, c);
                if (getPieceAt(r, c)->team == teamBias)
                {
                    //printf("\nThe piece at R:%d and C:%d is in team %d", r, c, teamBias);
                    if (getPieceAt(r, c)->type == 'K')
                    {
                        col = c;
                        //printf("\nIssa king of team %d. This is the king we want to protect\n\n", teamBias);
                        //return c;
                    }
                }
            }
        }
    } //end for - getting King position
    return col;
}

/*  Checks if the proposed move will result in self-check
 *  Also checks that player is making a move to get them OUT of check (synonymous with above line)
 * If the proposed move still has teamBias in check, then isStillCheck will return 1;
 * If the proposed move does NOT lead to teamBias in check, then isStillCheck will return 0;
 */
int isStillCheck(PIECE *p, int tRow, int tCol, char teamBias)
{
    //our team's row and column
    int fRow = p->row;
    int fCol = p->col;

    //simulating the move to check if King would still be in check after it
    //Step 1: Is the move even valid? Considers en passant and castling!
    char pieceSymbol = p->type;
    int validity = 0;
    switch (pieceSymbol)
    {
    case 'P':
        validity = isValidPawnMove(p, tRow, tCol);
        break;

    case 'R':
        validity = isValidRookMove(p, tRow, tCol);
        break;

    case 'B':
        validity = isValidBishopMove(p, tRow, tCol);
        break;

    case 'N':
        validity = isValidKnightMove(p, tRow, tCol);
        break;

    case 'K':
        validity = isValidKingMove(p, tRow, tCol);
        break;

    case 'Q':
        validity = isValidQueenMove(p, tRow, tCol);
        break;

    default:
        validity = 0;
        break;
    } //end switch

    int isStillInCheck = 0;

    PIECE *pieceAtDesLoc = NULL;
    if (getPieceAt(tRow, tCol) != NULL)
    {
        pieceAtDesLoc = getPieceAt(tRow, tCol);
    }

    PIECE *pieceCapt = NULL;
    if (getPieceAt(fRow, tCol) != NULL)
    {
        pieceCapt = getPieceAt(fRow, tCol);
    }

    switch (validity)
    {
    case 0:
        return 1;
        break;
    case 1: //regular move/capture
        //moving the piece there
        boardArray[tRow][tCol]->piece_ptr = NULL;
        setPieceAt(p, tRow, tCol);
        boardArray[fRow][fCol]->piece_ptr = NULL;

        //now that the piece is there, is the King still attacked? Lets save that response since we have to put back everything else
        if (isAttacked(teamBias, getKingRow(teamBias), getKingCol(teamBias)))
        {
            isStillInCheck = 1;
        }

        //putting pieces back to the way they were
        boardArray[fRow][fCol]->piece_ptr = NULL;
        setPieceAt(p, fRow, fCol);
        boardArray[tRow][tCol]->piece_ptr = NULL;
        if (pieceAtDesLoc != NULL)
        {
            setPieceAt(pieceAtDesLoc, tRow, tCol);
        }
        break;

    case 2: //en passant

        boardArray[fRow][tCol]->piece_ptr = NULL; //piece being captured
        boardArray[tRow][tCol]->piece_ptr = NULL;
        setPieceAt(p, tRow, tCol);
        boardArray[fRow][fCol]->piece_ptr = NULL;

        //now that the piece is there, is the King still attacked? Lets save that response since we have to put back everything else
        if (isAttacked(teamBias, getKingRow(teamBias), getKingCol(teamBias)))
        {
            isStillInCheck = 1;
        }

        boardArray[tRow][tCol]->piece_ptr = NULL;
        boardArray[fRow][tCol]->piece_ptr = NULL;
        if (pieceCapt != NULL)
        {
            setPieceAt(pieceCapt, fRow, tCol);
        }
        //        else
        //       {
        //           printf("*************something is wrong with en passsant*****************");
        //        }
        boardArray[fRow][fCol]->piece_ptr = NULL;
        setPieceAt(p, fRow, fCol);
        break;

    case 3: //you cannot castle out of check, so castling is not gonna work buddy
        return 1;
        break;
    }
    //printf("********the value of isStillInCheck is %d", isStillInCheck);
    return isStillInCheck;
} //end func isStillCheck

/* Checks for Checkmate
 * If in checkmate, returns 1. If not in checkmate, returns 0;
 */
int isCheckmate(char teamBias)
{
    int kRow = getKingRow(teamBias);
    int kCol = getKingCol(teamBias);
    PIECE *ourPieces[16];
    int maxIndex = 0;

    //flags used to determine checkmate
    int isSurroundedAndAttacked = 0;
    int posMoves = 0;
    int movesDoNotBlockCheck = 0;

    //----- Here we are checking if the pieces surrounding us are being attacked/taken
    //top row check
    if (isAttacked(teamBias, kRow + 1, kCol - 1) && isAttacked(teamBias, kRow + 1, kCol) && isAttacked(teamBias, kRow, kCol + 1))
    {
        //bottom row check
        if (isAttacked(teamBias, kRow - 1, kCol - 1) && isAttacked(teamBias, kRow - 1, kCol) && isAttacked(teamBias, kRow - 1, kCol + 1))
        {
            //checking pieces at our side.
            if (isAttacked(teamBias, kRow, kCol - 1) && isAttacked(teamBias, kRow, kCol + 1))
            {
                //is our king in check?
                if (isAttacked(teamBias, kRow, kCol))
                {
                    isSurroundedAndAttacked = 1;
                }
            }
        }
    }

    //Now we are creating an array of our pieces
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            if (getPieceAt(r, c) != NULL)
            {
                if (getPieceAt(r, c)->team == teamBias)
                {
                    ourPieces[maxIndex] = getPieceAt(r, c);
                    maxIndex++;
                }
            }
        }
    }

    //Now we are getting the number of ALL the possible moves
    for (int i = 0; i < maxIndex; i++)
    {
        for (int r = 0; r < 8; r++)
        {
            for (int c = 0; c < 8; c++)
            {
                if (isValidPieceMove(ourPieces[i], r, c))
                {
                    posMoves += 1;
                }
            }
        }
    }

    //Now we are checking to see how many moves will NOT block check
    for (int i = 0; i < maxIndex; i++)
    {
        for (int r = 0; r < 8; r++)
        {
            for (int c = 0; c < 8; c++)
            {
                if (isStillCheck(ourPieces[i], r, c, teamBias))
                {
                    movesDoNotBlockCheck += 1;
                }
            }
        }
    }

    //now we have everything we need to determine checkmate, time to see if it is or not
    // why posMoves = moves..blahblah? no move will block checkmate, so posMoves must be equal to the amount of moves that DONT block checkmate/check
    if (isSurroundedAndAttacked && (posMoves == movesDoNotBlockCheck))
    {
        return 1;
    }
    else
    {
        return 0;
    }
} //edn func isCheckmate

/* Directs the inputs to a relevant check to prevent unneccesary checking of piece movements
 * Returns 1 if valid move, 0 if not 
 */
int isValidPieceMove(PIECE *p, int toRow, int toCol)
{
    /*
    if(isStillCheck(p, toRow, toCol, p->team)){
        return 0;
    }
    */

    if (toRow > 7 || toCol > 7 || toRow < 0 || toCol < 0)
    {
        return 0; // out of bounds.
    }

    switch (p->type)
    {
    //Pawn
    case 'P':
        return isValidPawnMove(p, toRow, toCol);
        break;

    case 'R':
        return isValidRookMove(p, toRow, toCol);
        break;

    case 'B':
        return isValidBishopMove(p, toRow, toCol);
        break;

    case 'N':
        return isValidKnightMove(p, toRow, toCol); //seg
        break;

    case 'K':
        return isValidKingMove(p, toRow, toCol);
        break;

    case 'Q':
        return isValidQueenMove(p, toRow, toCol);
        break;

    default:
        return 0;
        break;
    } //end switch
} //end isValidPieceMove

/* Checks that a valid pawn move is attempted
 * Returns 1 if valid, 0 if not 
 */
int isValidPawnMove(PIECE *p, int desRow, int desCol)
{
    int curRow = p->row;
    int curCol = p->col; //NOTE: compiler says that this variable is unused
    int reverseFactor = 1;

    if (p->team == 'W')
    {
        reverseFactor = -1;
    }

    //checks when pawn moves up by one/two spaces
    if (getPieceAt(desRow, desCol) == NULL)
    { // "Is there a piece at your desired row & column?"
        if (curRow + reverseFactor == desRow && curCol == desCol)
        {

            return 1;
        }

        if (curRow + (2 * reverseFactor) == desRow && p->moves < 1 && curCol == desCol)
        {
            return 1;
        }
    } //end if

    //if there's piece at desired location, desired row is up one, and desired column is one left or one right, return 1
    if ((getPieceAt(desRow, desCol) != NULL) && (desRow == curRow + reverseFactor) && (desCol == curCol + 1 || desCol == curCol - 1))
    {
        if (getPieceAt(desRow, desCol)->team == p->team)
        { //friendly fire detection
            return 0;
        }
        else
        {
            return 1;
        }
    } //end capture if

    if (isValidEnPassant(p, desRow, desCol, reverseFactor))
    {
        return 2;
    }
    return 0;
} //end func isValidPawnMovement

/* Checks that a valid en passant move is attempted
 * Returns 1 if valid, 0 if not 
 */
int isValidEnPassant(PIECE *p, int tRow, int tCol, int revFactor)
{
    int cRow = p->row;
    int cCol = p->col;

    // checks if adjacent pieces are pawns,      AND  checks if adjacent pieces are on other team, AND checks if current row is in 4,5  AND if the adjacent piece has only moved once (therefore it has double moved)
    if (getPieceAt(cRow, tCol) != NULL)
    {
        //printf("\nPiece to left or right of pawn you are moving is exists");
        if ((getPieceAt(cRow, tCol)->type == 'P'))
        { //&& (getPieceAt(curRow, desCol)->team != p->team) && (getPieceAt(curRow, desCol)->moves == 1) && (curRow == 4 || curRow == 5) && (curCol == desCol+1 || curCol == desCol-1) && (curRow+reverseFactor == desRow) && ( (getPreviouslyMovedWhitePiece() == getPieceAt(curRow, desCol))|| (getPreviouslyMovedBlackPiece() == getPieceAt(curRow,desCol)) ) ) {
            //printf("\nThe piece mentioned above is a pawn");
            if (getPieceAt(cRow, tCol)->team != p->team)
            {
                //printf("\nPiece is on the other team");
                if (getPieceAt(cRow, tCol)->moves == 1)
                {
                    //printf("\nPiece there only moved once");
                    if ((cRow == 3 || cRow == 4))
                    {
                        //printf("\nIf the capturing piece is on row 4 or five");
                        if ((cCol == tCol + 1 || cCol == tCol - 1))
                        {
                            //printf("\nIf it's a diagonal move(vertical check");
                            if ((cRow + revFactor == tRow))
                            {
                                //printf("\nIf it's a diagonal move(horizontal check)");
                                if ((getPreviouslyMovedWhitePiece() == getPieceAt(cRow, tCol)) || (getPreviouslyMovedBlackPiece() == getPieceAt(cRow, tCol)))
                                {
                                    //printf("If the captured piece is the last moved piece on that team");
                                    return 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

/* Checks that a valid King move is attempted
 * Returns 1 if valid, 0 if not 
 */
int isValidKingMove(PIECE *p, int toRow, int toCol)
{
    int curRow = getPieceRow(p);
    int curCol = getPieceCol(p); //NOTE: compiler says that this variable is unused

    //Checks if king wants to move up
    if (curRow + (1) == toRow && curCol == toCol)
    {
        if (getPieceAt(toRow, toCol))
        {
            if (getPieceAt(toRow, toCol)->team == p->team)
            { //friendly fire detection
                return 0;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            return 1;
        }
    }
    //Checks if king wants to move to upright
    if (curRow + (1) == toRow && curCol + (1) == toCol)
    {
        if (getPieceAt(toRow, toCol))
        {
            if (getPieceAt(toRow, toCol)->team == p->team)
            { //friendly fire detection
                return 0;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            return 1;
        }
    }
    //checks if king wants to move to right
    if (curRow == toRow && curCol + (1) == toCol)
    {
        if (getPieceAt(toRow, toCol))
        {
            if (getPieceAt(toRow, toCol)->team == p->team)
            { //friendly fire detection
                return 0;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            return 1;
        }
    }
    //checks if king wants to move bottom right
    if (curRow - (1) == toRow && curCol + (1) == toCol)
    {
        if (getPieceAt(toRow, toCol))
        {
            if (getPieceAt(toRow, toCol)->team == p->team)
            { //friendly fire detection
                return 0;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            return 1;
        }
    }
    //checks if king wants to move bottom
    if (curRow - (1) == toRow && curCol == toCol)
    {
        if (getPieceAt(toRow, toCol))
        {
            if (getPieceAt(toRow, toCol)->team == p->team)
            { //friendly fire detection
                return 0;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            return 1;
        }
    }
    //checks if king wants to move bottom left
    if (curRow - (1) == toRow && curCol - (1) == toCol)
    {
        if (getPieceAt(toRow, toCol))
        {
            if (getPieceAt(toRow, toCol)->team == p->team)
            { //friendly fire detection
                return 0;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            return 1;
        }
    }
    //checks if king wants to move left
    if (curRow == toRow && curCol - (1) == toCol)
    {
        if (getPieceAt(toRow, toCol))
        {
            if (getPieceAt(toRow, toCol)->team == p->team)
            { //friendly fire detection
                return 0;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            return 1;
        }
    }
    //checks if king wants to move top left
    if (curRow + (1) == toRow && curCol - (1) == toCol)
    {
        if (getPieceAt(toRow, toCol))
        {
            if (getPieceAt(toRow, toCol)->team == p->team)
            { //friendly fire detection
                return 0;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            return 1;
        }
    }

    if (curRow == toRow && (curCol + 2 == toCol || curCol - 2 == toCol))
    {
        if (isValidCastling(p, toRow, toCol))
        {
            return 3; // 3 is the code for castling operations
        }
    }
    return 0;
} //end isValidKingMove

/* Ensures no out of bounds checks
 * Returns 0 if valid or 7 when trying to access out of board
 */
int Limit(int input)
{
    if (input < 0)
    {
        return 0;
    }
    else if (input > 7)
    {
        return 7;
    }
    else
    {
        return input;
    }
}

/* Checks that a valid Bishop move is attempted
 * Returns 1 if valid, 0 if not 
 */
int isValidBishopMove(PIECE *p, int toRow, int toCol)
{
    int curRow = getPieceRow(p);
    int curCol = getPieceCol(p);
    int spaces;

    //if the location is not a diagonal it is invalid
    if (abs(curRow - toRow) != abs(curCol - toCol))
    {
        return 0;
    }
    else
    {
        spaces = abs(curRow - toRow);
    }
    for (int i = 1; i <= spaces; i++)
    {
        //only checks for the line in its direction, so the piece won't be locked by an adjacent piece in different direction
        if (toRow > curRow && toCol > curCol)
        {
            //checks pieces for top right diagonal moves
            if (getPieceAt(Limit(curRow + i), Limit(curCol + i)) != NULL && curRow + i != toRow && curCol + i != toCol)
            {
                return 0;
            }
        }
        if (toRow > curRow && toCol < curCol)
        {
            //checks pieces for top left diagonal moves
            if (getPieceAt(Limit(curRow + i), Limit(curCol - i)) != NULL && curRow + i != toRow && curCol - i != toCol)
            {
                return 0;
            }
        }
        if (toRow < curRow && toCol < curCol)
        {
            //checks pieces for bottom left diagonal moves
            if (getPieceAt(Limit(curRow - i), Limit(curCol - i)) != NULL && curRow - i != toRow && curCol - i != toCol)
            {
                return 0;
            }
        }
        if (toRow < curRow && toCol > curCol)
        {
            //checks pieces for bottom right diagonal moves
            if (getPieceAt(Limit(curRow - i), Limit(curCol + i)) != NULL && curRow - i != toRow && curCol + i != toCol)
            {
                return 0;
            }
        }
    } //end for

    //If it reaches this point, all illegal moves checks are negative, so all is good and its a legal move
    if (getPieceAt(toRow, toCol) != NULL)
    {
        if (getPieceAt(toRow, toCol)->team == p->team)
        { //friendly fire detection
            return 0;
        }
        else
        {
            return 1; //enemy piece there
        }
    }
    else
    {
        return 1;
    }
} //end isValidBishopMove

/* Checks that a valid rook move is attempted
 * Returns 1 if valid, 0 if not 
 */
//Checks for Legal Rook Movement
int isValidRookMove(PIECE *p, int toRow, int toCol)
{
    int curRow = getPieceRow(p);
    int curCol = getPieceCol(p);

    /*int rFactor = 1;
    if(p->team == 'B'){
        rFactor = -1;
    }*/

    for (int i = 1; i < 8; i++)
    {

        //so it only checks one direction at a time
        if (curRow < toRow)
        {

            //checks pieces for top moves
            if (curRow + i == toRow && curCol == toCol)
            {
                if (getPieceAt(toRow, toCol) != NULL)
                { //if there's a piece at end location
                    if (getPieceAt(toRow, toCol)->team == p->team)
                    { //friendly fire detection
                        return 0;
                    }
                    else
                    {
                        return 1; //enemy piece there
                    }
                }
                else
                {
                    return 1;
                }
            }
            else if (curRow + i != toRow && getPieceAt(Limit(curRow + i), curCol) != NULL) //check if piece is encountered before the toRow
            {
                return 0;
            }
        }

        if (curCol > toCol)
        {
            //checks pieces for left moves
            if (curRow == toRow && curCol - i == toCol)
            {
                if (getPieceAt(toRow, toCol) != NULL)
                {
                    if (getPieceAt(toRow, toCol)->team == p->team)
                    { //friendly fire detection
                        return 0;
                    }
                    else
                    {
                        return 1; //enemy piece present
                    }
                }
                else
                {
                    return 1;
                }
            }
            else if (curCol - i != toCol && getPieceAt(curRow, Limit(curCol - i)) != NULL) //check if piece is encountered before the toRow
            {
                return 0;
            }
        }

        if (curRow > toRow)
        {
            //checks pieces for bottom moves
            if (curRow - i == toRow && curCol == toCol)
            {
                if (getPieceAt(toRow, toCol) != NULL)
                {
                    if (getPieceAt(toRow, toCol)->team == p->team)
                    { //friendly fire detection
                        return 0;
                    }
                    else
                    {
                        return 1; //enemy piece there
                    }
                }
                else
                {
                    return 1;
                }
            }
            else if (curRow - i != toRow && getPieceAt(Limit(curRow - i), curCol) != NULL) //check if piece is encountered before the toRow
            {
                return 0;
            }
        }

        if (curCol < toCol)
        {
            //checks pieces for right moves
            if (curRow == toRow && curCol + i == toCol)
            {
                if (getPieceAt(toRow, toCol) != NULL)
                {
                    if (getPieceAt(toRow, toCol)->team == p->team)
                    { //friendly fire detection
                        return 0;
                    }
                    else
                    {
                        return 1; //enemy piece present
                    }
                }
                else
                {
                    return 1;
                }
            }
            else if (curCol + i != toCol && getPieceAt(curRow, Limit(curCol + i)) != NULL) //check if piece is encountered before the toRow
            {
                return 0;
            }
        }

    } //end for

    return 0;
} //end isValidRookMove

/* Checks that a valid Knight move is attempted
 * Returns 1 if valid, 0 if not 
 */
//Checks if a valid knight move is being made
int isValidKnightMove(PIECE *p, int toRow, int toCol)
{
    int curRow = getPieceRow(p);
    int curCol = getPieceCol(p);

    if (curRow + 2 == toRow && curCol + 1 == toCol)
    {
        if (getPieceAt(toRow, toCol) != NULL)
        {
            if (getPieceAt(toRow, toCol)->team == p->team)
            { //if it's friendly
                return 0;
            }
            else
            {
                return 1; //if it's enemy
            }
        }
        else
        {
            return 1; //no piece there and move is an L shape so it's good
        }

    } //Checks for up two, one to the right movement

    if (curRow + 2 == toRow && curCol - 1 == toCol)
    {
        if (getPieceAt(toRow, toCol) != NULL)
        {
            if (getPieceAt(toRow, toCol)->team == p->team)
            { //if it's friendly
                return 0;
            }
            else
            {
                return 1; //if it's enemy
            }
        }
        else
        {
            return 1;
        }

    } //checks for up two, one to the left

    if (curRow + 1 == toRow && curCol + 2 == toCol)
    {
        if (getPieceAt(toRow, toCol) != NULL)
        {
            if (getPieceAt(toRow, toCol)->team == p->team)
            { //if it's friendly
                return 0;
            }
            else
            {
                return 1; //if it's enemy
            }
        }
        else
        {
            return 1;
        }

    } //checks for up one, two to the right
    if (curRow + 1 == toRow && curCol - 2 == toCol)
    {
        if (getPieceAt(toRow, toCol) != NULL)
        {
            if (getPieceAt(toRow, toCol)->team == p->team)
            { //if it's friendly
                return 0;
            }
            else
            {
                return 1; //if it's enemy
            }
        }
        else
        {
            return 1;
        }

    } //checks for up one, two to the left
    if (curRow - 2 == toRow && curCol + 1 == toCol)
    {
        if (getPieceAt(toRow, toCol) != NULL)
        {
            if (getPieceAt(toRow, toCol)->team == p->team)
            { //if it's friendly
                return 0;
            }
            else
            {
                return 1; //if it's enemy
            }
        }
        else
        {
            return 1;
        }

    } //Checks for down two, one to the right

    if (curRow - 2 == toRow && curCol - 1 == toCol)
    {
        if (getPieceAt(toRow, toCol) != NULL)
        {
            if (getPieceAt(toRow, toCol)->team == p->team)
            { //if it's friendly
                return 0;
            }
            else
            {
                return 1; //if it's enemy
            }
        }
        else
        {
            return 1;
        }

    } //checks for down two, one to the left

    if (curRow - 1 == toRow && curCol - 2 == toCol)
    {
        if (getPieceAt(toRow, toCol) != NULL)
        {
            if (getPieceAt(toRow, toCol)->team == p->team)
            { //if it's friendly
                return 0;
            }
            else
            {
                return 1; //if it's enemy
            }
        }
        else
        {
            return 1;
        }

    } //checks for down one, two to the left

    if (curRow - 1 == toRow && curCol + 2 == toCol)
    {
        if (getPieceAt(toRow, toCol) != NULL)
        {
            if (getPieceAt(toRow, toCol)->team == p->team)
            { //if it's friendly
                return 0;
            }
            else
            {
                return 1; //if it's enemy
            }
        }
        else
        {
            return 1;
        }

    }         //checks for down one, two to the right
    return 0; //all combinations of L's made, move is therefore invalid
} //end func isValidKnightMove

/* Checks that a valid castling move is attempted
 * Returns 1 if valid, 0 if not 
 */
int isValidCastling(PIECE *p, int toRow, int toCol)
{

    if ((p->type == 'K') && (toRow == p->row) && (p->moves == 0))
    {
        if (p->col > toCol)
        { //left side
            //printf("\npcol<toCol");
            if ((getPieceAt(toRow, toCol) == NULL))
            {
                //printf("\nthere is no piece at where the king is trying to move");
                if (getPieceAt(toRow, toCol - 1) == NULL)
                {
                    //printf("\nthere is no piece to the left of king toCol");
                    if (getPieceAt(toRow, toCol + 1) == NULL)
                    {
                        //printf("\nNo piece to right of King toCol");
                        if (getPieceAt(toRow, toCol - 2) != NULL) // took out -> type
                        {
                            //printf("The toCol-2 piece exists!");
                            if (getPieceAt(toRow, toCol - 2)->type == 'R')
                            {
                                //printf("\nRook is two units left to King toCol");
                                if (getPieceAt(toRow, toCol - 2)->team == p->team)
                                {
                                    //printf("\nRook is a friendly");
                                    if ((getPieceAt(toRow, toCol - 2)->moves == 0))
                                    {
                                        //printf("\nThe rook hasnt moved");
                                        if (isAttacked(p->team, toRow, toCol) || isAttacked(p->team, toRow, toCol + 1) || isAttacked(p->team, toRow, toCol - 1))
                                        {
                                            //printf("\nEmpty spaces are in check");
                                            return 0;
                                        }
                                        else
                                        {
                                            //printf("\nEmpty spaces ARE NOT in check.");
                                            return 1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        else if (p->col < toCol)
        {
            if (getPieceAt(toRow, toCol) == NULL)
            {
                //printf("\npCol>toCol");
                if ((getPieceAt(toRow, toCol - 1) == NULL))
                {
                    //printf("Piece left of toCol is empty");
                    if (getPieceAt(toRow, toCol + 1) != NULL)
                    {
                        //printf("\nPiece right on king is NOT empty");
                        if (getPieceAt(toRow, toCol + 1)->type == 'R')
                        {
                            //printf("\nRPiece right of king toCol is a rook");
                            if (getPieceAt(toRow, toCol + 1)->team == p->team)
                            {
                                //printf("\nPiece to right og king toCol is a buddy");
                                if (getPieceAt(toRow, toCol + 1)->moves == 0)
                                {
                                    //printf("\nPiece to right of king toCol has NOT moved ");
                                    if (isAttacked(p->team, toRow, toCol) || isAttacked(p->team, toRow, toCol - 1))
                                    {
                                        //printf("\nThe pieces in between rook and king are being attacked");
                                        return 0;
                                    }
                                    else
                                    {
                                        //printf("\nIts safe to move");
                                        return 1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    } //end mega if/else
    return 0;
} //end isValifCastling

/* Checks that a valid queen move is attempted
 * Returns 1 if valid, 0 if not 
 */
int isValidQueenMove(PIECE *p, int toRow, int toCol)
{
    //A queen is a bishop + rook, so if these two conditions return 1, then its a valid move!
    if (isValidBishopMove(p, toRow, toCol) || isValidRookMove(p, toRow, toCol))
    {
        return 1;
    }
    return 0;
} //end isValidFreddyMercuryMove
