#ifndef SERVERCINEMA_H
#define SERVERCINEMA_H
#include "../lib/message.h"
#include "database/sqlite/sqlite3.h"
void send_all_movie(int socketfd, sqlite3* db);
void send_cinema_list(int socketfd, sqlite3* db, char* movie_id);
void send_showtime_list(int socketfd, sqlite3* db, char* movie_id, char* cinema_id);
#endif