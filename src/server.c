/* A simple server in the internet domain using TCP
 The port number is passed as an argument
 This version runs forever, forking off a separate
 process for each connection
 gcc server2.c -lsocket
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#define BUFFER_SIZE 1000

typedef struct client CLIENT;
typedef struct clients CLIENTLIST;

struct client {             //represents a client logged in
    int Sock;               //Sock is client's socket, used to know who to write to
    char *Username;         //Username is username of clients, used for identification
    int Challenged;         //Challenged is used to indicate that the user is challenged by another dude
    char *IncomingMessage;      //IncommingMessage is a message that is to be sent to the client
    CLIENT *Next;           //linked list shenanigans
    CLIENT *Prev;
    CLIENTLIST *List; 
};

struct clients {            //linked list of clients
    CLIENT *First;
    CLIENT *Last;
    int Length;
}; 

CLIENT *CreateClient() {
    CLIENT *temp = malloc(sizeof(CLIENT));
    temp->Sock = 0;
    temp->Username = NULL;
    temp->Challenged = 0;
    temp->IncomingMessage = NULL;
    temp->Next = NULL;
    temp->Prev = NULL;
    temp->List = NULL;
    return temp;
}

int DeleteClient(CLIENT *client) {
    if (client) {
        client->Sock = 0;
        client->Challenged = 0;
        if (client->Username) {
            free(client->Username);
        }
        client->IncomingMessage = NULL;
        client->Next = NULL;
        client->Prev = NULL;
        client->List = NULL;
        free(client);
        return 0;
    } else {
        return 1;
    }
}
CLIENT *FindClient(CLIENTLIST *clientlist, char *username) {   //given a username, find the client with that username(used for chatting)
    CLIENT *temp = clientlist->First;
    for(int i = 0; i < clientlist->Length; i++) {               //going through the length of the list
        if (strcmp(temp->Username, username) == 0) {            //if the name matches breaks out of loop
            break;
        } else {                                                //else, continues on to next list entry
            temp = temp->Next;
        }
    }
    return temp;                                                //will return null if user not in list
}                   
int RemoveClientFromList(CLIENTLIST *clientlist, char *username) {   //when a user logs off/quits, removes them from the list of online clients
    CLIENT *RemovedClient = FindClient(clientlist, username);
    if (RemovedClient == NULL) {
        printf("\nClient not in list yo\n");
        return 1;
    }
    CLIENT *temp = NULL;
    CLIENT *tempPrev = NULL;
    CLIENT *tempNext = NULL;
    if (RemovedClient == clientlist->First) {
        temp = RemovedClient->Next;                     //2nd entry will become first entry
        DeleteClient(RemovedClient);
        clientlist->First = temp;                       //2nd entry is now the first entry
        temp->Prev = NULL;                              //previous entry of the 2nd entry is removed
        clientlist->Length--;
    } else if (RemovedClient == clientlist->Last) {     //last entry removal
        temp = RemovedClient->Prev; 
        DeleteClient(RemovedClient);
        temp->Next = NULL;
        clientlist->Last = temp;
        clientlist->Length--;
    } else {                                            //somewhere in the middle removal
        tempNext = RemovedClient->Next;                 //Ex want to remove entry 2 in 1 2 3
        tempPrev = RemovedClient->Prev;                 //getting 1 and 3
        DeleteClient(RemovedClient);                    //removing 2
        tempNext->Prev = tempPrev;                      //3's previous is now 1     
        tempPrev->Next = tempNext;                      //1's next is now 3
        clientlist->Length--;
    }
    return 0;
}

int AddClientToList(CLIENTLIST *clientlist, CLIENT *client) {       //adds client to end of linked list
    if(clientlist->Length == 0) {                   //first client case
        clientlist->First = client;
        clientlist->Last = client;
        client->Prev = NULL;
        client->Next = NULL;
        client->List = clientlist;
    } else {
        CLIENT *prevlast = clientlist->Last;                    //previous last entry
        prevlast->Next = client;
        client->Prev = prevlast;
        client->Next = NULL;
        client->List = clientlist;
        clientlist->Last = client;       
    }
    clientlist->Length++;
    return 0;
}
CLIENTLIST *CreateClientList() {
    CLIENTLIST *temp = malloc(sizeof(CLIENTLIST));
    temp->First = NULL;
    temp->Last = NULL;
    temp->Length = 0;
    return temp;
}

int DeleteClientList(CLIENTLIST *clientlist) {
    if (clientlist) {
        CLIENT *temp = clientlist->First;
        CLIENT *tempnext = NULL;
        while(temp != NULL) {                   //goes through each client and frees it
            tempnext = temp->Next;
            DeleteClient(temp);
            temp = tempnext;
        }
        clientlist->Length = 0;
        clientlist->First = NULL;
        clientlist->Last = NULL;
        free(clientlist);
        return 0;
    } else {
        return 1;
    }
}
CLIENTLIST *Clientlist = NULL;

int RequestHandler(CLIENT *client, int sock, struct sockaddr_in cli_addr);

//void dostuff(int); /* function prototype */
void error(char *msg)
{
    perror(msg);
    exit(1);
}

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

