#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/message.h"
#include "../lib/function.h"

#define MAXLINE 4096

int logIn(int socketfd, char *uname)
{
    char passwd[MAXLINE];
    int state;

    printf("Account: ");
    scanf("%s", uname); 
    printf("Password: "); 
    scanf("%s", passwd);
    // concatenate all data fields into a message with correct format
    // convert signal of enum type to string
    char* signal = get_string_from_signal(LOGIN);
    char* datafield[] = {signal, uname, passwd};
    int msg_len = sizeof(datafield) / sizeof(datafield[0]);
    char* message = concatenate_strings(datafield, msg_len);
    
    sendStr(socketfd, message);
    // receive server response
    state = recvInt(socketfd);
    return state;
}

int signup(int socketfd){
    char username[MAXLINE];
    char passwd[MAXLINE];
    int state;

    printf("Account: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", passwd);
    // copy username
    // strcpy(uname, username);
    // concatenate all data fields into a message with correct format
    // convert signal of enum type to string
    char* signal = get_string_from_signal(SIGNUP);
    char* datafield[] = {signal, username, passwd};
    int msg_len = sizeof(datafield) / sizeof(datafield[0]);
    char* message = concatenate_strings(datafield, msg_len);
    
    sendStr(socketfd, message);
    // receive server response
    state = recvInt(socketfd);
    return state;
}