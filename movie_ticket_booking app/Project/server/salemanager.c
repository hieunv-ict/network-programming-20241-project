#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "../lib/message.h"
#include "../lib/function.h"
#include "database_helper.h"
#include "database/sqlite/sqlite3.h"
#define DBNAME "users.db"
#define MAXLINE 2048

int add_new_movie(int socketfd, sqlite3* db, char title[], char category[], char director[], char duration[]) {
    char *errMsg = 0;
    char sql[256], director_id[10]; 
    sqlite3_stmt* stmt;
    // printf("Sign up: %s %s", username, password);
    // Insert the user into the database
    // Check if the director exists and get their ID
    const char *check_director_sql = "SELECT Director_id FROM Director WHERE Name = ?;";
    if (sqlite3_prepare_v2(db, check_director_sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return FAILURE;
    }

    // Bind the director's name to the query
    sqlite3_bind_text(stmt, 1, director, -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // Director exists; retrieve their ID
        snprintf(director_id, sizeof(director_id), "%d", sqlite3_column_int(stmt, 0));
    } else if (rc == SQLITE_DONE) {
        // Director doesn't exist; insert them
        sqlite3_finalize(stmt);

        const char *insert_director_sql = "INSERT INTO Director (Name) VALUES (?);";
        if (sqlite3_prepare_v2(db, insert_director_sql, -1, &stmt, NULL) != SQLITE_OK) {
            fprintf(stderr, "Failed to prepare insert statement: %s\n", sqlite3_errmsg(db));
            return FAILURE;
        }

        // Bind the director's name to the insert query
        sqlite3_bind_text(stmt, 1, director, -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            fprintf(stderr, "Failed to insert new director: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return FAILURE;
        }

        sqlite3_finalize(stmt);

        // Retrieve the new director's ID
        const char *get_last_id_sql = "SELECT last_insert_rowid();";
        if (sqlite3_prepare_v2(db, get_last_id_sql, -1, &stmt, NULL) != SQLITE_OK) {
            fprintf(stderr, "Failed to prepare last ID query: %s\n", sqlite3_errmsg(db));
            return FAILURE;
        }

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            snprintf(director_id, sizeof(director_id), "%d", sqlite3_column_int(stmt, 0));
        } else {
            fprintf(stderr, "Failed to retrieve new director ID: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return FAILURE;
        }
    } else {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return FAILURE;
    }

    sqlite3_finalize(stmt);

    snprintf(sql, sizeof(sql), "INSERT INTO Movie (Director_id, Title, Category, Duration) VALUES ('%s', '%s', '%s', '%s');", director, title, category, duration);

    // char sql2[] = "SELECT Name from Director;";

    if (sqlite3_exec(db, sql, 0, 0, &errMsg) != SQLITE_OK) {
        if (strstr(errMsg, "UNIQUE constraint failed")) {
            printf("Error: Movie already exists.\n");
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

int is_director_exists(sqlite3 *db, const char *director_name) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT COUNT(*) FROM Director WHERE Name = ?;";
    int exists = 0;  // Flag to indicate if the director exists

    // Prepare the SQL statement
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return -1; // Error code for failure
    }

    // Bind the director name to the SQL statement
    sqlite3_bind_text(stmt, 1, director_name, -1, SQLITE_STATIC);

    // Execute the SQL query
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        exists = sqlite3_column_int(stmt, 0); // Check the count value
    } else {
        fprintf(stderr, "Failed to execute query: %s\n", sqlite3_errmsg(db));
    }

    // Clean up
    sqlite3_finalize(stmt);

    return exists;
}