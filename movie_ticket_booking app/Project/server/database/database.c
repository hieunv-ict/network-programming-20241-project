#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#define DB_NAME "users.db"
// Initialize the database and create the users table if it doesn't exist
sqlite3* initializeDatabase() {
    sqlite3 *db;
    char *errMsg = 0;
    const char *sql = "CREATE TABLE IF NOT EXISTS users (" \
                      "id INTEGER PRIMARY KEY AUTOINCREMENT, " \
                      "username TEXT UNIQUE NOT NULL, " \
                      "password TEXT NOT NULL);";

    if (sqlite3_open(DB_NAME, &db) != SQLITE_OK) {
        fprintf(stderr, "Could not open database: %s\n", sqlite3_errmsg(db));
        exit(1);
    }
    else{
        fprintf(stderr, "Opened database successfully\n");
    }

    if (sqlite3_exec(db, sql, 0, 0, &errMsg) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }

    sqlite3_close(db);
    return db;
}