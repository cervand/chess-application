/* Header File for GameClient.c */

#include <gtk/gtk.h>

#ifndef CLIENTWIN_H
#define CLIENTWIN_H

int ClientWin(int argc, char *argv[]);
gboolean Refresh(gpointer data);
void destroy(GtkWidget *widget, gpointer data);
void Add_Friend(GtkWidget* widget, gpointer data);
void Delete_Friend(GtkWidget* widget, gpointer data);
void Message_Friend(GtkWidget* widget, gpointer data);
void Settings(GtkWidget* widget, gpointer data);
int create_window(int argc,char *argv[]);
#endif
