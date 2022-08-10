#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>
#include <glib.h>
#include "Board.h"
#include "Square.h"
#include "Check.h"
#include "Piece.h"
#include "Movement.h"

//vars
GtkWidget * window;
GtkWidget *hBox;
GtkWidget *board;
GtkTextBuffer *boardbuffer; 
GtkWidget *mvEntry;
GtkWidget *sendMvBut;
int row, col, endrow, endcol;
char move[5];
int isNewGame = 0;

//funcs
static void destroy(GtkWidget *widget, gpointer data);
static void decipherMessage(GtkWidget *widget, gpointer data);


int main (int argc, char **argv)
{
  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "Chess");   
  gtk_window_set_default_size(GTK_WINDOW (window), 300, 300);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);
  g_signal_connect (window, "delete-event", G_CALLBACK (destroy), NULL);

  hBox = gtk_hbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window), hBox);
  gtk_widget_show(hBox);

 /* Create a multiline text widget. */
  board = gtk_text_view_new ();
  boardbuffer = gtk_text_buffer_new(NULL);
  
  /* Obtaining the buffer associated with the widget. */
  gtk_text_view_set_buffer(GTK_TEXT_VIEW(board), boardbuffer);
  gtk_box_pack_start (GTK_BOX (hBox), board, 1, 1, 0);

  //entry for move
  mvEntry = gtk_entry_new();                               
  gtk_editable_set_editable(GTK_EDITABLE(mvEntry), TRUE);
  gtk_box_pack_start(GTK_BOX(hBox), mvEntry, TRUE, TRUE, 0);
  gtk_widget_show(mvEntry);

  //button to send whatever is in the entry for move
  sendMvBut = gtk_button_new();
  g_signal_connect (sendMvBut, "clicked", G_CALLBACK (decipherMessage), NULL);        
  gtk_box_pack_start(GTK_BOX (hBox), sendMvBut, FALSE, FALSE, 0);
  gtk_widget_show (sendMvBut); 

  gtk_widget_show_all (window);
  gtk_main();
  
  /*Pseudocode for Chess algorithm

  isNewGame = 1;

  while(!Checkmate){

    if(isNewGame){
      CreateBoard();
      DefaultPieceSetup();
      isNewGame = 0;  
    }
    PrintGTKBoard()
    textLabel = Make a move!
    gtk_text_buffer_set_text (boardbuffer, chessText, -1);
    while(1){}
      if(userPressesSendMove){
        get input from mvEntry
        if(movePieceTo(fromPos, toPos)){
          PrintGTKBoard()
          gtk_text_buffer_set_text (boardbuffer, chessText, -1);
          sendRequestToServer("Hey I made a move, this is it")
          break;
        }
      }
    }

    if(isCheckmate){
      tell the server: "CHECKMATE <winnerUsername>"
      textLabel = "winnerUsername wins by Checkmate" 
      //make sure to print the above line for both clients. 
      isNewGame = 1;
      break;
    }

    textLabel = Waiting for your opponent to make a move

    msgRecieved = ServerResponse();

    while(strcmp( msgRecieved, EnemyMoveResponse) == 1){
      if(strcmp(msgRecieved, EnemyMoveResponse) == 0){
        movePieceTo(enemFrom, enemTo)
      }
    }
    if(isCheckmate)
  }
  End of my pseudocode*/

  return 0;
}

static void destroy(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}//end func destroy

static void decipherMessage(GtkWidget *widget, gpointer data){
  char *input;

  if(isNewGame){
    input = gtk_entry_get_text(GTK_ENTRY(mvEntry));

    //printf("It is %s", input);
  
    row = input[0] - 97;
	  col = 56 - input[1];
    endrow = input[2] - 97;
	  endcol = 56 - input[3];  

    if(isValidPieceMove(getPieceAt(col, row), endcol, endrow)){
      movePieceTo(getPieceAt(col, row), col, row, endcol, endrow);
      if(isCheckmate('W')){
        //gameOver = 1;
        //printf("Game over!");
      }
    }

    PrintGTKBoard();
    gtk_text_buffer_set_text (boardbuffer, chessText, -1);

  }
  else{
    isNewGame = 1;
    CreateBoard();
    DefaultPieceSetup();
    PrintGTKBoard();
    gtk_text_buffer_set_text (boardbuffer, chessText, -1);
  }
}

