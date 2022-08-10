/************************************************************/
/*  Chess.c                                                 */
/*                                                          */
/*  Main Program: Accept user inputs, include while loop,   */
/*                call functions                            */
/************************************************************/
// 4/23 Bradley - Started main Chess program, Default board setup only, takes inputs and moves pieces, alternating turns
// 4/29 + 30 Bradley - Added CustomBoardSetup to allow for any setup of boards, removed many temporary goto statements
// 5/3  Taylor - Adding checkamte implementation, changed PlayerTurn Type to return value for check 
// 5/4 Bradley - Added spectate match option
// 5/5 Bradley - Added PrintMove to display moves made 

#include "Board.h"
#include "Piece.h"
#include "Square.h"
#include "Movement.h"
#include "AI_MinMax.h"
#include "Checkmate.h"
#include "Check.h"
#include "Chess.h"

int main() {
	fp = fopen("./bin/Gamelog.txt", "w");
	fprintf(fp, "Gamelog\n");
	int turn = 1;			//used to alternate turns
	char input;			//used to decide teams and whether player wants to play against computer or another human
	int checkmate = 0;			// checkmate flag -- if 1 then while loop while stop 
	int check = 0; 				//check flag -- if 1 then player is in check, but won't exit loop
	int turncounter = 1;	//used to keep track of the number of turns that have passed
	int spectate = 0;		//used to see if the player wants to play the game or watch an AI game (Computer vs Computer)
	int AICheckFlag = 0;
	int secondturnprint = 0;
	CreateBoard();
	printf("\n\nWelcome to chess!");
	while(1) {
		printf("\nWould you like to play classic chess or create a custom board to play?(A for Classic, B for Custom): ");
		scanf(" %c", &input);
		if (input == 'A') {
			DefaultPieceSetup();					//sets up board normally
			break;
		} else if (input == 'B') {
			CustomPieceSetup();						//allows to set up a custom board
			break;
		} else {
			printf("\nInvalid Input!\n");
		}
	} 
	while (1) {
		printf("\nWould you like to play with this board or spectate a game (Computer vs Computer) with this board? (P for play, S for spectate): ");
		scanf(" %c", &input);
		if (input == 'P') {
			spectate = 0;
			player = 'H';
			break;
		} else if (input == 'S') {
			spectate = 1;
			playerteam = 'W';
			otherteam = 'B';
			player = 'C';
			otherplayer = 'C';
			firstteam = 'W';
			goto gamestart;
		} else {
			printf("\nInvalid Input!\n");
		}
	}
	while(1) {
		printf("\nWhich team would you like to play as?(B = Black, W = white):");
		scanf(" %c", &input);
		if (input == 'B') {
			playerteam = 'B';						//player is Black other player is White
			otherteam = 'W';
			break;
		} else if (input == 'W') {
			playerteam = 'W';						//player is White other player is Black
			otherteam = 'B';
			break;
		} else {
			printf("\nInvalid Input!\n");
		}
	} 
	while (1) {
		printf("\nWould you like to play against another human(locally) or a computer player? (H for Human and C for Computer): ");
		scanf(" %c", &input);
		if (input == 'H') {
			otherplayer = 'H';								//against human
			break;
		} else if (input =='C') {
			otherplayer = 'C';								//against computer
			break;
		} else {
			printf("\nInvalid Input!\n");
		}
	}

	while (1) {
		printf("\nDo you want to go first or second? (1 for first, 2 for second):");
		scanf(" %c", &input);
		if (input == '1') {
			turn = 1;
			firstteam = playerteam;
			secondturnprint = 2;				//prints algebraic moves in the second block of code (turn % 2 == 0)
			break;
		} else if (input == '2') {
			turn = 2;
			firstteam = otherteam;
			secondturnprint = 1;				//prints algebraic moves in the first block of code (turn % 2 == 1)
			break;
		} else {
			printf("\nInvalid Input!\n");
		}
	}

	gamestart:while (1 && checkmate == 0) {			//do until checkmate //added checkmate flag here
		PrintBoard(1);			//Display the board
		printf("\nTurn %d: Player ", turncounter);
		fprintf(fp,"\nTurn %d: Player ", turncounter);
		if ((turn % 2) == 1) {										//turn 1
			if (spectate == 1) {									//AI vs AI showdown
				printf("1's Turn Team: (W) (Computer) \n");
				fprintf(fp,"1's Turn Team: (W) (Computer) \n");
				AICheckFlag = AI_MinMax(playerteam);
				if (AICheckFlag == 1){
					PIECE * piece = getPreviouslyMovedBlackPiece();
					if (piece->type != 'P') {
						printf("\n%c", piece->type);
						fprintf(fp, "\n%c", piece->type);
					}
					printf("%c%c", piece->col + 97, 56 - piece->row);
					fprintf(fp, "%c%c", piece->col + 97, 56 - piece->row);
					printf("#");
					fprintf(fp, "#");
					printf("\n!--------------------CHECKMATE: GAME OVER TEAM %c LOSES--------------------!\n", playerteam);
					fprintf(fp,"\n!--------------------CHECKMATE: GAME OVER TEAM %c LOSES--------------------!\n", playerteam);
					checkmate = 1;
					break;
				}
				PIECE * movedpiece = getPreviouslyMovedWhitePiece();
				PrintMove(playerteam, movedpiece, turncounter, player);
				sleep(2);					//little delay so can actually read the move

			} else {												//player vs AI/Human showdown
				printf("1's Turn Team: (%c) (Human) \n", playerteam);
				fprintf(fp, "1's Turn Team: (%c) (Human) \n", playerteam);	
				PlayerTurn(playerteam, turncounter);
			} 
			//check
			check = Check(otherteam);
			if (check == 1){
				checkmate = totalCheckmate(otherteam);
				if (checkmate == 1){
					if (secondturnprint == 1) {
						PrintAlgebraicMove(firstteam);			//prints the move then the # for checkmating the other team
						printf("#");
						fprintf(fp, "#");
					} else if (secondturnprint == 2) {					//if checkmate occurs in the first move of the pair of algebraic moves
						PIECE * piece = NULL;
						if (firstteam == 'W') {
							piece = getPreviouslyMovedWhitePiece();
						} else {
							piece = getPreviouslyMovedBlackPiece();
						}
						if (piece->type != 'P') {
							printf("%c", piece->type);
							fprintf(fp,"%c", piece->type);
						}
						printf("%c%c", piece->col + 97, 56 - piece->row);
						fprintf(fp,"%c%c", piece->col + 97, 56 - piece->row);
						printf("#");
						fprintf(fp, "#");
					}
					printf("\nFinal Board State:\n");
					fprintf(fp,"\nFinal Board State:\n");
					PrintBoard(1);					
					printf("\n!--------------------CHECKMATE: GAME OVER TEAM %c LOSES--------------------!\n", otherteam);
					fprintf(fp,"\n!--------------------CHECKMATE: GAME OVER TEAM %c LOSES--------------------!\n", otherteam);
					break;
				}
				firstteaminducedcheck = 1;
				printf("\n!----------Team %c is in check! Please move out of check!----------! \n", otherteam);
				fprintf(fp,"\n!----------Team %c is in check! Please move out of check!----------! \n", otherteam);
			}
			if (secondturnprint == 1) {				//first block of code (turn % 2 == 1) is the second turn due to user input for turn order
				PrintAlgebraicMove(firstteam);
			}

		} else if ((turn % 2) == 0) {							//turn 2
			if (spectate == 1) {								//AI vs AI showdown
				printf("2's Turn Team: (B) (Computer) \n");
				fprintf(fp,"2's Turn Team: (B) (Computer) \n");
				AICheckFlag = AI_MinMax(otherteam);
				if (AICheckFlag == 1){
					PrintAlgebraicMove(firstteam);
					printf("#");
					fprintf(fp, "#");
					printf("\n!--------------------CHECKMATE: GAME OVER TEAM %c LOSES--------------------!\n", otherteam);
					fprintf(fp,"\n!--------------------CHECKMATE: GAME OVER TEAM %c LOSES--------------------!\n", otherteam);
					checkmate = 1;
					break;
				}
				PIECE * movedpiece = getPreviouslyMovedBlackPiece();			
				PrintMove(otherteam, movedpiece, turncounter, otherplayer);
				PrintAlgebraicMove(firstteam);
				sleep(2);					//little delay so can actually read the move
			}
			else {
				printf("2's Turn Team: (%c) ", otherteam);
				fprintf(fp, "2's Turn Team: (%c) ", otherteam);
				if (otherplayer == 'H') {				//other human's turn
					printf("(Human) \n");
					fprintf(fp, "(Human) \n");
					PlayerTurn(otherteam, turncounter);
					
				} else if (otherplayer == 'C') {		//AI turn
					printf("(Computer) \n");
					fprintf(fp, "(Computer) \n");
					AICheckFlag = AI_MinMax(otherteam);
					if (AICheckFlag == 1){
						printf("\n!--------------------CHECKMATE: GAME OVER TEAM %c LOSES--------------------!\n", otherteam);
						fprintf(fp,"\n!--------------------CHECKMATE: GAME OVER TEAM %c LOSES--------------------!\n", otherteam);
						checkmate = 1;
						break;
					}
					PIECE * movedpiece = NULL;
					if (otherteam == 'B'){
						movedpiece = getPreviouslyMovedBlackPiece();
					} else if (otherteam == 'W') {
						movedpiece = getPreviouslyMovedWhitePiece();
					}
					PrintMove(otherteam, movedpiece, turncounter, otherplayer);
					sleep(2);
				}
			}
			//check
			check = Check(playerteam);
			if (check == 1){
				checkmate = totalCheckmate(playerteam);
				if (checkmate == 1){
					if (secondturnprint == 2) {
						PrintAlgebraicMove(firstteam);			//prints the move then the # for checkmating the other team
						printf("#");
						fprintf(fp, "#");
					} else if (secondturnprint == 1) {			//checkmate happens on first move in pair, second move doesn't get printed
						PIECE * piece = NULL;
						if (firstteam == 'W') {
							piece = getPreviouslyMovedWhitePiece();
						} else {
							piece = getPreviouslyMovedBlackPiece();
						}
						if (piece->type != 'P') {
							printf("%c", piece->type);
							fprintf(fp,"%c", piece->type);
						}
						printf("%c%c", piece->col + 97, 56 - piece->row);
						fprintf(fp,"%c%c", piece->col + 97, 56 - piece->row);
						printf("#");
						fprintf(fp, "#");
					}
					printf("\nFinal Board State:\n");
					fprintf(fp,"\nFinal Board State:\n");
					PrintBoard(1);
					printf("\n!--------------------CHECKMATE: GAME OVER TEAM %c LOSES--------------------!\n", playerteam);
					fprintf(fp,"\n!--------------------CHECKMATE: GAME OVER TEAM %c LOSES--------------------!\n", playerteam);
					break;
				}				
				secondteaminducedcheck = 1;
				printf("\n!----------Team %c is in check! Please move out of check!----------! \n", playerteam);
				fprintf(fp,"\n!----------Team %c is in check! Please move out of check!----------! \n", playerteam);
			
			}
			if (secondturnprint == 2) {
				PrintAlgebraicMove(firstteam);				//every other turn prints out the movement pairs in Algebraic notation;
			}
		}

		
		turn++;					//increment the turn to flip turns to other players
		turncounter++;			//increment total number of turns
	}
	
	DeleteBoard();									//to free up everything after the game ends so no memory leaks
	fclose(fp);
	printf("\n----------Gamelog stored in bin/ folder----------\n");
	printf("\n\nThank you so much a-for-to playing our game!\n\n");
}

