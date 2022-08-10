#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <assert.h>
#include <string.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

void RequestHandler(int sock, struct sockaddr_in cli_addr);

char * getStringAtSection(char *message, int section) {         //ex. LOGREQ User Password, User is section 1, password is section 2
    int i, m;
    int j = 0;
    int k = 0;
    char sectionStr[257];//inc
    for (i = 0; i < strlen(message)/sizeof(char); i++) {
        if (message[i] == ' ') {            //if there's a space
            if (j == section - 1) {
                for (m = 1; m < strlen(message); m++) {
                    sectionStr[k] = message[i + m];                //copying the section to the section variable using a for loop
                    k++;
                    if (message[i + m + 1] == ' ') {            //if the next character is a space then break out, done copying
                        break;
                    }
                }
                sectionStr[k] = '\0';                              //terminating character
                break;
            }
            j++;
        }
    }
    char *str_to_ret = malloc(sizeof(char)*257);
    strcpy(str_to_ret, sectionStr);
    strcat(str_to_ret, "\0");
    return str_to_ret;
}

int writeToDatabase(char *clientBuffer, char *clientIP, char *portNum){
    char *username = malloc( sizeof(getStringAtSection(clientBuffer, 1)));         //where username will be stored
    char *password = malloc( sizeof(getStringAtSection(clientBuffer, 2)));        //where password will be stored
    strcpy(username, getStringAtSection(clientBuffer, 1));
    strcpy(password, getStringAtSection(clientBuffer, 2));
    
    FILE *fp;
    
    printf("writetodatabaseran");
    
    strcpy(username, getStringAtSection(clientBuffer, 1));//copying username to var
    strcpy(password, getStringAtSection(clientBuffer, 2));//copying password to var
    
    char fileName[40];
    
    strcpy(fileName, username);
    strcat(fileName, ".txt");

    fp = fopen(fileName, "w+");

    fputs("USERINFORMATION: ", fp);
    fputs(username, fp);
    fprintf(fp," ");
    fputs(password, fp);
    fprintf(fp," ");
    fputs(clientIP, fp);
    fprintf(fp," ");
    fputs(portNum, fp);
    fprintf(fp,"\n");

    //if for whatever reason, it can't write to the database, RETURN 0!

    fclose(fp);
    return 1;
}

int loginSuccess(char *username, char *password){
    FILE *fp;
    char fileName[40];
    char fileUsername[strlen(username)];
    char filePass[strlen(password)];
    char fileText [200];
    
    strcpy(fileName, username);
    strcat(fileName, ".txt");
    fp = fopen(fileName, "r");
    
    if(fp == NULL) {
       perror("User does not exist!");
       return(-1);
    }

    fgets (fileText, 200, fp);
    
    fclose(fp);

    strcpy(fileUsername, getStringAtSection(fileText, 1));
    strcpy(filePass, getStringAtSection(fileText, 2));

    printf("The username is %s and the password is '%s'", fileUsername, filePass);

    if( (strcmp(username, fileUsername) == 0) && (strcmp(password, filePass))){
        return 0;
    }
    return -3;
}

int main(int argc, char *argv[]) {
    int sockfd, newsock, portno, temp, i;
    //int pid;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    //char buffer[257];//inc
    // int ipAddr, portNo; //variable holder for ip and port no
    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    while(1) {
        newsock = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsock < 0) {
            error("ERROR on accept");
        }

        if (!fork()) {
            while(i < 5) {
                printf("\nNewsock: %d\n", newsock);
                RequestHandler(newsock, cli_addr);
                i++;
            }
            
        }
    }
}


void RequestHandler(int sock, struct sockaddr_in cli_addr)                 
{                                              
    int n;
    char input[256];        //stores client message
    char output[256];       //message sent back to client
    char sPort[30];
    int port;
    char IP[30];
    
    memset(input, '\0', sizeof(char) *256);
    memset(output, '\0', sizeof(char) * 256);
    
    n = read(sock, input, 255);
    
    if (n < 0) {
        error("ERROR reading from socket");
    }

    printf("Here is the message before doing anything with it: %s\n", input);




    //Create account request
    if (strncmp(input, "CREACC", 6) == 0) {
        strncpy(IP, inet_ntoa(cli_addr.sin_addr), INET_ADDRSTRLEN); //copies IP Address to save it
        port = ntohs(cli_addr.sin_port); //variable for port
        strcat(input, " ");
        strcat(input, IP);
        strcat(input, " ");
        sprintf(sPort, "%d", port);
        strcat(input, sPort);

        //when login stuff is imple
        if(writeToDatabase(input, IP, sPort)){
            write(sock, "SUCCESS", 100); 
        }
        else{
            write(sock, "FAILURE", 100);
        }
    }


    //login request
    if (strncmp(input, "LOGREQ", 6) == 0) {
        if(loginSuccess(getStringAtSection(input, 1), getStringAtSection(input, 2))){
            write(sock, "SUCCESS", 50);
            printf("we logged in loser, get ready to play chess");
        }
        else{
            write(sock, "FAILURE", 100);
            printf("lmao you can't get this to work");
        }

    }
    printf("Here is the message after doing anything with it: %s\n", input);
}//end func RequestHandler
 
