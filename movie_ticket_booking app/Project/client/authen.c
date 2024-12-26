#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/message.h"
#include "../lib/function.h"

#define MAXLINE 4096

int logIn(int socketfd, char *uname, char role[])
{
    char passwd[MAXLINE];
    char response[MAXLINE];
    char fields[2][128];

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
    recvStr(socketfd, response);
    int response_cnt = 0;
    parse_message(response, fields, &response_cnt);
    strcpy(role, fields[1]);
    return get_signal_from_string(fields[0]);
}

int signup(int socketfd){
    char username[MAXLINE];
    char passwd[MAXLINE];
    char role[MAXLINE];
    int role_choice;
    int state;

    printf("Account: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", passwd);
    printf("\n1. Ticket Buyer\n2. Sale Manager\n3. Admin\nChoose a role: ");
    scanf("%d", &role_choice);
    // copy username
<<<<<<< HEAD
    uname = username;
=======
    // strcpy(uname, username);
>>>>>>> main
    // concatenate all data fields into a message with correct format
    // convert signal of enum type to string
    if (role_choice==1) strcpy(role, "Ticket Buyer");
    else if (role_choice==2) strcpy(role, "Sale Manager");
    else strcpy(role, "Admin");
    char* signal = get_string_from_signal(SIGNUP);
    char* datafield[] = {signal, username, passwd, role};
    int msg_len = sizeof(datafield) / sizeof(datafield[0]);
    char* message = concatenate_strings(datafield, msg_len);
    
    sendStr(socketfd, message);
    // receive server response
    state = recvInt(socketfd);
    return state;
}
