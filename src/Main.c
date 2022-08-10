/*Main Program */


#include "Main.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include "ChatWin.h"
#include "LoginWin.h"
#include "ClientWin.h"

static int portno;
static struct sockaddr_in ServerAddress;

int main(int argc, char *argv[]) {
    struct hostent *server;
    if (argc < 3) {
        printf("\nUsage: %s hostname port\n", argv[0]);
        return 0;
    }
    portno = atoi(argv[2]);                     //establishing port and socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
       printf("\nSocket Error!\n");
        return 0;
    }
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        printf("\nHost Error!\n");
        return 0;
    }
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_port = htons(portno);
    ServerAddress.sin_addr = *(struct in_addr*)server->h_addr_list[0];

    if (connect(sockfd, (struct sockaddr*) &ServerAddress, sizeof(struct sockaddr_in)) < 0) {
        printf("\nServer Connection Error!\n");
    }
    LoginWin(argc, argv);
    ClientWin(argc, argv);
    //ChatWin(argc, argv, "a", "e", 1, "nice");
    return 0;
}

char *ServerRequest(const char *Message, char *RecvBuf) {                   //Use this function to send any request to the server
    int n;
    n = write(sockfd, Message, (int) strlen(Message));
    if (n < 0) {
        printf("\nSocket Write Error!\n");
    }
    n = read(sockfd, RecvBuf, 1000);
    if (n < 0) {
        printf("\nSocket Read Error!\n");
    }
    RecvBuf[n] = 0;
    printf("\nReceived Response: %s\n", RecvBuf);
    return (RecvBuf);
}//end func ServerRequest
