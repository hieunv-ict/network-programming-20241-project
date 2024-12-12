#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#define DBNAME "users.db"
#define QUERY_NUM 9
// Initialize the database and create the users table if it doesn't exist
sqlite3* initializeDatabase() {
    sqlite3 *db;
    char *errMsg = 0;
    // user table
    const char *sql = "CREATE TABLE IF NOT EXISTS users (" \
                      "id INTEGER PRIMARY KEY AUTOINCREMENT, " \
                      "username TEXT UNIQUE NOT NULL, " \
                      "password TEXT NOT NULL);";

    if (sqlite3_open(DBNAME, &db) != SQLITE_OK) {
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
    // director table
    const char *sql_director = 
        "CREATE TABLE IF NOT EXISTS Director ("
        "Director_id INTEGER PRIMARY KEY, "
        "Name TEXT"
        ");";

    int rc = 0;
    // Execute SQL statement for the Director table
    rc = sqlite3_exec(db, sql_director, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error while creating Director table: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        printf("Director table created successfully.\n");
    }

    // SQL statement to create the Movie table
    char *sql_movie = 
        "CREATE TABLE IF NOT EXISTS Movie ("
        "Movie_id INTEGER PRIMARY KEY, "
        "Director_id INTEGER, "
        "Title TEXT, "
        "Category TEXT, "
        "Duration INTEGER, "
        "FOREIGN KEY (Director_id) REFERENCES Director(Director_id)"
        ");";

    // Execute SQL statement for the Movie table
    rc = sqlite3_exec(db, sql_movie, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error while creating Movie table: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        printf("Movie table created successfully.\n");
    }
    char* sql_cinema = 
    "CREATE TABLE IF NOT EXISTS Cinema ("
        "Cinema_id TEXT PRIMARY KEY, "
        "Cinema_name TEXT,"
        "Cinema_location TEXT"
        ");";

    char* sql_theatre = 
    "CREATE TABLE IF NOT EXISTS Theatre ("
        "Theatre_id TEXT PRIMARY KEY, "
        "Theatre_name TEXT, "
        "Capacity INTEGER, "
        "Cinema_id TEXT,"
        "FOREIGN KEY (Cinema_id) REFERENCES Cinema(Cinema_id)"
        ");";

    char* sql_showtime = 
    "CREATE TABLE IF NOT EXISTS Showtime ("
        "Showtime_id TEXT PRIMARY KEY, "
        "Datetime TEXT, "
        "Movie_id INTEGER, "
        "Theatre_id TEXT, "
        "FOREIGN KEY (Theatre_id) REFERENCES Theatre(Theatre_id), " 
        "FOREIGN KEY (Movie_id) REFERENCES Movie(Movie_id)"
        ");";

    char* sql_seat = 
    "CREATE TABLE IF NOT EXISTS Seat("
        "Seat_id TEXT PRIMARY KEY"
        ");";
    
    char* sql_seat_theatre =
    "CREATE TABLE IF NOT EXISTS SeatTheatre("
        "Seat_id TEXT,"
        "Theatre_id TEXT,"
        "Status TEXT,"
        "FOREIGN KEY (Seat_id) REFERENCES Seat(Seat_id),"
        "FOREIGN KEY (Theatre_id) REFERENCES Theatre(Theatre_id)"
        ");";

    char* sql_book = 
    "CREATE TABLE IF NOT EXISTS Booking("
        "Booking_id INTEGER PRIMARY KEY,"
        "Showtime_id TEXT,"
        "Fee REAL,"
        "FOREIGN KEY (Showtime_id) REFERENCES Showtime(Showtime_id)"
        ");";

    char * sql_booking_seat = 
        "CREATE TABLE IF NOT EXISTS BookingSeat("
            "Seat_id TEXT,"
            "Booking_id INTEGER,"
            "FOREIGN KEY (Seat_id) REFERENCES Seat(Seat_id),"
            "FOREIGN KEY (Booking_id) REFERENCES Booking(Booking_id)"
            ");";

    char* sql_ticket = 
    "CREATE TABLE IF NOT EXISTS Ticket("
        "Ticket_type_id TEXT PRIMARY KEY,"
        "Ticket_type TEXT,"
        "Ticket_price REAL"
        ");";

    char* sql_ticket_booking = 
    "CREATE TABLE IF NOT EXISTS TicketBooking("
        "Booking_id INTEGER, "
        "Ticket_type TEXT, "
        "Ticket_quantity INTEGER, "
        "FOREIGN KEY (Ticket_type) REFERENCES Ticket(Ticket_type_id)"
        ");";

    // Execute SQL statement for the Movie table

    
    char* create_query[QUERY_NUM] = {sql_cinema, sql_theatre, sql_showtime, sql_seat, sql_seat_theatre, sql_book, sql_booking_seat, sql_ticket, sql_ticket_booking};
    for (int i = 0; i < QUERY_NUM; i++)
    {
        if (strcmp(create_query[i], "") != 0){
            rc = sqlite3_exec(db, create_query[i], 0, 0, &errMsg);
        }
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error while creating table: %s\n", errMsg);
            sqlite3_free(errMsg);
        } else {
            printf("Table created successfully.\n");
        }
    }
    
    sqlite3_close(db);
    return db;
}