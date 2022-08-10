/* Messing around with GTK and testing it with Git Hub stuff */
/* Feel free to use this file to test more GTK stuff */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include "LoginWin.h"
#include "Main.h"
#include "ClientWin.h"
struct sockaddr_in ServerAddress;
static GtkWidget *Box;
static GtkWidget *TopTextLabel;
static GtkWidget *LoginButton;
static GtkWidget *CreateAccountButton;          
static GtkWidget *UsernameEntry;
static GtkWidget *PasswordEntry;
static GtkWidget *PasswordRe_EntryLabel;
static GtkWidget *PasswordRe_Entry;
static GtkWidget *window;
static int accountcreated = 0; 
//int sockfd;

int LoginWin(int argc, char *argv[]) {
    /*struct hostent *server;
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
    */
    //GtkWidget *button;
    GtkWidget *TextLabel;
    GtkWidget *Hbox;
    GtkWidget *Hbox2;
    GtkWidget *halign;
    GtkWidget *halign2;
    GtkWidget *companyLogo;
    GtkWidget *appLogo;
    GtkWidget *LogIn;
    GtkWidget *CreateAcc;
    gtk_init (&argc, &argv);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), "Chess Chat");                   //setting window properties
    gtk_window_set_default_size(GTK_WINDOW (window), 300, 150);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect (window, "delete-event", G_CALLBACK (destroylogin), NULL);         //if red x is pressed, window is gone

    Box = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), Box);

    Hbox2 = gtk_hbox_new(TRUE, 0);

    appLogo = gtk_image_new_from_file("src/images/ChessChatLogoSmall.png");
    gtk_box_pack_start(GTK_BOX(Hbox2), appLogo, FALSE, FALSE, 0);
    gtk_widget_show(appLogo);

    companyLogo = gtk_image_new_from_file("src/images/logoSmall.png");
    gtk_box_pack_start(GTK_BOX(Hbox2), companyLogo, FALSE, FALSE, 0);
    gtk_widget_show(companyLogo);

    halign2 = gtk_alignment_new(1, 0, 1, 1);
    gtk_container_add(GTK_CONTAINER(halign2), Hbox2);
    gtk_box_pack_start(GTK_BOX(Box), halign2, FALSE, FALSE, 5);

    TextLabel = gtk_label_new("Username(Max 40 characters, Min 4 characters)");                                 //username label
    gtk_misc_set_alignment(GTK_MISC(TextLabel), 0.0f, 0.0f);
    gtk_box_pack_start(GTK_BOX(Box), TextLabel, TRUE, FALSE, 0);
    gtk_widget_show(TextLabel);

    UsernameEntry = gtk_entry_new();                                //username entry thing
    gtk_editable_set_editable(GTK_EDITABLE(UsernameEntry), TRUE);
    gtk_box_pack_start(GTK_BOX(Box), UsernameEntry, TRUE, TRUE, 0);
    gtk_widget_show(UsernameEntry);

    TextLabel = gtk_label_new("Password(Max 40 characters, Min 4 characters)");                     //password label
    gtk_misc_set_alignment(GTK_MISC(TextLabel), 0.0f, 0.0f);
    gtk_box_pack_start(GTK_BOX(Box), TextLabel, TRUE, FALSE, 0);
    gtk_widget_show(TextLabel);

    PasswordEntry = gtk_entry_new();                        //password entry
    gtk_editable_set_editable(GTK_EDITABLE(PasswordEntry), TRUE);
    gtk_entry_set_visibility(GTK_ENTRY(PasswordEntry), FALSE);
    gtk_box_pack_start(GTK_BOX(Box), PasswordEntry, TRUE, TRUE, 0);
    gtk_widget_show(PasswordEntry);

    PasswordRe_EntryLabel = gtk_label_new("Re-Enter Password(Max 40 characters, Min 4 characters");                //password re-entry label, hidden until create account
    gtk_misc_set_alignment(GTK_MISC(PasswordRe_EntryLabel), 0.0f, 0.0f);
    gtk_box_pack_start(GTK_BOX(Box), PasswordRe_EntryLabel, TRUE, TRUE, 0);

    PasswordRe_Entry = gtk_entry_new();                      //password re-entry textbox, hidden until create account
    gtk_editable_set_editable(GTK_EDITABLE(PasswordRe_Entry), TRUE);
    gtk_entry_set_visibility(GTK_ENTRY(PasswordRe_Entry), FALSE);
    gtk_box_pack_start(GTK_BOX(Box), PasswordRe_Entry, TRUE, TRUE, 0);

    TopTextLabel = gtk_label_new("");                                //status label, displays error messages 
    gtk_misc_set_alignment(GTK_MISC(TopTextLabel), 0.0f, 0.0f);
    gtk_box_pack_start(GTK_BOX(Box), TopTextLabel, TRUE, TRUE, 0);
    gtk_widget_show(TopTextLabel);

   
   
    Hbox = gtk_hbox_new(TRUE, 2);

    CreateAccountButton = gtk_button_new();
    CreateAcc = gtk_image_new_from_file("src/images/CreateAccButton.png");
    gtk_button_set_image (GTK_BUTTON (CreateAccountButton), CreateAcc);
    //gtk_widget_set_size_request(CreateAccountButton, 70, 17);
    g_signal_connect (CreateAccountButton, "clicked", G_CALLBACK (CreateAccount), NULL);        
    gtk_box_pack_start(GTK_BOX (Hbox), CreateAccountButton, FALSE, FALSE, 0);
    gtk_widget_show (CreateAccountButton);

    LoginButton = gtk_button_new();                //Login Button
    LogIn = gtk_image_new_from_file("src/images/LoginButton.png");
    gtk_button_set_image (GTK_BUTTON (LoginButton), LogIn);
    //gtk_widget_set_size_request(LoginButton, 70, 17);
    g_signal_connect (LoginButton, "clicked", G_CALLBACK (LoginToServer), NULL);     
    gtk_box_pack_start(GTK_BOX (Hbox), LoginButton, FALSE, FALSE, 0);                    
    gtk_widget_show (LoginButton);

    halign = gtk_alignment_new(1, 0, 0, 0);
    gtk_container_add(GTK_CONTAINER(halign), Hbox);
    gtk_box_pack_start(GTK_BOX(Box), halign, FALSE, FALSE, 0);

    gtk_widget_show_all (window);
    gtk_widget_hide(PasswordRe_Entry);
    gtk_widget_hide(PasswordRe_EntryLabel);

    gtk_main ();

    return 0;
}//end func main
/*
char *ServerRequest(const char *Message, char *RecvBuf) {                   //Use this function to send any request to the server
    int n;
    n = write(sockfd, Message, strlen(Message));
    if (n < 0) {
        printf("\nSocket Write Error!\n");
    }
    n = read(sockfd, RecvBuf, 700);
    if (n < 0) {
        printf("\nSocket Read Error!\n");
    }
    RecvBuf[n] = 0;
    printf("\nReceived Response: %s\n", RecvBuf);
    return (RecvBuf);
}//end func ServerRequest
*/

