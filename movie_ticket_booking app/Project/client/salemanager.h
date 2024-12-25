#ifndef SALEMANAGER_H
#define SALEMANAGER_H
#include "../lib/message.h"

void add_new_movies(int socketfd, int num_movies);
void announce_new_showtimes(int socketfd, int num_showings);
void update_screenings_in4(int socketfd);
void salemanager_menu(int socketfd);

#endif