#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "../lib/message.h"
#include "../lib/function.h"
#include "movie_function.h"
#define MAXLINE 1024
void change_user_role(int socketfd){

    char* username;
    char temp[128];
    int role = 0;
    char* str_role;
    char response[MAXLINE];
    char fields[1][128];

    printf("Change user role.\n 1 to process. 2 to back: ");
    int choice = 0;
    scanf("%d", &choice);
    if (choice == 2){
        return;
    }
    printf("Enter username: ");
    scanf("%s", temp);
    username = temp;
    char* roles[3] = {"Ticket Buyer", "Sale Manager", "Admin"};
    printf("Enter role to change. 1 for ticket buyer, 2 for sale manager, 3 for admin: ");
    scanf("%d",&role);
    str_role = roles[role-1];

    char* signal = get_string_from_signal(CHANGEROLE);
    char* datafield[] = {signal, username, str_role};
    int msg_len = sizeof(datafield) / sizeof(datafield[0]);
    char* message = concatenate_strings(datafield, msg_len);

    sendStr(socketfd, message);
    // receive server response
    recvStr(socketfd, response);
    int response_cnt = 0;
    parse_message(response, fields, &response_cnt);
    printf("Response: %s \n", fields[0]);

}