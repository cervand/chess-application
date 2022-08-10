/* main.c header file */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include "LoginWin.h"
#include "ClientWin.h"
#include "ChatWin.h"
#include "Board.h"
#include "Square.h"
#include "Piece.h"


#ifndef MAIN_H
#define MAIN_H

int sockfd;

char *ServerRequest(const char *Message, char *RecvBuf);
#endif