//edit: changed from void to int type 
void PlayerTurn(char team, int turnnumber) {
	char pieceinput[2];
	int row, col, endrow, endcol, movesuccess;
	PIECE * selectedpiece = NULL;
	
	pieceinputprompt:while (1) {
		printf("Select a piece by entering its location: " );
		scanf(" %2s", pieceinput);					
		row = pieceinput[0] - 97;					//converting to int from ASCII char
		col = 56 - pieceinput[1] ;					//converting to int from ASCII number, note to self check for invalid inputs
		if ((row < 0) || (row > 7) || (col < 0) || (col > 7)) {							
			printf("\nOut of bounds! \n");
			continue;																			//if user enters in out of bounds position
		} else {
			selectedpiece = getPieceAt(col, row);
			if (selectedpiece == NULL) {														//if piece doesn't exist at selected position
				printf("\nThere's no piece at that location!\n");
				continue;
			} else if (selectedpiece->team != team) {											//if user tries to select opponent's piece
				printf("\nWhoa there partner, you tried selecting the other team's piece!\n");
				continue;
			} else {																			//otherwise all is good so continue to movement
				break;
			}
		}
	}
	while (1) {
		printf("\nEnter the end location (enter 00 to de-select the current piece):");
		scanf(" %2s", pieceinput);
		if (strncmp(pieceinput, "00", 2) == 0) {						//if user enters in 00 goes back to selecting a piece
			goto pieceinputprompt;
		} else {
			endrow = pieceinput[0] - 97;
			endcol = 56 - pieceinput[1];
			if ((endrow < 0) || (endrow > 7) || (endcol < 0) || (endcol > 7)) {		//if end position not in range
				printf("\nOut of bounds!\n");
				continue;	
			}	
			movesuccess = movePieceTo(selectedpiece, col, row, endcol, endrow);
			if (movesuccess == 0) {													//if user tries to do an illegal move
				printf("\nInvalid Move! Please re-enter a legal move!\n");
				goto pieceinputprompt;
			} else {
				PrintMove(team, selectedpiece, turnnumber, 'H');							//displays move to console
			}
			return;
		}
		break;
	}

	
}

