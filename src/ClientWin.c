/* Game Client */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "ClientWin.h"
#include "ChatWin.h"    // need to call to chat with friends to open a new window
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include "Main.h"
#include "LoginWin.h"
struct sockaddr_in ServerAddress;
static GtkWidget *window;
static GtkWidget *table;
static GtkTextBuffer *buffer;
static GtkTextTag *wrap;
static GtkTextTag *justifyL;
static GtkTextIter start, end;
static GtkWidget *MessageLog;
static GtkWidget *scrolled_window;

static GtkTextBuffer *buffer2;
static GtkTextTag *wrap2;
static GtkTextTag *justifyL2;
static GtkTextIter start2, end2;
static GtkWidget *MessageLog2;
static GtkWidget *scrolled_window2;

static GtkWidget *MessageEntry1;
static GtkWidget *MessageEntry2;
static GtkWidget *TextLabel1;
static GtkWidget *TextLabel2;
static GtkWidget *TextLabel3;
static GtkWidget *TextLabel4;
static GtkWidget *button1;
static GtkWidget *button2;
static GtkWidget *button3;
static GtkWidget *button4;
static GtkWidget *button5;

static int argcbuffer;
static char **argvbuffer;

static int chatwindowopen = 0;

int ClientWin(int argc, char *argv[]) {
/*    create_window(argc, *argv[]);
    gtk main();
    return 0;
}*/
//added this to test if i could open the client win after clicking login 
//int create_window(int argc, char *argv[]){
    argcbuffer = argc;                    //callback functions don't take in argc/argv so need to make a global variable to pass them in
    argvbuffer = argv;
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), "Chat and Chess");
    gtk_window_set_default_size(GTK_WINDOW (window), 400, 600);

    table = gtk_table_new (10, 10, 0);
    gtk_container_add (GTK_CONTAINER (window), table);

    buffer = gtk_text_buffer_new(NULL);
    wrap = gtk_text_buffer_create_tag (buffer, "wrapping", "wrap-mode", GTK_WRAP_WORD_CHAR, NULL);
    justifyL = gtk_text_buffer_create_tag(buffer, "leftside", "justification", GTK_JUSTIFY_LEFT, NULL);
    gtk_text_buffer_get_iter_at_offset (buffer, &start, 0);
    gtk_text_buffer_get_iter_at_offset (buffer, &end, -1);
    gtk_text_buffer_apply_tag (buffer, wrap, &start, &end);
    MessageLog = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(MessageLog), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(MessageLog), FALSE);
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (MessageLog), GTK_WRAP_WORD_CHAR); 
    gtk_widget_show(MessageLog);
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(MessageLog), buffer);
    scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add (GTK_CONTAINER (scrolled_window),  MessageLog);
    gtk_container_set_border_width (GTK_CONTAINER (scrolled_window), 5);
    gtk_table_attach(GTK_TABLE(table), scrolled_window, 0, 50, 5, 55, GTK_FILL, GTK_FILL, 0, 0);
    gtk_widget_show(scrolled_window);

    buffer2 = gtk_text_buffer_new(NULL);
    wrap2 = gtk_text_buffer_create_tag (buffer2, "wrapping", "wrap-mode", GTK_WRAP_WORD_CHAR, NULL);
    justifyL2 = gtk_text_buffer_create_tag(buffer2, "leftside", "justification", GTK_JUSTIFY_LEFT, NULL);
    gtk_text_buffer_get_iter_at_offset (buffer2, &start2, 0);
    gtk_text_buffer_get_iter_at_offset (buffer2, &end2, -1);
    gtk_text_buffer_apply_tag (buffer2, wrap2, &start2, &end2);
    MessageLog2 = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(MessageLog2), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(MessageLog2), FALSE);
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (MessageLog2), GTK_WRAP_WORD_CHAR); 
    gtk_widget_show(MessageLog2);
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(MessageLog2), buffer2);

    //gtk_table_attach(GTK_TABLE(table), MessageLog2, 0, 50, 58, 75, GTK_FILL, GTK_FILL, 0, 0);

    scrolled_window2 = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window2), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add (GTK_CONTAINER (scrolled_window2),  MessageLog2);
    gtk_container_set_border_width (GTK_CONTAINER (scrolled_window2), 5);
    gtk_table_attach(GTK_TABLE(table), scrolled_window2, 0, 50, 61, 111, GTK_FILL, GTK_FILL, 0, 0);
    gtk_widget_show(scrolled_window2);
    
    TextLabel1 = gtk_label_new("Online Users:");
    gtk_misc_set_alignment(GTK_MISC(TextLabel1), 0.0f, 0.0f);
    gtk_table_attach(GTK_TABLE(table), TextLabel1, 0, 50, 1, 3, GTK_SHRINK, GTK_SHRINK, 0, 0);
    gtk_widget_show(TextLabel1);

    TextLabel2 = gtk_label_new("Friends:");
    gtk_misc_set_alignment(GTK_MISC(TextLabel2), 0.0f, 0.0f);
    gtk_table_attach(GTK_TABLE(table), TextLabel2, 0, 50, 57, 59, GTK_SHRINK, GTK_SHRINK, 0, 0);
    gtk_widget_show(TextLabel2);

    TextLabel3 = gtk_label_new("Enter a friend's username to add/delete:");
    gtk_misc_set_alignment(GTK_MISC(TextLabel3), 0.0f, 0.0f);
    gtk_table_attach(GTK_TABLE(table), TextLabel3, 0, 50, 113, 115, GTK_SHRINK, GTK_SHRINK, 0, 0);
    gtk_widget_show(TextLabel3);

    MessageEntry1 = gtk_entry_new();
    gtk_editable_set_editable(GTK_EDITABLE(MessageEntry1), TRUE);
    gtk_table_attach(GTK_TABLE(table), MessageEntry1, 0, 30, 117, 119, GTK_FILL, GTK_FILL, 1, 0);
    gtk_widget_show(MessageEntry1);

    button1 = gtk_button_new_with_label ("Add Friend");
    gtk_table_attach(GTK_TABLE(table), button1, 32, 36, 117, 119, GTK_FILL, GTK_SHRINK, 0, 0);
    g_signal_connect(button1,"clicked",G_CALLBACK(Add_Friend),NULL);
    gtk_widget_show(button1);

    button2 = gtk_button_new_with_label ("Delete Friend");
    gtk_table_attach(GTK_TABLE(table), button2, 38, 50, 117, 119, GTK_FILL, GTK_SHRINK, 0, 0);
    g_signal_connect(button2,"clicked",G_CALLBACK(Delete_Friend),NULL);
    gtk_widget_show(button2);

    TextLabel4 = gtk_label_new("Enter a friend's username to chat with:");
    gtk_misc_set_alignment(GTK_MISC(TextLabel4), 0.0f, 0.0f);
    gtk_table_attach(GTK_TABLE(table), TextLabel4, 0, 50, 121, 123, GTK_SHRINK, GTK_SHRINK, 0, 0);
    gtk_widget_show(TextLabel4);

    MessageEntry2 = gtk_entry_new();
    gtk_editable_set_editable(GTK_EDITABLE(MessageEntry2), TRUE);
    gtk_table_attach(GTK_TABLE(table), MessageEntry2, 0, 30, 125, 127, GTK_FILL, GTK_FILL, 1, 0);
    gtk_widget_show(MessageEntry2);

    button3 = gtk_button_new_with_label ("Message a Friend");
    gtk_table_attach(GTK_TABLE(table), button3, 32, 50, 125, 127, GTK_FILL, GTK_SHRINK, 0, 0);
    g_signal_connect(button3,"clicked",G_CALLBACK(Message_Friend),NULL);
    gtk_widget_show(button3);

    button4 = gtk_button_new_with_label ("Account Settings");
    gtk_table_attach(GTK_TABLE(table), button4, 0, 50, 129, 131, GTK_FILL, GTK_SHRINK, 0, 0);
    g_signal_connect(button4,"clicked",G_CALLBACK(Settings),NULL);
    gtk_widget_show(button4);

    button5 = gtk_button_new_with_label ("Log Out (QUIT)");
    gtk_table_attach(GTK_TABLE(table), button5, 0, 50, 133, 135, GTK_FILL, GTK_SHRINK, 0, 0);
    g_signal_connect(button5,"clicked",G_CALLBACK(destroy),NULL);
    gtk_widget_show(button5);

    
    g_signal_connect (window, "delete-event", G_CALLBACK (destroy), NULL);
    gtk_widget_show(table);
    gtk_widget_show_all(window);
    g_timeout_add_seconds(5, Refresh, NULL);
    gtk_main();
    
    return 0;
}

