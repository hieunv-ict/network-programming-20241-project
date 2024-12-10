#ifndef MOVIEFUNC_H
#define MOVIEFUNC_H
#include "database/sqlite/sqlite3.h"
void send_movie_list(sqlite3* db, int socketfd, char title[]);
#endif