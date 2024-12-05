#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/message.h"
#include "../lib/function.h"

#define MAXLINE 1028
int fieldCount;
char datafields[][50];
int search_movie_by_title(int socketfd){
    
    char title[MAXLINE];
    printf("Enter movie title: ");
    scanf("%s", title);
    char response[MAXLINE];
    char* signal = get_string_from_signal(SEARCH);
    char* datafield[] = {signal, title};
    int msg_len = sizeof(datafield) / sizeof(datafield[0]);
    char* message = concatenate_strings(datafield, msg_len);

    sendStr(socketfd, message);
    int n = recvStr(socketfd, response);
    printf("%s \n", response);
    if (n > 0){
        parse_message(response, datafields, &fieldCount);
        char* signal = datafields[0];
        return get_signal_from_string(signal);
    }
    else return -1;
}