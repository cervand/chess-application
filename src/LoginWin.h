/*Header file for messing around with GTK */
/* Declare callback functions here */

#include <gtk/gtk.h>
#include <stdlib.h>
#ifndef LOGINWIN_H
#define LOGINWIN_H
char User[40];

int LoginWin(int argc, char *argv[]);

void destroylogin(GtkWidget *widget, gpointer data);

void LoginToServer(GtkWidget *widget, gpointer data);

void CreateAccount(GtkWidget *widget, gpointer data);
#endif