void PrintMove(char team, PIECE * piece, int turnnumber, char player) {				//prints Team, Turn, playertype, and piece moved from start location to end location
	char prevLocation[2];
	char newLocation[2];
	printf("\nFor Turn %d, Team (%c) (%c) moved ", turnnumber, team, player);
	fprintf(fp,"\nFor Turn %d, Team (%c) (%c) moved ", turnnumber, team, player);
	if (piece) {
		switch(piece->type) {
			case 'P':
				printf("Pawn from ");
				fprintf(fp, "Pawn from ");
				break;
			case 'R':
				printf("Rook from ");
				fprintf(fp,"Rook from ");
				break;
			case 'B':
				printf("Bishop from ");
				fprintf(fp,"Bishop from ");
				break;
			case 'N':
				printf("Knight from ");
				fprintf(fp, "Knight from ");
				break;
			case 'Q':
				printf("Queen from ");
				fprintf(fp,"Queen from ");
				break;
			case 'K':
				printf("King from ");
				fprintf(fp,"King from ");
				break;
		}
		prevLocation[0] = piece->prevCol + 97;
		prevLocation[1] = 56 - piece->prevRow;
		newLocation[0] = piece->col + 97;
		newLocation[1] = 56 - piece->row;
		printf("%c%c to %c%c\n", prevLocation[0], prevLocation[1], newLocation[0], newLocation[1]);
		fprintf(fp,"%c%c to %c%c\n", prevLocation[0],prevLocation[1], newLocation[0],newLocation[1]);
	}
}