/* writes account to database 
 *
 */
int writeToDatabase(char *clientBuffer, char *clientIP, char *portNum){             
    char *username = (getStringAtSection(clientBuffer, 1));         //where username will be stored
    char *password = (getStringAtSection(clientBuffer, 2));        //where password will be stored
 //   strcpy(username, getStringAtSection(clientBuffer, 1));
//    strcpy(password, getStringAtSection(clientBuffer, 2));                  //note, keep in mind that we will have to free these strings
    
    FILE *fp;
  
    char fileName[50];
    strcpy(fileName, "./bin/");
    strcat(fileName, username);
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
    free(username);
    free(password);
    return 1;
}

/* Verifies that the credentials when logging in are correct. Uses codes to figure out results
 *  - (0)  Username and password match. 
 *  - (-1) User not found
 *  - (-2) Wrong credentials
 *  - (-3) Our code is wack and something with the file reading or something happened.
 */
int loginSuccess(char *username, char *password){
    FILE *fp;
    char fileName[50];
    char fileUsername[strlen(username)];
    char filePass[strlen(password)];
    char fileText [200];
    strcpy(fileName, "./bin/");
    strcat(fileName, username);
    strcpy(filePass, password);
    strcat(fileName, ".txt");
    fp = fopen(fileName, "r");
/*    printf("Here is inputusername: %s",username);
    printf("Here is inputpassword: %s",password);

    printf("Here is char username: %s",fileUsername);
    printf("Here is char password: %s",filePass);    
*/ //yay finally got it to print error instead of success
    
    if(fp == NULL) {
       perror("User does not exist!");
       return(-1);
    }

    fgets (fileText, 200, fp);
    
    fclose(fp);

    strcpy(fileUsername, username);
    strcpy(filePass, password);

    printf("The username is %s and the password is '%s'", fileUsername, filePass);

    if( (strcmp(username, fileUsername) == 0) && (strcmp(password, filePass) == 0)){
        return 0;
    }
    return -3;
}

//this returns 1 if input for friend is actually in friends list of user
int friendOrFoe(char *username, char *friend){
    FILE *fp;
    char fileName[50];
    char line[600];
    int i = 0;
    int j = 0;
    int k = 0;
    int numberofstrings;
    int toggle = 0;
    int spaces = 0;
    strcpy(fileName, "./bin/");
    strcat(fileName, username);
    strcat(fileName, ".txt");
    fp = fopen(fileName, "r");

    if (fp == NULL) {
	    perror("Error opening main user database file");
	    return(-1);  
    }
    do {                                //getting the second line of the file, which has a list of all friends
        int c = fgetc(fp);
        if (feof(fp)) {
            break;
        }
        if (c == '\n') {
            toggle = 1;
            continue;
        }
        if (toggle == 0) {
            continue;
        } else if (toggle == 1) {
            line[i] = c;
            i++;
        }
    } while(1);

    fclose(fp);// done w file
    
    for (i = 0; i < (int) strlen(line); i++) {          //getting number of spaces to find out how many friends there are
        if (line[i] == ' ') {
            spaces++;
        }
    }

    char friendslistarray[spaces + 1][40];                  //array of strings with friends

    for (i = 0; i < (int) strlen(line); i++) {
        if (line[i] == ' ') {                               //if there's a space then indicates a different user
            friendslistarray[j][k] = 0;                     //setting last character to null
            j++;                                            //shift to next string in array
            k = 0;                                          //reset index to 0                                         
        } else {
            friendslistarray[j][k] = line[i];        //else if no space, adds the char to the string
            k++;                                            //increment index
        }
    }
    friendslistarray[j][k] = 0;                             //terminate last one's string
    numberofstrings = sizeof(friendslistarray)/(sizeof(friendslistarray[0]));
    for (i = 0; i < numberofstrings; i++) {
        if (strcmp(friend, friendslistarray[i]) == 0) {     //if name matches up in the array, break out and return 1
            return 1;
        }
    }
    return 0;
}