void destroylogin(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}//end func destroy

void LoginToServer(GtkWidget *widget, gpointer data) {           //Sends username and pass to server
    char buffer[256];                                                   //server will decide if it's valid then returns success/fail
    char RecvBuf[256];
    char *Response;
    memset(buffer, '\0', sizeof(char) * 256);
    memset(RecvBuf, '\0', sizeof(char) * 256);
    const gchar *username, *password;
    int isValidPassword = 0;
    int isValidUsername = 1; 

    username = gtk_entry_get_text(GTK_ENTRY(UsernameEntry));
    password = gtk_entry_get_text(GTK_ENTRY(PasswordEntry));
    
    
    if (strcmp(username, "") == 0) {
            gtk_label_set_text(GTK_LABEL(TopTextLabel), "Uh-oh. Your username is empty! Try again!");
            isValidUsername = 0;
    }
    if (strcmp(password, "") == 0) {
            gtk_label_set_text(GTK_LABEL(TopTextLabel), "Uh-oh. Your password is empty! Try again!");
            isValidPassword = 0;
    }

    strcat(buffer,"LOGREQ ");
    strcat(buffer, (char *) username);
    strcat(buffer, " ");
    strcat(buffer, (char *) password);

    Response = ServerRequest(buffer, RecvBuf);
    printf("Response: %s\n", Response);
    if (strcmp(Response, "FAILURE") == 0){
        gtk_label_set_text(GTK_LABEL(TopTextLabel), "Your username or password is incorrect. Please try again.");
    }
//debugging to make login failure - taylor 
   /* else if (strcmp(Response, "FAILURE") == -1){
        gtk_label_set_text(GTK_LABEL(TopTextLabel), "User does not exist! Please try again.");
    }*/
    else if(strcmp(Response, "SUCCESS") == 0){
        gtk_label_set_text(GTK_LABEL(TopTextLabel), "You have succesfuly logged in!");
        strcpy(User, (char *) username);
        gtk_widget_hide(window);
        gtk_main_quit();
        //adding functionality to open clientwin after successful click 
/*        g_signal_connect(G_OBJECT(p_button[0]), "clicked", 
        G_CALLBACK(create_window(argc,argv), NULL);
        gtk_box_pack_start(GTK_BOX(p_main_box), p_button[0], FALSE, FALSE, 0);

        gtk_widget_show_all(p_window);*/
    }
//    else{
//	gtk_label_set_text(GTK_LABEL(TopTextLabel), "Oops! Something went wrong with our side of code!");
//    }
   
    printf("\nMessage to Server is: %s\n", buffer);
    printf("\nServer Response is: %s\n", Response);
}//end func logintoserver



