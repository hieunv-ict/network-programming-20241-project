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

int select_cinema(sqlite3* db, const char* query, const char* filter, char* (cinema_data)[][2]){
    int cnt = 0;
    sqlite3_stmt *stmt;
    int rc;
    
    rc = sqlite3_open(DBNAME, &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }
    sqlite3_bind_text(stmt, 1, filter, -1, SQLITE_STATIC);
    
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW){
        const unsigned char *cinema_id = sqlite3_column_text(stmt, 0);
        const unsigned char *cinema_name = sqlite3_column_text(stmt, 1);
        //const unsigned char *cinema_location = sqlite3_column_text(stmt, 2);
        
        
        cinema_data[cnt][0] = (char*)malloc(strlen((const char*)cinema_id) + 1);
        cinema_data[cnt][1] = (char*)malloc(strlen((const char*)cinema_name) + 1);
        //cinema_data[cnt][2] = (char*)malloc(strlen((const char*)cinema_location) + 1);

        if (!cinema_data[cnt][0] || !cinema_data[cnt][1]) {
            fprintf(stderr, "Memory allocation failed.\n");
            // Free previously allocated rows
            for (int i = 0; i < cnt; i++) {
                free(cinema_data[i][0]);
                free(cinema_data[i][1]);
                
            }
            sqlite3_finalize(stmt);
            return 0;
        }

        printf("%s \n", cinema_data[0][1]);
        strcpy(cinema_data[cnt][0], (const char*)cinema_id);
        strcpy(cinema_data[cnt][1], (const char*)cinema_name);
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

int cinema_message(sqlite3* db, const char* filter ,const char* sql, char list[]){
    // Get movie_data
    char* cinema_data[1024][2];
    int cinema_cnt = select_cinema(db, sql, filter, cinema_data);

    // Get movie_id and movie title to process into string
    for (int i = 0; i < cinema_cnt; i++){   
        char element[MAXLINE] = "";
        strcpy(element, cinema_data[i][0]);
        strcat(element,"#");
        strcat(element, cinema_data[i][1]);
        //strcat(element,"#");
        //strcat(element, cinema_data[i][2]);
        char temp[MAXLINE];
        strcpy(temp, element);
        printf("Element: %s \n", element);
        strcat(list, temp);
        strcat(list, "#");
    }

    if (cinema_cnt == 0) return 0;
    else return 1;
}

void response_cinema_list(int socketfd, char list[], int flag){
    char* datafields[2];
    datafields[0] = (char*)malloc(sizeof(char) * 128);
    datafields[1] = (char*)malloc(sizeof(char) * MAXLINE);
    if (flag > 0){
        char* signal = get_string_from_signal(MOVIECINEMA);
        strcpy(datafields[0], signal);
        strcpy(datafields[1], list);
        
        int msg_len = 2;
        char* message = concatenate_strings(datafields, msg_len);
        sendStr(socketfd, message);
    }
    else{
        printf("No cinema is send \n");
        char* signal = get_string_from_signal(NOMOVIECINEMA);
        sendStr(socketfd, signal);
    }
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
    char tmp[128];
    strcpy(tmp, movie_id);
    int flag = cinema_message(db, tmp, get_cinema, list);
    response_cinema_list(socketfd, list, flag);
    
}