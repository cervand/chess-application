/*
//basing off of QV GTK example for reference
TODO (5/22/20 12:52AM):
-Table organization (try to put chat bar and send button at bottom)
-Try to get received messages to list upwards
-Try to send with enter key
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include "ChatWinTest.h"
#include "Board.h"
#include "Square.h"
#include "Piece.h"
#include "Movement.h"
#include "Check.h"

static GtkWidget *window;
static GtkWidget *popup;
static GtkWidget *acceptbutton;
static GtkWidget *declinebutton;
static GtkWidget *popuptable;
static GtkWidget *MessageEntry;
static GtkWidget *button;
static GtkWidget *challenge;
static GtkWidget *receivebutton;
static GtkWidget *table;
//GtkWidget *MessageReceive;
static GtkWidget *board;
static GtkTextBuffer *buffer;

static GtkTextTag *wrap;
static GtkTextTag *justifyL;
static GtkTextIter start, end;
static GtkWidget *scrolled_window;
static GtkWidget *Info;
static GtkWidget *StartLabel;
static GtkWidget *EndLabel;
static GtkWidget *MoveButton;
static GtkWidget *StartEntry;
static GtkWidget *EndEntry;
static GtkTextBuffer *boardbuffer;
static char *firstuser;
static char *seconduser;
int row, col, endrow, endcol;
char move[5];
int isNewGame = 0;




/*  user1 is one who started it, user two is one who is going to receive message
    need an initial message parameter and challenge parameter
    challenge parameter would be an int that indicates whether to show chat window 
    only or chatwindow and chess */
int main(int argc, char *argv[]) {             
    
    gtk_init (&argc, &argv);

    /* Setting up containers */
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL) ;
        gtk_widget_set_size_request(window, WINDOW_WIDTH, WINDOW_HEIGHT) ;
        gtk_container_set_border_width (GTK_CONTAINER(window), WINDOW_BORDER) ;
        gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER) ;
        gtk_window_set_title(GTK_WINDOW(window), seconduser) ;
        gtk_window_set_resizable(GTK_WINDOW(window), TRUE) ;
    table = gtk_table_new (50, 50, TRUE);
        gtk_container_add (GTK_CONTAINER (window), table);

    buffer = gtk_text_buffer_new(NULL);
    wrap = gtk_text_buffer_create_tag (buffer, "wrapping", "wrap-mode", GTK_WRAP_WORD_CHAR, NULL);
    justifyL = gtk_text_buffer_create_tag(buffer, "leftside", "justification", GTK_JUSTIFY_LEFT, NULL);
    gtk_text_buffer_get_iter_at_offset (buffer, &start, 0);
    gtk_text_buffer_get_iter_at_offset (buffer, &end, -1);
    gtk_text_buffer_apply_tag (buffer, wrap, &start, &end);
    
    ChatLog = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(ChatLog), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(ChatLog), FALSE);

    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (ChatLog), GTK_WRAP_WORD_CHAR); 
    
    
    gtk_widget_show(ChatLog);
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(ChatLog), buffer);
    
    scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add (GTK_CONTAINER (scrolled_window), ChatLog);
    gtk_container_set_border_width (GTK_CONTAINER (scrolled_window), 5);
    gtk_table_attach(GTK_TABLE(table), scrolled_window, 0, 25, 0, 46, GTK_FILL, GTK_FILL, 0, 0);
    gtk_widget_show(scrolled_window);

    //CreateBoard();
    //DefaultPieceSetup();
    //PrintGTKBoard();
    boardbuffer = gtk_text_buffer_new(NULL);
    board = gtk_text_view_new();
    gtk_table_attach(GTK_TABLE(table), board, 26, 50, 0, 24, GTK_FILL, GTK_FILL, 0, 0);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(board), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(board), FALSE);
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(board), boardbuffer);
    //gtk_text_buffer_set_text (boardbuffer, chessText, -1);
    gtk_widget_hide(board);

    Info = gtk_label_new("Turn 1: ");
    gtk_table_attach(GTK_TABLE(table), Info, 26, 30, 24, 26, GTK_SHRINK, GTK_SHRINK, 0, 0);
    gtk_widget_hide(Info);

    StartLabel = gtk_label_new("Start: ");
    gtk_table_attach(GTK_TABLE(table), StartLabel, 26, 30, 28, 30, GTK_SHRINK, GTK_SHRINK, 0, 0);
    gtk_widget_hide(StartLabel);

    EndLabel = gtk_label_new("End:    ");
    gtk_table_attach(GTK_TABLE(table), EndLabel, 26, 30, 32, 34, GTK_SHRINK, GTK_SHRINK, 0, 0);
    gtk_widget_hide(EndLabel);

    MoveButton = gtk_button_new_with_label ("Move");
    gtk_table_attach(GTK_TABLE(table), MoveButton, 30, 35, 24, 26, GTK_SHRINK, GTK_SHRINK, 0, 0);
    gtk_widget_hide(MoveButton);

    StartEntry = gtk_entry_new();
    gtk_editable_set_editable(GTK_EDITABLE(StartEntry), TRUE);
    //gtk_box_pack_start(GTK_BOX(Box), MessageEntry, TRUE, TRUE, 0);
    gtk_table_attach(GTK_TABLE(table), StartEntry, 30, 49, 28, 30, GTK_FILL, GTK_FILL, 1, 0);
    gtk_widget_hide(StartEntry);

    EndEntry = gtk_entry_new();
    gtk_editable_set_editable(GTK_EDITABLE(EndEntry), TRUE);
    //gtk_box_pack_start(GTK_BOX(Box), MessageEntry, TRUE, TRUE, 0);
    gtk_table_attach(GTK_TABLE(table), EndEntry, 30, 49, 32, 34, GTK_FILL, GTK_FILL, 1, 0);
    gtk_widget_hide(EndEntry);
    //Box = gtk_hbox_new(FALSE,0);
    //gtk_container_add(GTK_CONTAINER(window), Box);
    
