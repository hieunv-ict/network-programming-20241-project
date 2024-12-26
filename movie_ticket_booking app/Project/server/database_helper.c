#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "../lib/message.h"
#include "../lib/function.h"

#include "database/sqlite/sqlite3.h"
#define DBNAME "users.db"
#define MAXLINE 2048



int insert_record(sqlite3* db, const char *query, const char *values[], int value_count) {
    sqlite3_stmt *stmt;
    int rc;
    rc = sqlite3_open(DBNAME, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    // Begin transaction and set WAL mode
    
    sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
    sqlite3_exec(db, "PRAGMA journal_mode = WAL;", NULL, NULL, NULL);
    // Prepare the SQL statement
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    // Bind values to the placeholders
    for (int i = 0; i < value_count; i++) {
        rc = sqlite3_bind_text(stmt, i + 1, values[i], -1, SQLITE_STATIC);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Failed to bind value %d: %s\n", i + 1, sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return rc;
        }
    }

    // Execute the statement
    
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return rc;
    }

    // Finalize the statement
    sqlite3_finalize(stmt);
    // Commit the transaction
    rc = sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
    fprintf(stderr, "Commit failed: %s\n", sqlite3_errmsg(db));
    return rc;
    }
    
    sqlite3_close(db);
    printf("Record inserted successfully.\n");
    return 0;
}

int insert_record_2(sqlite3* db, const char *query, const char *values[], int value_count) {
    sqlite3_stmt *stmt;
    int rc;

    if (!db) {
        fprintf(stderr, "Database connection is NULL.\n");
        return SQLITE_ERROR;
    }

    // Set busy timeout and WAL mode
    sqlite3_busy_timeout(db, 5000); // Wait up to 5 seconds if locked
    sqlite3_exec(db, "PRAGMA journal_mode=WAL;", NULL, NULL, NULL);

    // Start transaction
    rc = sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to begin transaction: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return rc;
    }

    // Prepare the SQL statement
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    // Bind values to the placeholders
    for (int i = 0; i < value_count; i++) {
        rc = sqlite3_bind_text(stmt, i + 1, values[i], -1, SQLITE_STATIC);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Failed to bind value %d: %s\n", i + 1, sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return rc;
        }
    }

    // Execute the statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
        sqlite3_finalize(stmt);
        return rc;
    }
    else {
        // Commit the transaction if successful
        rc = sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Failed to commit transaction: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return rc;
        }
    }

    // Finalize the statement
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    // Return success or error code
    return rc;

    
}

