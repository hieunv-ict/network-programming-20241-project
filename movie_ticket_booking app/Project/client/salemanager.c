#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "../lib/message.h"
#include "../lib/function.h"
#include "movie_function.h"

void add_new_movies(int socketfd, int num_movies) {
    for(int i=0; i<num_movies; i++) {

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
        case 2:
        case 3:
        case 4:

        default:
            break;
    }
}