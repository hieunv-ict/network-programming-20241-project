#ifndef ADMINSERVER_H
#define ADMINSERVER_H
#include "../lib/message.h"
#include "database/sqlite/sqlite3.h"
void update_user_role(int socketfd, sqlite3* db, char* username, char* role);
#endif