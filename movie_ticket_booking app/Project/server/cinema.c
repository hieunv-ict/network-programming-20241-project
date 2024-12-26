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
#define TICKETPRICE 100000
#define VALUELEN 128
// booking:
// receive movie id from client
// send available cinema
// receive cinema
// send showtime
// receive showtime
//...

int select_cinema(sqlite3* db, const char* query, const char* filters[], int filter_count, char* (buffer)[][2]){
    int cnt = 0;
    sqlite3_stmt *stmt;
    int rc;
    rc = sqlite3_open(DBNAME, &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);

    if (filter_count > 0){
        for (int i = 0; i < filter_count; i++) {
            rc = sqlite3_bind_text(stmt, i + 1, filters[i], -1, SQLITE_STATIC);
            if (rc != SQLITE_OK) {
                fprintf(stderr, "Failed to bind filter %d: %s\n", i, sqlite3_errmsg(db));
                sqlite3_finalize(stmt);
                return 0;
            }
        }
    }
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW){
        const unsigned char *id = sqlite3_column_text(stmt, 0);
        const unsigned char *name = sqlite3_column_text(stmt, 1);
        //const unsigned char *cinema_location = sqlite3_column_text(stmt, 2);
        
        buffer[cnt][0] = (char*)malloc(strlen((const char*)id) + 1);
        buffer[cnt][1] = (char*)malloc(strlen((const char*)name) + 1);
        //cinema_data[cnt][2] = (char*)malloc(strlen((const char*)cinema_location) + 1);

        if (!buffer[cnt][0] || !buffer[cnt][1]) {
            fprintf(stderr, "Memory allocation failed.\n");
            // Free previously allocated rows
            for (int i = 0; i < cnt; i++) {
                free(buffer[i][0]);
                free(buffer[i][1]);
                
            }
            sqlite3_finalize(stmt);
            return 0;
        }
        strcpy(buffer[cnt][0], (const char*)id);
        strcpy(buffer[cnt][1], (const char*)name);
        cnt++;
    }
    if (cnt == 0){
        printf("No value \n");
    }

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Execution error: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    // Finalize the statement and close the database
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return cnt;

}

int booking_response(sqlite3* db, const char* filter[] ,int filter_cnt, const char* sql, char list[]){
    // Get movie_data
    char* data[64][2];
    
    int cnt = select_cinema(db, sql, filter, filter_cnt, data);
    printf("cnt: %d\n", cnt);
    // Get movie_id and movie title to process into string
    for (int i = 0; i < cnt; i++){   
        char element[MAXLINE] = "";
        strcpy(element, data[i][0]);
        strcat(element,"#");
        strcat(element, data[i][1]);
        //strcat(element,"#");
        //strcat(element, cinema_data[i][2]);
        char temp[MAXLINE];
        strcpy(temp, element);
        
        strcat(list, temp);
        if (i < cnt - 1){
            strcat(list, "#");
        }
        
    }
    if (cnt == 0) return 0;
    else return 1;
}

void response_booking_info(int socketfd, char list[], int flag, int sig){
    char* datafields[2];
    // printf("Response: %s\n", list);
    datafields[0] = (char*)malloc(sizeof(char) * 128);
    datafields[1] = (char*)malloc(sizeof(char) * MAXLINE);
    if (flag > 0){
        char* signal = get_string_from_signal(sig);
        strcpy(datafields[0], signal);
        strcpy(datafields[1], list);
        
        int msg_len = 2;
        char* message = concatenate_strings(datafields, msg_len);
        printf("Message: %s\n", message);
        sendStr(socketfd, message);
    }
    else{
        printf("No value is send \n");
        char* signal = get_string_from_signal(sig+1);
        sendStr(socketfd, signal);
    }
}

void send_all_movie(int socketfd, sqlite3* db){
    const char* get_movie = 
    "SELECT Movie.Movie_id, Movie.Title FROM Movie WHERE Duration > ?;";
    char list[MAXLINE] = "";
    const char* filter[1] = {"1"};
    int flag = booking_response(db, filter, 1, get_movie, list);
    response_booking_info(socketfd, list, flag, MOVIELIST);

}
void send_cinema_list(int socketfd, sqlite3* db, char* movie_id){
    const char* get_cinema =
    "SELECT Cinema.Cinema_id, Cinema.Cinema_name FROM Cinema "
    "INNER JOIN " 
    "Theatre ON Theatre.Cinema_id = Cinema.Cinema_id "
    "INNER JOIN "
    "Showtime ON Showtime.Theatre_id = Theatre.Theatre_id "
    "WHERE Showtime.Movie_id = ?"
    ";";
    char list[MAXLINE] = "";
    const char* filter[1] = {movie_id};
    int flag = booking_response(db, filter, 1, get_cinema, list);
    response_booking_info(socketfd, list, flag, MOVIECINEMA);
    
}

