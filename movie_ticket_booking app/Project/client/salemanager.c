#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "../lib/message.h"
#include "../lib/function.h"
#include "movie_function.h"
#include "booking.h"
#define MAXLINE 1024

void add_new_movies(int socketfd, int num_movies) {
    for(int i=0; i<num_movies; i++) {
        char title[MAXLINE], category[MAXLINE], director[MAXLINE], duration[MAXLINE];
        printf("Enter title: "); scanf("\n%[^\n]", title);
        printf("Enter category: "); scanf("\n%[^\n]", category);
        printf("Enter director: "); scanf("\n%[^\n]", director);
        printf("Enter duration: "); scanf("\n%[^\n]", duration);
        char* signal = get_string_from_signal(ADD);
        char* fields[] = {signal, title, category, director, duration};
        char* message = concatenate_strings(fields, 5);
        char result[100][50];
        printf("%s message", message);
        int re = get_response_list(socketfd, fields, 5, result);
    }
}

void announce_new_showtimes(int socketfd, int num_showings) {
    for(int i=0; i<num_showings; i++) {

    }
}

void update_screenings_in4(int socketfd) {

}

void salemanager_menu(int socketfd) {
    int choice;
    do {
        printf("\n Sale manager menu:\n1. Add new movies\n");
        printf("2. Announce new showtimes for movies\n");
        printf("3. Update screening information\n");
        printf("4. See all movies\n");
        printf("5. Back\nEnter your choice: ");
        scanf("%d", &choice);
    } while(choice > 5 || choice < 1);
    switch(choice) {
        case 1:
            int num_movs;
            printf("Enter number of movies to add: "); scanf("%d", &num_movs);
            add_new_movies(socketfd, num_movs);
            break;
        case 2:
        case 3:
        case 4:

        default:
            break;
    }
}