#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "../lib/message.h"
#include "../lib/function.h"

#include "database/sqlite/sqlite3.h"
#define DBNAME "users.db"
#define MAXLINE 2048
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
    else{

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
        printf("No cinema \n");
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

int cinema_message(sqlite3* db, const char* filter[] ,int filter_cnt, const char* sql, char list[]){
    // Get movie_data
    char* data[1024][2];
    
    int cnt = select_cinema(db, sql, filter, filter_cnt, data);

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
        strcat(list, "#");
    }

    if (cnt == 0) return 0;
    else return 1;
}

void response_cinema_list(int socketfd, char list[], int flag, int sig){
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
        sendStr(socketfd, message);
    }
    else{
        printf("No cinema is send \n");
        char* signal = get_string_from_signal(sig+1);
        sendStr(socketfd, signal);
    }
}

void send_all_movie(int socketfd, sqlite3* db){
    const char* get_movie = 
    "SELECT Movie.Movie_id, Movie.Title FROM MOVIE WHERE Duration > ?;";
    char list[MAXLINE] = "";
    const char* filter[1] = {"1"};
    int flag = cinema_message(db, filter, 1, get_movie, list);
    response_cinema_list(socketfd, list, flag, MOVIELIST);

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
    int flag = cinema_message(db, filter, 1, get_cinema, list);
    response_cinema_list(socketfd, list, flag, MOVIECINEMA);
    
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
    int flag = cinema_message(db, filter, 2, get_showtime, list);
    response_cinema_list(socketfd, list, flag, CINEMASHOWTIME);
}