void send_showtime_list(int socketfd, sqlite3* db, char* movie_id, char* cinema_id){
    const char* get_showtime =
    "SELECT Showtime.Showtime_id, Showtime.Datetime FROM Showtime "
    "INNER JOIN "
    "Theatre ON Theatre.Theatre_id = Showtime.Theatre_id "
    "INNER JOIN "
    "Cinema ON Cinema.Cinema_id = Theatre.Cinema_id "
    "WHERE Showtime.Movie_id = ? AND Cinema.cinema_id = ?"
    ";";
    char list[MAXLINE] = "";
    const char* filter[2] = {movie_id, cinema_id};
    int flag = booking_response(db, filter, 2, get_showtime, list);
    response_booking_info(socketfd, list, flag, CINEMASHOWTIME);
}

void send_seats(int socketfd, sqlite3* db, char* movie_id, char* cinema_id, char* showtime_id){
    const char* get_seats =
    "SELECT SeatTheatre.Seat_id, SeatTheatre.Status FROM SeatTheatre "
    "INNER JOIN "
    "Theatre ON Theatre.Theatre_id = SeatTheatre.Theatre_id "
    "INNER JOIN "
    "Cinema ON Cinema.Cinema_id = Theatre.Cinema_id "
    "INNER JOIN "
    "Showtime ON Showtime.Theatre_id = Theatre.Theatre_id "
    "WHERE Showtime.Movie_id = ? AND Cinema.cinema_id = ? AND Showtime.Showtime_id = ?"
    ";";
    char list[MAXLINE] = "";
    const char* filter[3] = {movie_id, cinema_id, showtime_id};
    int flag = booking_response(db, filter, 3, get_seats, list);
    response_booking_info(socketfd, list, flag, SHOWTIMESEATS);
}

void send_price(int socketfd, sqlite3* db, char* movie_id, char* cinema_id, char* showtime_id, char* seats[]){
    
}

void send_booking_price(int socketfd, char seat_num[128]){
    int num = atoi(seat_num);
    double price = num * TICKETPRICE;
    char str_price[32];
    sprintf(str_price, "%.1f", price);
    strcat(str_price, "VND");
    response_booking_info(socketfd, str_price, 1, PRICERES);
    
}

int send_booking_result(int socketfd, sqlite3 *db, char datafields[][128]){
    //create booking record with given user_id and showtime_id.
    //create booking seat records with given seats and new booking_id
    //change seat status of reserved seat in SeatTheatre table => need theatre_id.


    // const char* insert_movie = "INSERT INTO Director (Name) VALUES (?);";
    //char* movie_value[1] = {"Sam Raimi"};
    // insert_record_2(db, insert_movie, movie_value, 1);

    // char *errMsg = 0;
    // char sql[256];
    // // printf("Sign up: %s %s", username, password);
    // // Insert the user into the database

    // get movie_title
    const char* get_movie =
    "SELECT Title, Duration FROM Movie "
    "WHERE Movie_id = ? "
    ";";
    char* movie_id = datafields[2];
    const char* filter[1] = {movie_id};
    char* data[64][2];
    int cnt = select_cinema(db, get_movie, filter, 1, data);
    char* movie_title = data[cnt-1][0];

    // get username
    const char* get_user_id = 
    "SELECT id, username FROM users WHERE username = ?;";
    char* username = datafields[1];
    const char* filter1[1] = {username};
    cnt = select_cinema(db, get_user_id, filter1, 1, data);
    char* user_id = data[cnt-1][0];
    char* showtime_id = datafields[4];
    sqlite3_close(db);
    // count seats
    int index = 5;
    int seat_cnt = 0;
    while (strcmp(datafields[index], "\0") != 0){
        seat_cnt++;
        index++;
    }
    int price = seat_cnt * TICKETPRICE;
    char str_price[16];
    sprintf(str_price, "%d", price);
    char* temp = str_price;

    // insert new record into booking table
    const char* insert_booking = "INSERT INTO Booking (Showtime_id, user_id, Fee) VALUES (?, ?, ?);";
    char* booking_value[3] = {showtime_id, user_id, temp};
    insert_record(db, insert_booking, booking_value, 3); 
    // insert new booking seats
    const char* get_booking = "SELECT Booking_id, Showtime_id FROM Booking WHERE Showtime_id = ? AND user_id = ?;";
    const char* filter2[2] = {showtime_id, user_id};
    cnt = select_cinema(db, get_booking, filter2, 2, data);
    char* booking_id = data[cnt-1][0];

    // update seat status
    const char* get_theatre = 
    "SELECT Theatre.Theatre_id, Theatre.Theatre_name FROM Theatre "
    "INNER JOIN "
    "Showtime ON Theatre.Theatre_id = Showtime.Theatre_id "
    "WHERE Showtime.Showtime_id = ?;";
    const char* filter3[1] = {showtime_id};
    cnt = select_cinema(db, get_theatre, filter3, 1, data);
    char* theatre_id = data[cnt-1][0];

    const char* update_seat = 
    "UPDATE SeatTheatre "
    "SET Status = 1 "
    "WHERE Seat_id = ? AND Theatre_id = ?;";
    for (int i = 0; i < seat_cnt; i++){
        char* seat_theatre_value[2] = {datafields[5+i], theatre_id};
        insert_record(db, update_seat, seat_theatre_value, 2);
        sendStr(socketfd, get_string_from_signal(SUCCESS));
    }

}