/*
    MessageReceive = gtk_label_new("");                   
    //gtk_misc_set_alignment(GTK_MISC(MessageReceive), 0.0f, 0.0f);
    gtk_table_attach(GTK_TABLE(table), MessageReceive, 0, 7, 0, 2, GTK_SHRINK, GTK_SHRINK, 0, 0);
    gtk_widget_show(MessageReceive);
*/
    MessageEntry = gtk_entry_new();
    gtk_editable_set_editable(GTK_EDITABLE(MessageEntry), TRUE);
    //gtk_box_pack_start(GTK_BOX(Box), MessageEntry, TRUE, TRUE, 0);
    gtk_table_attach(GTK_TABLE(table), MessageEntry, 1, 20, 46, 48, GTK_FILL, GTK_FILL, 1, 0);
    gtk_widget_show(MessageEntry);
    


    button = gtk_button_new_with_label ("Send");
    gtk_table_attach(GTK_TABLE(table), button, 20, 23, 46, 48, GTK_FILL, GTK_SHRINK, 0, 0);
    gtk_widget_show(button);
    //temporary button to simulate receiving message
    receivebutton = gtk_button_new_with_label ("Take");
    gtk_table_attach(GTK_TABLE(table), receivebutton, 23, 26, 46, 58, GTK_FILL, GTK_SHRINK, 0, 0);
    gtk_widget_show(receivebutton);

    //gtk_box_pack_start(GTK_BOX (Box), button, FALSE, TRUE, 10);
    challenge = gtk_button_new_with_label ("Fight");
    gtk_table_attach(GTK_TABLE(table), challenge, 20, 23, 48, 50, GTK_FILL, GTK_SHRINK, 0, 0);
    gtk_widget_show(challenge);

    /*pop up window for challenge*/
    popup = gtk_window_new(GTK_WINDOW_TOPLEVEL) ;
    gtk_widget_set_size_request(popup, 50, 40) ;
    gtk_container_set_border_width (GTK_CONTAINER(popup), WINDOW_BORDER) ;
    gtk_window_set_position(GTK_WINDOW(popup), GTK_WIN_POS_CENTER) ;
    gtk_window_set_title(GTK_WINDOW(popup), "Challenge Request") ;
    popuptable = gtk_table_new (2, 1, TRUE);
    gtk_container_add(GTK_CONTAINER(popup), popuptable);
    acceptbutton = gtk_button_new_with_label("Accept");
    gtk_table_attach(GTK_TABLE(popuptable), acceptbutton, 0, 1, 0, 1, GTK_FILL, GTK_FILL, 0, 0);
    gtk_widget_show(acceptbutton);
    declinebutton = gtk_button_new_with_label("Decline");
    gtk_table_attach(GTK_TABLE(popuptable), declinebutton, 1, 2, 0, 1, GTK_FILL, GTK_FILL, 0, 0);
    gtk_widget_show(declinebutton);

    /*register event handlers*/
    gtk_widget_add_events(window, GDK_KEY_PRESS_MASK);
    g_signal_connect (window, "delete_event", G_CALLBACK(destroychat), NULL) ;
    g_signal_connect (MessageEntry, "activate", G_CALLBACK (sendmessage), MessageEntry);
    g_signal_connect (button, "clicked", G_CALLBACK (sendmessage), NULL);
    g_signal_connect (challenge, "clicked", G_CALLBACK (fight), NULL);
    g_signal_connect (MoveButton, "clicked", G_CALLBACK (decipherMessage), NULL);
    g_signal_connect (receivebutton, "clicked", G_CALLBACK (receive), NULL);
    g_signal_connect (acceptbutton, "clicked", G_CALLBACK (chessstart), NULL);
    g_signal_connect (declinebutton, "clicked", G_CALLBACK (killpopup), NULL);
    g_signal_connect (popup, "delete_event", G_CALLBACK(killpopup), NULL) ;
    
    /*show the window*/
    gtk_widget_show(table);
    gtk_widget_show_all(window) ;
    gtk_main() ;

    return 0 ;

} //end main