/* Sends username and password to server
    - Creates UI for creating an account
    - Checks that password and username are valid
    - Sends data to server after verifying and doing the above
    - Closes */
void CreateAccount(GtkWidget *widget, gpointer data) {      
    const char *username, *password, *re_enteredpassword;
    char message[256], RecvBuff[256];
    char *Response;
    int isValidPassword = 0;
    int isValidUsername = 1;
    memset(message, '\0', sizeof(char)*256);                //clear out the strings and set them to be all null characters
    memset(RecvBuff, '\0', sizeof(char)*256);
    if (accountcreated == 0) {                      //show account creation screen
        gtk_widget_show(PasswordRe_EntryLabel);
        gtk_widget_show(PasswordRe_Entry);
        gtk_widget_hide(LoginButton);
        gtk_label_set_text(GTK_LABEL(TopTextLabel), "Please confirm your password by re-entering it.");
        gtk_entry_set_text(GTK_ENTRY(UsernameEntry), "");
        gtk_entry_set_text(GTK_ENTRY(PasswordEntry), "");
        gtk_entry_set_text(GTK_ENTRY(PasswordRe_Entry), "");
        accountcreated = 1;
    } 
    else if (accountcreated == 1) {              
        username = gtk_entry_get_text(GTK_ENTRY(UsernameEntry));
        password = gtk_entry_get_text(GTK_ENTRY(PasswordEntry));
        re_enteredpassword = gtk_entry_get_text(GTK_ENTRY(PasswordRe_Entry));

        if (strcmp(username, "") == 0) {
            gtk_label_set_text(GTK_LABEL(TopTextLabel), "Uh-oh. Your username is empty! Try again!");
            isValidUsername = 0;
        } else if (strlen(username) > 40) {                     //greater than 40 characters
            gtk_label_set_text(GTK_LABEL(TopTextLabel), "Your username is too long!");
            isValidUsername = 0;
        } else if (strlen(username) < 4) {
            gtk_label_set_text(GTK_LABEL(TopTextLabel), "Your username is too short!");
            isValidUsername = 0;
        }

        if (strlen(password) < 4 || strlen(re_enteredpassword) < 4) {
            gtk_label_set_text(GTK_LABEL(TopTextLabel), "Password too short!");
        } else if (strcmp(password, re_enteredpassword) == 0 && strcmp(password, "") != 0 && strcmp(re_enteredpassword, "") != 0) { 
//            printf("Password and Re-Entered Password are: %s and %s\n", password, re_enteredpassword);
            isValidPassword = 1;
        } else if (strcmp(password, "") == 0) {             
            gtk_label_set_text(GTK_LABEL(TopTextLabel), "Uh-oh. You entered empty text! Try again!");
        } else if (strcmp(password, re_enteredpassword) != 0) {                                   
            gtk_label_set_text(GTK_LABEL(TopTextLabel), "Uh-oh. Your passwords do not match! Try again!");
        } 
    
        if(isValidPassword && isValidUsername){                               
            gtk_widget_hide(PasswordRe_EntryLabel);
            gtk_widget_hide(PasswordRe_Entry);
            gtk_widget_show(LoginButton);

            

            strcpy(message, "CREACC ");
            strcat(message, username);
            strcat(message, " ");
            strcat(message, password);
            message[strlen(message)] = '\0';

            Response = ServerRequest(message, RecvBuff); 

            if (strcmp(Response, "SUCCESS") == 0) {
                printf("Account succesfully created!");
                gtk_label_set_text(GTK_LABEL(TopTextLabel), "You have now created an account! Login to continue");
                accountcreated = 0;
            } 
            else {
                printf("Account was not created");
                gtk_label_set_text(GTK_LABEL(TopTextLabel), "Username already taken!");
            }

            printf("\nMessage to server is: %s\n", message);
            printf("\nServer Response is: %s\n", Response);         
        }
    }
}//end func CreateAccount


