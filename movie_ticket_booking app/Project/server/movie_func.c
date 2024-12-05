#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/message.h"
#include "../lib/function.h"

#define MAXLINE 1028


void send_movie_list(int socketfd, char* title){
    //query database to get result
    // no result => return not found
    // has result => return search found
    int fieldCount;
    
    int len = 1;
    char* datafields[len+1];
    char* movie_id[len];
    movie_id[0] = "movie1";
    
    if (len > 0){
        char* signal = get_string_from_signal(SEARCHFOUND);
        datafields[0] = signal;
        for (int i = 1; i < len+1; i++){
            datafields[i] = movie_id[i-1];
        }
        
        int msg_len = len+1;
        char* message = concatenate_strings(datafields, msg_len);
        
        sendStr(socketfd, message);
        printf("Respone: %s \n", message);
    }
    else{
        char* signal = get_string_from_signal(SEARCHNOTFOUND);
        sendStr(socketfd, signal);
    }
    

}