void PrintAlgebraicMove(char firstteam) {					//Chess Alg notation prints moves in pairs
	char endloc1, endloc2;
	if (firstteam == 'W') {										//white is first so it's first move printed then black
		PIECE *whitepiece = getPreviouslyMovedWhitePiece();
		if (firstteampiececaptured == 1) {
			if (capturedpiecetype != 'P') {
				printf("\n%c", capturedpiecetype);
				fprintf(fp,"\n%c", capturedpiecetype);
			}
			endloc1 = capturedpiececol + 97;
			endloc2 = 56 - capturedpiecerow;
		} else if (firstteampiececaptured == 0) {
			if (whitepiece->type != 'P') {
				printf("\n%c", whitepiece->type);
				fprintf(fp,"\n%c", whitepiece->type);
			}
			endloc1 = whitepiece->col + 97;
			endloc2 = 56 - whitepiece->row;			
		}
		if (firstteammove == 1) {				//normal capture
			printf("x%c%c",endloc1, endloc2);
			fprintf(fp,"x%c%c",endloc1, endloc2);
		} else if (firstteammove == 2) {		//en passant capture
			if (firstteampiececaptured == 0) {
				printf("%cx%c%ce.p.",whitepiece->prevCol + 97, endloc1,endloc2);
				fprintf(fp,"%cx%c%ce.p.",whitepiece->prevCol + 97, endloc1,endloc2);				
			} else {																	//if the pawn that en passant-ed another pawn gets captured
				printf("%cx%c%ce.p.",capturedpiecestartcol + 97, endloc1,endloc2);
				fprintf(fp,"%cx%c%ce.p.",capturedpiecestartcol + 97, endloc1,endloc2);				
			}
		} else if (firstteammove == 31) {		//king side castling
			printf("0-0 %c%c", endloc1, endloc2);
			fprintf(fp,"0-0 %c%c", endloc1, endloc2);
		} else if (firstteammove == 32) {		//queen side castling
			printf("0-0-0 %c%c", endloc1, endloc2);
			fprintf(fp,"0-0-0 %c%c", endloc1, endloc2);
		} else {								//reg movement no capture
			printf("%c%c", endloc1, endloc2);
			fprintf(fp,"%c%c", endloc1, endloc2);
			if (firstteampawnpromotion == 1) {
				printf("=Q");
				fprintf(fp,"=Q");
			}
		}
		if (firstteaminducedcheck == 1) {
			printf("+");
			fprintf(fp, "+");
		}
		printf("  ");
		fprintf(fp, "  ");
		PIECE *blackpiece = getPreviouslyMovedBlackPiece();
		if (blackpiece->type != 'P') {
			printf("%c", blackpiece->type);
			fprintf(fp,"%c", blackpiece->type);
		}
		endloc1 = blackpiece->col + 97;
		endloc2 = 56 - blackpiece->row;
		if (secondteammove == 1) {
			printf("x%c%c", endloc1, endloc2);
			fprintf(fp,"x%c%c", endloc1, endloc2);
		} else if (secondteammove == 2) {
			printf("%cx%c%ce.p.",blackpiece->prevCol + 97, endloc1,endloc2);
			fprintf(fp, "%cx%c%ce.p.",blackpiece->prevCol + 97, endloc1,endloc2);
		} else if (secondteammove == 31) {
			printf("0-0 %c%c", endloc1, endloc2);
			fprintf(fp,"0-0 %c%c", endloc1, endloc2); 
		} else if (secondteammove == 32) {
			printf("0-0-0 %c%c", endloc1, endloc2);
			fprintf(fp,"0-0-0 %c%c", endloc1, endloc2);
		} else {
			printf("%c%c", endloc1, endloc2);
			fprintf(fp, "%c%c", endloc1, endloc2);
			if (secondteampawnpromotion == 1) {
				printf("=Q");
				fprintf(fp, "=Q");
			}
		}
		if (secondteaminducedcheck == 1) {
			printf("+");
			fprintf(fp, "+");
		} 
	} else if (firstteam == 'B') {								//black is first so their move is printed first then white
		PIECE *blackpiece = getPreviouslyMovedBlackPiece();
		if (firstteampiececaptured == 1) {
			if (capturedpiecetype != 'P') {
				printf("\n%c", capturedpiecetype);
				fprintf(fp,"\n%c", capturedpiecetype);
			}
			endloc1 = capturedpiececol + 97;
			endloc2 = 56 - capturedpiecerow;
		} else if (firstteampiececaptured == 0) {
			if (blackpiece->type != 'P') {
				printf("\n%c", blackpiece->type);
				fprintf(fp,"\n%c", blackpiece->type);
			}
			endloc1 = blackpiece->col + 97;
			endloc2 = 56 - blackpiece->row;			
		}
		if (firstteammove == 1) {					//standard capture
			printf("x%c%c", endloc1, endloc2);
			fprintf(fp,"x%c%c", endloc1, endloc2);
		} else if (firstteammove == 2) {			//en passant
			if (firstteampiececaptured == 0) {
				printf("%cx%c%ce.p.",blackpiece->prevCol + 97, endloc1,endloc2);
				fprintf(fp,"%cx%c%ce.p.",blackpiece->prevCol + 97, endloc1,endloc2);				
			} else {																	//if the pawn that en passant-ed another pawn gets captured
				printf("%cx%c%ce.p.",capturedpiecestartcol + 97, endloc1,endloc2);
				fprintf(fp,"%cx%c%ce.p.",capturedpiecestartcol + 97, endloc1,endloc2);				
			}
		} else if (firstteammove == 31) {			//kingside castling
			printf("0-0 %c%c", endloc1, endloc2);
			fprintf(fp,"0-0 %c%c", endloc1, endloc2);
		} else if (firstteammove == 32) {			//queenside castling
			printf("0-0-0 %c%c", endloc1, endloc2);
			fprintf(fp,"0-0-0 %c%c", endloc1, endloc2);
		} else {
			printf("%c%c", endloc1, endloc2);
			fprintf(fp,"%c%c", endloc1, endloc2);
			if (firstteampawnpromotion == 1) {
				printf("=Q");
				fprintf(fp, "=Q");
			}
		}
		if (firstteaminducedcheck == 1) {			
			printf("+");
			fprintf(fp,"+");
		}	
		printf("  ");
		fprintf(fp, "  ");
		PIECE *whitepiece = getPreviouslyMovedWhitePiece();
		if (whitepiece->type != 'P') {
			printf("%c", whitepiece->type);
			fprintf(fp,"%c", whitepiece->type);
		}
		endloc1 = whitepiece->col + 97;
		endloc2 = 56 - whitepiece->row;
		if (secondteammove == 1) {
			printf("x%c%c", endloc1, endloc2);
			fprintf(fp,"x%c%c", endloc1, endloc2);
		} else if (secondteammove == 2) {
			printf("%cx%c%ce.p.",whitepiece->prevCol + 97, endloc1,endloc2);
			fprintf(fp,"%cx%c%ce.p.",whitepiece->prevCol + 97, endloc1,endloc2);
		} else if (secondteammove == 31) {
			printf("0-0 %c%c", endloc1, endloc2);
			fprintf(fp,"0-0 %c%c", endloc1, endloc2); 
		} else if (secondteammove == 32) {
			printf("0-0-0 %c%c", endloc1, endloc2);
			fprintf(fp,"0-0-0 %c%c", endloc1, endloc2);
		} else {
			printf("%c%c", endloc1, endloc2);
			fprintf(fp, "%c%c", endloc1, endloc2);
			if (secondteampawnpromotion == 1) {
				printf("=Q");
				fprintf(fp, "=Q");
			}
		}
		if (secondteaminducedcheck == 1) {
			printf("+");
			fprintf(fp, "+");
		}			
	}
	firstteaminducedcheck = 0;				//resetting variables for next run through
	secondteaminducedcheck = 0;
	firstteammove = 0;
	secondteammove = 0;
	firstteampawnpromotion = 0;
	secondteampawnpromotion = 0;
	firstteampiececaptured = 0;
}