gboolean destroychat (GtkWidget *widget,
         GdkEvent  *event,
         gpointer   data)
{  
  gtk_main_quit();
  return TRUE;
} //end destroy


gboolean sendmessage (GtkWidget *widget,
         GdkEvent  *event,
         gpointer   data) 
{
    /*
   gtk_widget_add_events(window, GDK_KEY_PRESS_MASK);
    const gchar *message = data;

    //if keypress == enter
    if (event->keyval = GDK_KEY_enter) {
        
        gtk_text_buffer_insert_at_cursor (buffer, "Username:", -1);
        gtk_text_buffer_insert_at_cursor (buffer, "\n", 1);
        gtk_text_buffer_insert_at_cursor (buffer, gtk_entry_get_text(GTK_ENTRY(MessageEntry)), -1);
        gtk_text_buffer_insert_at_cursor (buffer, "\n", 1);
        
        char *Response;
        char RecvBuffer[500], Buffer[1000];
        //const gchar *Text;
        //Text = gtk_entry_get_text(GTK_ENTRY(MessageEntry));
        strcpy(Buffer, "MESSAG");
        strcat(Buffer,  " ");                    //divider for the actual message
        strcat(Buffer, firstuser);
        strcat(Buffer," ");
        strcat(Buffer, gtk_entry_get_text(GTK_ENTRY(MessageEntry)));
        strcat(Buffer," ");
        strcat(Buffer, seconduser);


        
        //strcat(Buffer,Text);
        gtk_entry_set_text(GTK_ENTRY(MessageEntry), ""); 
        Response = ServerRequest(Buffer, RecvBuffer);

        
        
        return TRUE;
    }
    return FALSE;
    */
} //end send


gboolean fight (GtkWidget *widget, GdkEvent *event, gpointer data) 
{
    char *Response;
    char RecvBuffer[500], Buffer[1000];
    strcpy(Buffer, "GAMECH");
    strcat(Buffer,  " ");                    //divider for the actual message
    strcat(Buffer, firstuser);
    strcat(Buffer," ");
    strcat(Buffer, seconduser);

    gtk_widget_show(popuptable);
    gtk_widget_show(popup) ;
    
    /*

    Response = ServerRequest(Buffer, RecvBuffer);
    return TRUE;

    */

}


