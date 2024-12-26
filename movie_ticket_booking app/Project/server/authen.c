#include <stdio.h>
#include <string.h>
#include "../lib/message.h"
#include "../lib/function.h"
#include "database/sqlite/sqlite3.h"
#define MAXLINE 4096




int sign_up(sqlite3 *db, int socketfd, char username[], char password[]) {
    
    char *errMsg = 0;
    char sql[256];
    // printf("Sign up: %s %s", username, password);
    // Insert the user into the database
    snprintf(sql, sizeof(sql), "INSERT INTO users (username, password) VALUES ('%s', '%s');", username, password);

    if (sqlite3_exec(db, sql, 0, 0, &errMsg) != SQLITE_OK) {
        if (strstr(errMsg, "UNIQUE constraint failed")) {
            printf("Error: Username already exists.\n");
            return FAILURE;
        } 
        else {
            fprintf(stderr, "SQL error: %s\n", errMsg);
        }
        sqlite3_free(errMsg);
    }

    else {
        return SUCCESS;
    }
}

int log_in(sqlite3 *db, int socketfd, char username[], char password[]) {
    
    sqlite3_stmt *stmt;
    if (sqlite3_open("users.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Could not open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }
    const char *sql = "SELECT password FROM users WHERE username = ?;";
    // Prepare SQL statement
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return FAILURE;
    }
    // Bind the username parameter
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    // Execute the query
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        // printf("Username: %s \n", username);
        const char *storedPassword = (const char *)sqlite3_column_text(stmt, 0);
        if (strcmp(storedPassword, password) == 0) {
            return SUCCESS;
        } 
        else {
            return FAILURE;
        }
    } 
    else {
        printf("Username not found \n");
        return USERNOTFOUND;
    }
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}