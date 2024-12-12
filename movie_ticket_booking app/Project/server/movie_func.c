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
void response_movie_list(int socketfd, char list[]);
void query_movie(sqlite3* db, const char* title, const char* sql, char list[]);

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
    printf("Filter: %s \n", filter);
    sqlite3_bind_text(stmt, 1, filter, -1, SQLITE_STATIC);

    
    while ((rc=sqlite3_step(stmt)) == SQLITE_ROW) {
        int movie_id = sqlite3_column_int(stmt, 0);
        printf("Filter: %s \n", filter);
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
        printf("No movie \n");
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
void response_movie_list(int socketfd, char list[]){
    char* datafields[2];
    datafields[0] = (char*)malloc(sizeof(char) * 128);
    datafields[1] = (char*)malloc(sizeof(char) * MAXLINE);
    if (flag > 0){
        char* signal = get_string_from_signal(SEARCHFOUND);
        strcpy(datafields[0], signal);
        strcpy(datafields[1], list);
        
        int msg_len = 2;
        char* message = concatenate_strings(datafields, msg_len);
        printf("Respone: %s \n", message);
        sendStr(socketfd, message);
        printf("Found \n");
        
    }
    else{
        printf("No movie is send \n");
        char* signal = get_string_from_signal(SEARCHNOTFOUND);
        sendStr(socketfd, signal);
    }
}
void send_movie_list(sqlite3* db, int socketfd, char title[]){
    char tmp[MAXLINE];
    strcpy(tmp, title);
    
    char list[MAXLINE] = "";
    //query database to get result and store result in list
    const char* sql = "SELECT * FROM Movie WHERE Title = ?;";
    query_movie(db, tmp, sql, list);
    // send message to client
    response_movie_list(socketfd, list);

}

void query_movie(sqlite3* db, const char* movie_title,const char* sql, char list[]){
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
    
}

void send_movies_browsed(sqlite3* db, int socketfd, char* category, char* value){
    
    char tmp_val[128];
    strcpy(tmp_val, value);
    char result[MAXLINE] = "";
    if (strcmp(category, "Genre") == 0){
        const char* sql = "SELECT * FROM Movie WHERE Category = ?;";
        query_movie(db, value, sql, result);
    }
    
    

}


// void browse_movie(sqlite3* db, int socketfd, char* category, char* value){
//     if (strcmp(category, "Genre") == 0){
//         select_movie_by_category(db, value);
//     }

// }
