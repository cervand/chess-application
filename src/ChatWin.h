#include <gtk/gtk.h>
#include <stdlib.h>
#ifndef LOGIN_H
#define LOGIN_H



#define WINDOW_BORDER 10


#define WINDOW_WIDTH  400
#define WINDOW_HEIGHT 700

char MessagedUser[40];       //stores the username of a user you message

GtkWidget* ChatLog;

int ChatWin(int argc, char *argv[], char *user1, char *user2, int showboard, char *initialmessage);

gboolean destroychat(GtkWidget *widget, GdkEvent *event, gpointer data);

gboolean sendmessage(GtkWidget *widget, GdkEvent *event, gpointer data);

gboolean fight(GtkWidget *widget, GdkEvent *event, gpointer data);

gboolean killpopup(GtkWidget *widget, GdkEvent *event, gpointer data);

gboolean chessstart(GtkWidget *widget, GdkEvent *event, gpointer data);

gboolean movemade(GtkWidget *widget, GdkEvent *event, gpointer data);

gboolean receive(GtkWidget *widget, GdkEvent *event, gpointer data);

void decipherMessage(GtkWidget *widget, gpointer data);


#endif