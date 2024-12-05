#include <stdio.h>
#include <string.h>
#include "../lib/message.h"
#include "../lib/function.h"

int fieldCount;
char datafields[][50];
int search_movie_by_title(int socketfd){
    
    char* title;
    printf("Enter movie title: ");
    scanf("%s", title);
    
    char* signal = get_string_from_signal(SIGNUP);
    char* datafield[] = {signal, title};
    int msg_len = sizeof(datafield) / sizeof(datafield[0]);
    char* message = concatenate_strings(datafield, msg_len);
    // end the message with \0
    
    sendStr(socketfd, message);
    char* response;
    int n = recvStr(socketfd, response);
    if (n > 0){
        parse_message(response, datafields, &fieldCount);
        char* signal = datafields[0];
        return get_signal_from_string(signal);
    }
    else return -1;
}