gboolean Refresh(gpointer data) {                      //with timeout_add_seconds, automatically refreshes who is online and friends list
    int showboard = 0;
    int length;
    char *incomingmessage = NULL;
    char *Response, *piece;
    char RecvBuf[1500], users[500], friends[500], copy[250];
    memset(RecvBuf, '\0', sizeof(char) *1500);
    memset(users, '\0', sizeof(char) * 500);
    memset(friends, '\0', sizeof(char) * 500);
    memset(copy, '\0', sizeof(char) * 250);
    Response = ServerRequest("REFRSH", RecvBuf);            //RecvBuf has a string with all users online and all friends online
    piece = strtok(RecvBuf, "|");
    if (piece) {
        strcpy(users, piece);
    }
    piece = strtok(NULL, "|");
    if (piece) {
        strcpy(friends, piece);
    }
    piece = strtok(NULL,"|");
    if (piece) {
        if (strcmp(piece, "CHALLENGE") == 0) {                  //if there's a challenge request
            showboard = 1;
        }
    }
    piece = strtok(NULL, "|");
    if (piece) {
        if (strcmp(piece, "NOMESSAGE") != 0) {                  //if there's a message
            strcpy(incomingmessage, piece);
            length = strlen(incomingmessage);
            incomingmessage[length] = '\0';
            if (chatwindowopen == 1) {                          //if the window is already open then just update the text view
                GtkTextBuffer *buffer3;
                buffer3 = gtk_text_buffer_new(NULL);
                gtk_text_buffer_set_text(GTK_TEXT_BUFFER(buffer3), (const gchar *) incomingmessage, 256);
                gtk_text_view_set_buffer(GTK_TEXT_VIEW(ChatLog), GTK_TEXT_BUFFER(buffer3));
            } else {                                            //else makes a new window 
                ChatWin(argcbuffer, argvbuffer, "USER1", "USER2", showboard, incomingmessage);
            }
         }        
    }
    length = strlen(users);
    users[length] = '\0';
    length = strlen(friends);
    friends[length] = '\0';

    //set the online user textview to the users string and the online friends textview to friends string
    gtk_text_buffer_set_text(buffer, (const gchar *) users, strlen(users));
    gtk_text_buffer_set_text(buffer2, (const gchar *) friends, strlen(friends));
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(MessageLog), buffer);
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(MessageLog2), buffer2);
    return TRUE;
}
void destroy(GtkWidget *widget, gpointer data) {
    
    char *Response;
    char RecvBuf[256];
    Response = ServerRequest("LOGOFF", RecvBuf);
    
    gtk_main_quit();
    // need to send a message to the server to update status
}

