#ifndef AUTHEN_H
#define AUTHEN_H
#include "../lib/message.h"
#include "database/sqlite/sqlite3.h"

//void logIn(int socketfd);
int sign_up(sqlite3 *db, int socketfd, char username[], char password[], char role[]);
int log_in(sqlite3 *db, int socketfd, char username[], char password[], char role[]);
#endif
