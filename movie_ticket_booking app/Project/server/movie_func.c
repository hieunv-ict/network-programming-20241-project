#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/message.h"
#include "../lib/function.h"
# include "database/sqlite/sqlite3.h"
#define MAXLINE 1028
#define DBNAME "users.db"
int flag = 0;

int select_movie(sqlite3* db, const char* query, const char* filter, char* (buf)[][5]);
void response_movie_list(int socketfd, int signal, int movie_count, char list[]);
int query_movie(sqlite3* db, const char* title, const char* sql, char list[]);

// pass in socketfd number and an array of 5 datafield of movie
// list has the format #movie_id_i#movie_title_i#...


// Flow of get movie and send to client: (example: send_movie_list())
// 1. write query to const char* sql
// 2. pass argument to query_movie to get string of movies and ids
// 3. pass argument to response_movie_list to send back to client

int select_movie(sqlite3* db, const char* query, const char* filter, char* (movie_data)[][5]){
    flag = 0;
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
    //printf("Filter: %s \n", filter);
    sqlite3_bind_text(stmt, 1, filter, -1, SQLITE_STATIC);

    
    while ((rc=sqlite3_step(stmt)) == SQLITE_ROW) {
        int movie_id = sqlite3_column_int(stmt, 0);
        //printf("Filter: %s \n", filter);
        int director_id = sqlite3_column_int(stmt, 1);
        const unsigned char *title = sqlite3_column_text(stmt, 2);
        const char *category = (const char*)sqlite3_column_text(stmt, 3);
        int duration = sqlite3_column_int(stmt, 4);

        
        movie_data[cnt][0] = (char*)malloc(sizeof(char) * 32);
        sprintf(movie_data[cnt][0], "%d", movie_id);
        
        movie_data[cnt][1] = (char*)malloc(sizeof(char) * 32);
        sprintf(movie_data[cnt][1], "%d", director_id);
        
        movie_data[cnt][2] = (char*)malloc(strlen((const char*)title) + 1);
        movie_data[cnt][3] = (char*)malloc(strlen((const char*)category) + 1);
        strcpy(movie_data[cnt][2], (const char*)title);
        strcpy(movie_data[cnt][3], (const char*)category);

        movie_data[cnt][4] = (char*)malloc(sizeof(char) * 64);
        sprintf(movie_data[cnt][4], "%d", duration);
        
        cnt++;
        flag = 1;
    }   
    
    if (cnt == 0){
        // printf("No movie \n");
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

void response_movie_list(int socketfd, int signal_num, int movie_count, char list[]){
    char* datafields[2];
    datafields[0] = (char*)malloc(sizeof(char) * 128);
    datafields[1] = (char*)malloc(sizeof(char) * MAXLINE);
    if (movie_count > 0){
        char* signal = get_string_from_signal(signal_num+1);
        strcpy(datafields[0], signal);
        strcpy(datafields[1], list);
        
        int msg_len = 2;
        char* message = concatenate_strings(datafields, msg_len);
        printf("Response: %s \n", message);
        sendStr(socketfd, message);
        // printf("Found \n");
        
    }
    else{
        // printf("No movie is send \n");
        char* signal = get_string_from_signal(signal_num+2);
        sendStr(socketfd, signal);
    }
}
void send_movie_list(sqlite3* db, int socketfd, char title[]){
    char tmp[MAXLINE];
    strcpy(tmp, title);
    
    char list[MAXLINE] = "";
    //query database to get result and store result in list
    const char* sql = "SELECT * FROM Movie WHERE Title = ?;";
    int mov_cnt = query_movie(db, tmp, sql, list);
    // send message to client
    response_movie_list(socketfd, SEARCH, mov_cnt, list);

}

int query_movie(sqlite3* db, const char* movie_title,const char* sql, char list[]){
    // Get movie_data
    char* movie_data[1024][5];
    int movie_cnt = select_movie(db, sql, movie_title, movie_data);

    
    // Get movie_id and movie title to process into string
    for (int i = 0; i < movie_cnt; i++){   
        char element[MAXLINE] = "";
        strcpy(element, movie_data[i][0]);
        strcat(element,"#");
        strcat(element, movie_data[i][2]);
        char temp[100];
        strcpy(temp, element);
        strcat(list, temp);
        if (i < movie_cnt-1){
            strcat(list, "#");
        }
    }
    return movie_cnt;
}

void send_movies_browsed(sqlite3* db, int socketfd, char* category, char* value){
    
    char tmp_val[128];
    strcpy(tmp_val, value);
    char result[MAXLINE] = "";
    if (strcmp(category, "Genre") == 0){
        const char* sql = "SELECT * FROM Movie WHERE Category = ?;";
        int mov_cnt = query_movie(db, value, sql, result);
        response_movie_list(socketfd, BROWSE, mov_cnt, result);
    }
    else if (strcmp(category, "Cinema") == 0) {
        const char* sql = "SELECT Movie.Movie_id, Movie.Director_id, Movie.Title, Movie.Category, Movie.Duration FROM Movie JOIN Showtime ON Showtime.Movie_id = Movie.Movie_id JOIN Theatre ON Theatre.Theatre_id = Showtime.Theatre_id JOIN Cinema ON Cinema.Cinema_id = Theatre.Cinema_id WHERE Cinema.Cinema_name = ?; ";
        int mov_cnt = query_movie(db, value, sql, result);
        response_movie_list(socketfd, BROWSE, mov_cnt, result);
    }
    else {
        const char* sql = "SELECT Movie.Movie_id, Movie.Director_id, Movie.Title, Movie.Category, Movie.Duration FROM Movie JOIN Showtime ON Movie.Movie_id = Showtime.Movie_id WHERE Showtime.Datetime = ?; ";
        int mov_cnt = query_movie(db, value, sql, result);
        response_movie_list(socketfd, BROWSE, mov_cnt, result);
    }
}



int select_movie2(sqlite3* db, const char* query, const char* filter, char* (movie_data)[][5]){
    flag = 0;
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
    //printf("Filter: %s \n", filter);
    sqlite3_bind_text(stmt, 1, filter, -1, SQLITE_STATIC);

    
    while ((rc=sqlite3_step(stmt)) == SQLITE_ROW) {
        int movie_id = sqlite3_column_int(stmt, 0);
        //printf("Filter: %s \n", filter);
        const unsigned char* title = sqlite3_column_text(stmt, 1);
        const unsigned char *director = sqlite3_column_text(stmt, 2);
        const char *category = (const char*)sqlite3_column_text(stmt, 3);
        int duration = sqlite3_column_int(stmt, 4);

        
        movie_data[cnt][0] = (char*)malloc(sizeof(char) * 32);
        sprintf(movie_data[cnt][0], "%d", movie_id);
        
        
        movie_data[cnt][1] = (char*)malloc(strlen((const char*)title) + 1);
        movie_data[cnt][2] = (char*)malloc(strlen((const char*)director) + 1);
        movie_data[cnt][3] = (char*)malloc(strlen((const char*)category) + 1);
        strcpy(movie_data[cnt][1], (const char*)title);
        strcpy(movie_data[cnt][2], (const char*)director);
        strcpy(movie_data[cnt][3], (const char*)category);

        movie_data[cnt][4] = (char*)malloc(sizeof(char) * 64);
        sprintf(movie_data[cnt][4], "%d", duration);
        
        cnt++;
        flag = 1;
    }   
    
    if (cnt == 0){
        // printf("No movie \n");
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



void view_movie_detail(sqlite3* db, int socketfd, char* id) {
    char list[MAXLINE] = "";

    // Prepare SQL query
    const char* query = "SELECT Movie.Movie_id, Movie.Title, Director.Name, Movie.Category, Movie.Duration "
                        "FROM Movie "
                        "INNER JOIN Director ON Director.Director_id = Movie.Director_id "
                        "WHERE Movie.Movie_id = ?; ";

    // Execute query and fetch results
    
    char* movie_data[1][5];
    int cnt = select_movie2(db, query, id, movie_data);
    // Check if data is retrieved successfully
    if (cnt > 0) {
        // Construct the response string
        for (int i = 0; i < 5; i++) {
            if (movie_data[0][i]) {
                strcat(list, movie_data[0][i]);
                if (i < 4) strcat(list, "#");
                free(movie_data[0][i]); // Free allocated memory
            }
        }
    } else {
        // Handle query failure or no result
        strcpy(list, "Error: No movie found.");
    }

    // Send response to the client
    response_movie_list(socketfd, SUCCESS-1, cnt, list);
}