gboolean killpopup (GtkWidget *widget,
         GdkEvent  *event,
         gpointer   data) 
{
    gtk_widget_hide(popup);
    return TRUE;
}
gboolean chessstart (GtkWidget *widget,
         GdkEvent  *event,
         gpointer   data) 
{
    gtk_widget_destroy(popup);
    gtk_widget_show(board);
    gtk_widget_show(Info);
    
    gtk_widget_show(StartLabel);
    gtk_widget_show(EndLabel);
    gtk_widget_show(MoveButton);
    
    gtk_widget_show(StartEntry);
    gtk_widget_show(EndEntry);
    gtk_widget_hide(challenge);

    gtk_window_resize(GTK_WINDOW(window), WINDOW_HEIGHT + 100, WINDOW_HEIGHT) ;
    return TRUE;
}


gboolean movemade (GtkWidget *widget, GdkEvent *event, gpointer data) 
{
    
    char Message[256], RecvBuf[256];
    const gchar *start, *end;
    char *Response;
    if(gtk_entry_get_text_length(GTK_ENTRY(StartEntry)) == 2 && gtk_entry_get_text_length(GTK_ENTRY(EndEntry)) == 2){
        printf("\n%s", gtk_entry_get_text(GTK_ENTRY(StartEntry)));
        printf("\n%s", gtk_entry_get_text(GTK_ENTRY(EndEntry)));
        start = gtk_entry_get_text(GTK_ENTRY(StartEntry));
        end = gtk_entry_get_text(GTK_ENTRY(EndEntry));
        strcpy(Message, "CHSMVE ");
        strcat(Message, (char *) start);
        strcat(Message, " ");
        strcat(Message, (char *) end);
        
        //Response = ServerRequest(Message, RecvBuf);
        
    }
    else{
        printf("\nInvalid input(s)");
    }
    
    gtk_entry_set_text(GTK_ENTRY(StartEntry), "");
    gtk_entry_set_text(GTK_ENTRY(EndEntry), "");

    return TRUE;
}


gboolean receive (GtkWidget *widget,
         GdkEvent  *event,
         gpointer   data) 
{
    gtk_text_buffer_insert_at_cursor (buffer, seconduser, -1);
    gtk_text_buffer_insert_at_cursor (buffer, "\n", 1);
    gtk_text_buffer_insert_at_cursor (buffer, gtk_entry_get_text(GTK_ENTRY(MessageEntry)), -1);
    gtk_text_buffer_insert_at_cursor (buffer, "\n", 1);
    gtk_entry_set_text(GTK_ENTRY(MessageEntry), "");
    return TRUE;
}

/* Deciphers Moves the Text Entries and Plays Chess */
static void decipherMessage(GtkWidget *widget, gpointer data){
  char *fromPos;
  char *toPos;

  if(isNewGame){
    fromPos = gtk_entry_get_text(GTK_ENTRY(StartEntry));
    toPos = gtk_entry_get_text(GTK_ENTRY(EndEntry));

    //printf("It is %s", input);
  
    row = fromPos[0] - 97;
	col = 56 - fromPos[1];
    
    endrow = toPos[0] - 97;
	endcol = 56 - toPos[1];  

    if(isValidPieceMove(getPieceAt(col, row), endcol, endrow)){
      movePieceTo(getPieceAt(col, row), col, row, endcol, endrow);
      if(isCheckmate('W')){
      }
    }
    PrintGTKBoard();
    gtk_text_buffer_set_text (boardbuffer, chessText, -1);
    /*
    while(!opponentMoveRecieved){
        text_label = waiting for your opponent to make a move. 
    }
    //socket stuff should go here
    //PrintGTKBoard()
    //gtk_text_buffer_set_text (boardbuffer, chessText, -1);
    text_label = make a move!
  }
  else{
    isNewGame = 1;
    CreateBoard();
    DefaultPieceSetup();
    PrintGTKBoard();
    gtk_text_buffer_set_text (boardbuffer, chessText, -1);
  }
}