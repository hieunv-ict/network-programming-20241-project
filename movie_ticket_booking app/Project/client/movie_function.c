#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/message.h"
#include "../lib/function.h"


#define MAXLINE 1028
int fieldCount;


void parse_movie_list(char datafields[][128]);

int view_movie_detail(int socketfd){
    char datafields[100][128];
    char id[16];
    printf("Enter movie id: ");
    scanf("\n%[^\n]", id);
    char response[MAXLINE];
    char* signal = get_string_from_signal(DETAILS);
    char* datafield[2] = {signal, id};
    char* message = concatenate_strings(datafield, 2);
    sendStr(socketfd, message);
    int n = recvStr(socketfd, response);
    if (n > 0){
        parse_message(response, datafields, &fieldCount);
        char* signal = datafields[0];

        printf("\n****Movie Details****\n");
        printf("Id: %s \n",datafields[1]);
        printf("Title: %s \n",datafields[2]);
        printf("Director: %s \n",datafields[3]);
        printf("Category: %s \n",datafields[4]);
        printf("Duration: %s minutes\n",datafields[5]);
        printf("\n*********************\n");

        return get_signal_from_string(signal);
    }
}
int search_movie_by_title(int socketfd){
    char datafields[100][128];
    char title[128];
    printf("Enter movie title: ");
    scanf("\n%[^\n]", title);
    char response[MAXLINE];
    char* signal = get_string_from_signal(SEARCH);
    char* datafield[] = {signal, title};
    int msg_len = sizeof(datafield) / sizeof(datafield[0]);
    char* message = concatenate_strings(datafield, msg_len);

    sendStr(socketfd, message);
    int n = recvStr(socketfd, response);
    // printf("%s \n", response);
    //printf("%s \n", response);
    if (n > 0){
        parse_message(response, datafields, &fieldCount);
        char* signal = datafields[0];
        parse_movie_list(datafields);
        return get_signal_from_string(signal);
    }
    else return -1;
}

void parse_movie_list(char datafields[][128]){
    int index = 1;
    while (strcmp(datafields[index], "") != 0){
        printf("ID: %s - %s \n",datafields[index], datafields[index+1]);
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
        char cinema_name_input[1024];
        printf("Enter a cinema name: ");
        // fflush(stdin);
        scanf("\n%[^\n]", cinema_name_input);

        strcpy(category_type, "Cinema");
        strcpy(category_value, cinema_name_input);
    }
    else if (category == 3){
        char showtime[20];
        printf("Enter a specific showtime: ");
        scanf("\n%[^\n]", showtime);

        strcpy(category_type, "Showtime");
        strcpy(category_value, showtime);
    }

    // send message to server
    char* datafield[] = {signal, category_type, category_value};
    char* message = concatenate_strings(datafield, msg_len);

    sendStr(socketfd, message); 
    char response[MAXLINE];
    recvStr(socketfd, response);
    // printf("%s \n", response);
    char datafields[100][128];
    memset(datafields, '\0', sizeof(datafields));
    parse_message(response, datafields, &fieldCount);
    parse_movie_list(datafields);

    if (strcmp(response, "BROWSENOTFOUND") == 0){
        // printf("Error \n");
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