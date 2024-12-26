#ifndef DATABASEHELPER_H
#define DATABASEHELPER_H
#include "../lib/message.h"
#include "database/sqlite/sqlite3.h"

int insert_record(sqlite3* db, const char *query, char* values[], int num_of_values);
void insert_record_2(sqlite3 *db, const char *query, char* values[], int num_of_values);
#endif