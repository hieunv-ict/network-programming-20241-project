#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/message.h"
#include "../lib/function.h"


#define MAXLINE 1028
int fieldCount;
char datafields[][50];

void parse_movie_list();
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
        parse_movie_list();
        return get_signal_from_string(signal);
    }
    else return -1;
}

void parse_movie_list(){
    int index = 1;
    printf("Movie list: \n");
    while (strcmp(datafields[index], "") != 0){
        printf("%s. %s \n",datafields[index], datafields[index+1]);
        index+=2;
    }
}

int browse_movie(int socketfd, int category){
    char* genres[5] = {"Romance", "Action", "Sci-fi", "Fantasy", "Dramatic"};
    char* signal = get_string_from_signal(BROWSE);
    char category_type[128] = "";
    char category_value[128] = "";
    int msg_len = 3;
    if (category == 1){
        // print list of available genres
        printf("List of movie genres: \n");
        for (int i = 0; i < 5; i++){
            printf("%d. %s \n", i+1, genres[i]);
        }
        // allow user to select genre
        int choice = 0;
        do {
            printf("Enter 1 movie genre you want to browse: ");
        scanf("%d", &choice);
        }while(choice < 1 || choice > 5);
        
        //char* genre = "";
        strcpy(category_type, "Genre");
        strcpy(category_value, genres[choice-1]);
        
    }
    else if (category == 2){
        printf("Cinema list: \n");

    }
    else if (category == 3){
        printf("Showtime list: \n");
    }

    // send message to server
    char* datafield[] = {signal, category_type, category_value};
    char* message = concatenate_strings(datafield, msg_len);

    sendStr(socketfd, message);
    char response[MAXLINE];
    int n = recvStr(socketfd, response);
    printf("%s \n", response);
    if (n < 0){
        printf("Error \n");
        return BROWSENOTFOUND;
    }
    else{
        return BROWSEFOUND;
    }
}

void get_cinema_list(){

}

void get_showtime_list(){

}