//this function will delete a strint if it matches with the input 
//this helps for when i am deleting someone from the database.txt
void deleteFriend(char * str, const char * toRemove){
    int i,j;
    int found = 1;
    int stringLen = strlen(str);
    int removeLen = strlen(toRemove);

    //search for friend 
    for(i=0;i<= stringLen - removeLen;  i++){
	    for(j=0; j<removeLen; j++){
	        if(str[i + j] != toRemove[j])
            {
                found = 0;
                break;			 
            }
	    }
	//if not the friend
	    if(str[i + j] != ' ' && str[i + j] != '\t' && str[i + j] != '\n' && str[i + j] != '\0') {
            found = 0;
        }
	//if yes friend found
	    if(found == 1)
        {
            for(j=i; j <= stringLen - removeLen; j++)
            {
                str[j] = str[j + removeLen];
            }
            stringLen = stringLen - removeLen;
	        i--;
	    }    
    }

}

int main(int argc, char *argv[])
{
    Clientlist = CreateClientList();
    int sockfd, portno;
    //int pid;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    //char buffer[257];//inc
//    fd_set ActiveFDs;
//    fd_set ReadFDs;
    // int ipAddr, portNo; //variable holder for ip and port no
    if (argc < 2)
    {
        error("ERROR, no port provided\n");
        exit(1);
    }
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR on binding");
        exit(1);
    }
        
    printf("Starting server on Port: %d\n", portno);
    listen(sockfd, 10);
    clilen = sizeof(cli_addr);
    while (1) {
        int newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0) {
            error("ERROR on accepting!");
        }
        //CLIENT *newclient = CreateClient();
        //newclient->Sock = newsockfd;
        //AddClientToList(Clientlist, newclient);
        CLIENT *newclient = CreateClient();
        newclient->Sock = newsockfd;
        if (!fork()) {
            //CLIENT *newclient = CreateClient();
            //newclient->Sock = newsockfd;
            AddClientToList(Clientlist, newclient);
            int temp;
            close(sockfd);
            do {
                temp = RequestHandler(newclient, newsockfd, cli_addr);
            } while (temp == 0);   
        } else {
            close(newsockfd);
        }
    }
    close(sockfd);

    return 0; /* we never get here */
}
char *getUsersAndFriends(CLIENT *client) {                  //gets all online users and gets all friends of client and puts them in a string
    CLIENT *currentclient = Clientlist->First;
    int friendson = 0;                                  //variable used to inidicate if there's any friends online
    char *Message = malloc(sizeof(char) * 1000);
    while (currentclient != NULL) {
        if (currentclient == Clientlist->First) {
            strcpy(Message, currentclient->Username);
        } else {
            strcat(Message, currentclient->Username);
        }
        strcat(Message, "\n");
        currentclient = currentclient->Next;
    }
    currentclient = Clientlist->First;                          //friends part 
    strcat(Message, "|");
    while (currentclient != NULL) {
        if (friendOrFoe(client->Username, currentclient->Username) == 1) {          //if the client is a friend then adds it to the message
            strcat(Message, currentclient->Username);
            strcat(Message, "\n");
            friendson = 1;
        }
        currentclient = currentclient->Next;
    }
    if (friendson == 0) {                   //used to prevent a segfault
        strcat(Message, " ");
    }
//    int length = strlen(Message);
    //Message[length] = '\0';                 //terminating character
    return Message;
}
/* Handles requests and responses communication between client and server */
int RequestHandler(CLIENT *client, int sock, struct sockaddr_in cli_addr)                 
{                                              
    int n;
    char input[1000];        //stores client message
    char output[256];       //message sent back to client(if it's not a success or failure)
    char sPort[30];
    int port;
    char IP[30];
    memset(input, '\0', sizeof(char) *1000);
    memset(output, '\0', sizeof(char) * 256);
    
    n = read(sock, input, 1000);
    
    if (n < 0) {
        error("ERROR reading from socket");
    }

    printf("\nHere is the message before doing anything with it: %s\n", input);


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
            write(sock, "SUCCESS", 8); 
        }
        else{
            write(sock, "FAILURE", 8);
        }
    } else if (strncmp(input, "LOGREQ", 6) == 0) {                      //login request
        char *username = getStringAtSection(input, 1);
        char *password = getStringAtSection(input, 2);
        if(loginSuccess(username, password) == 0 ){
            write(sock, "SUCCESS", 50);
            client->Username = getStringAtSection(input, 1);                //copying username to client struct
            printf("\nUsername: %s\nSocket: %d\n", client->Username, client->Sock);
            printf("we logged in loser, get ready to play chess");
            //printf("%s",getStringAtSection(input, 1));
        } else if (loginSuccess(username, password) == -1 ){
            write(sock, "FAILURE", 50);
            printf("\n user DNE");
        }
        free(username);
        free(password);
    } else if (strncmp(input, "MESSAG", 6) == 0) {                      //Message request to a user
        char *piece;
        char *user1, *user2;
        char *IncomingMessage = NULL;
        piece = strtok(input, "|");
        user1 = getStringAtSection(piece, 1);
        user2 = getStringAtSection(piece, 2);
        piece = strtok(NULL, "|");
        strcpy(IncomingMessage, user1);
        strcat(IncomingMessage, ":");
        strcat(IncomingMessage, piece);
        int length = strlen(IncomingMessage);
        IncomingMessage[length] = '\0';
        CLIENT *UserReceivingMessage = FindClient(Clientlist, user2);
        strcpy(UserReceivingMessage->IncomingMessage, IncomingMessage); 
        free(user1);
        free(user2);
    } else if (strncmp(input, "LOGOFF", 6) == 0) {                      //Log off request, remove from online users, refresh everyone's lists
        n = write(sock, "GOODBYE", 10);                                 //could instead just refresh each user's every 5 seconds or something 
        RemoveClientFromList(Clientlist, client->Username);
        close(sock);
        return 1;
    } else if (strncmp(input, "ADDFRE", 6) == 0) {                      //Add friend 
        char filename[50];
        char *friend = getStringAtSection(input, 1);
        strcpy(filename, "./bin/");
        strcat(filename, client->Username);
        strcat(filename, ".txt");
        FILE *file = fopen(filename, "a");                      //opening file in append mode
        if (file == NULL) {                                     //this should never run                   
            write(sock, "FAILURE", 50);
        } else {                                                //puts the friend's username with a space into the file
            fputs(friend, file);
            fprintf(file, " ");
            fclose(file);
            write(sock, "SUCCESS", 50);
        }
        free(friend);
    } else if (strncmp(input, "DELFRE", 6) == 0) {                     //delete friend :(
        char filename[50];
        char *friend = getStringAtSection(input, 1);
	    strcpy(filename, "./bin/");
	    strcat(filename, client->Username);
	    strcat(filename, ".txt");
	    FILE *file = fopen(filename, "r");
	    FILE *temp = fopen("delete.tmp","w");
	    char buffer[1000];

	    if ((file == NULL) || (temp == NULL)){
	        write(sock, "FAILURE", 50);
	    }
	    else{ 				//search for friend's name in list and remove and write to temp file
    	        while ((fgets(buffer, BUFFER_SIZE, file)) != NULL){
                    deleteFriend(buffer, friend);	//clears file of friend instances 
                    fputs(buffer, temp);		//stores new list without that friend into temp file
                }   
	    }

	    fclose(file);
	    fclose(temp);
	    remove(filename);
	    rename("delete.tmp",filename);			//rename temp file to original 
	    printf("%s was successfully removed from friends.", friend);
    } else if (strncmp(input, "REFRSH", 6) == 0) {                      //Refresh, updates online users, online friends, challenge requests, message requests
        printf("Client: %s\nSocket: %d\n", client->Username, client->Sock);
        char *Message = getUsersAndFriends(client);
        if (client->Challenged == 1) {                                  //on user side need to show the challenged screen 
            strcat(Message, "|CHALLENGE");
        } else {
            strcat(Message, "|NOCHALLENGE");
        }
        if (client->IncomingMessage) {
            strcat(Message,"|");
            strcat(Message, client->IncomingMessage);
        } else {
            strcat(Message, "|NOMESSAGE");
        }
        int length = strlen(Message);
        Message[length] = '\0';
        n = write(sock, Message, 1000);                                     
        free(Message);
        client->IncomingMessage = NULL;
    } else if (strncmp(input, "CHSMVE", 6) == 0) {                      //chess move

    } else if (strncmp(input, "CHALLE", 6) == 0) {                      //do you want to have a tossle m8
        char *username = getStringAtSection(input, 1);                  //getting user name of opponent
        CLIENT *opponent = FindClient(Clientlist, username);            //Finding the opponent in list
        opponent->Challenged = 1;                                       //setting the enemy as challenged, upon next check with server will prompt user
    } else if (strncmp(input, "DECCHA", 6) == 0) {                      //user declined challenge
        client->Challenged = 0;
    } else if (strncmp(input, "ACCCHA", 6) == 0) {                      //user accepted challenge
        client->Challenged = 0;
    } else if (strncmp(input, "CHGPAS", 6) == 0) {                      //change password

    } else if (strncmp(input, "GAMECH", 6) == 0) {
        char *user1 = getStringAtSection(input, 1);
        char *user2 = getStringAtSection(input, 2);
        printf("\n%s has challenged %s!\n", user1, user2);
        n = write(sock, "SUCCESS", 8);
    } else {
        printf("\nWtf was just sent to the server\n");
    }

    
    printf("Here is the message after doing anything with it: %s\n", input);
    return 0;
}//end func RequestHandler
 