void Add_Friend(GtkWidget* widget, gpointer data){
    gtk_text_buffer_insert_at_cursor (buffer, "ADDFRE ", -1);
    gtk_text_buffer_insert_at_cursor (buffer, gtk_entry_get_text(GTK_ENTRY(MessageEntry1)), -1);
    gtk_text_buffer_insert_at_cursor (buffer, "\n", 1);
    
    char SendBuffer[256], RecvBuffer[256];
    char *Response;
    strcpy(SendBuffer, "ADDFRE ");
    strcat(SendBuffer, (char *)gtk_entry_get_text(GTK_ENTRY(MessageEntry1)));
    Response = ServerRequest(SendBuffer, RecvBuffer);
    
    gtk_entry_set_text(GTK_ENTRY(MessageEntry1), "");
}

void Delete_Friend(GtkWidget* widget, gpointer data){
    gtk_text_buffer_insert_at_cursor (buffer2, "DELFRE ", -1);
    gtk_text_buffer_insert_at_cursor (buffer2, gtk_entry_get_text(GTK_ENTRY(MessageEntry1)), -1);
    gtk_text_buffer_insert_at_cursor (buffer2, "\n", 1);
    
    char SendBuffer[256], RecvBuffer[256];
    char *Response;
    strcpy(SendBuffer, "DELFRE ");
    strcat(SendBuffer, (char *)gtk_entry_get_text(GTK_ENTRY(MessageEntry1)));
    Response = ServerRequest(SendBuffer, RecvBuffer);
    
    gtk_entry_set_text(GTK_ENTRY(MessageEntry1), "");
}

void Message_Friend(GtkWidget* widget, gpointer data){
    gtk_text_buffer_insert_at_cursor (buffer, "MESSAG", -1);                                        //open up chat win here?
    gtk_text_buffer_insert_at_cursor (buffer, gtk_entry_get_text(GTK_ENTRY(MessageEntry2)), -1);
    gtk_text_buffer_insert_at_cursor (buffer, "\n", 1);
    const gchar *buffer;
    buffer = gtk_entry_get_text(GTK_ENTRY(MessageEntry2));
    ChatWin(argcbuffer, argvbuffer, "USER1", "USER2", 0, NULL);
    chatwindowopen = 1;
    gtk_entry_set_text(GTK_ENTRY(MessageEntry2), "");
}

void Settings(GtkWidget* widget, gpointer data){
    gtk_text_buffer_insert_at_cursor (buffer, "Account Settings Button Clicked", -1);
    gtk_text_buffer_insert_at_cursor (buffer, "\n", 1);
}

