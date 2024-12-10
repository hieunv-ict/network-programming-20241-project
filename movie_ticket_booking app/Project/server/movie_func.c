#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/message.h"
#include "../lib/function.h"
# include "database/sqlite/sqlite3.h"
#define MAXLINE 1028

int flag = 0;


void select_movie_by_title(sqlite3* db, const char* title, char list[]);

void send_movie_list(sqlite3* db, int socketfd, char title[]){
    //query database to get result
    // no result => return not found
    // has result => return search found
    //int fieldCount;
    // store movie result in movie_list
    
    char tmp[MAXLINE];
    strcpy(tmp, title);
    
    char list[MAXLINE] = "";
    select_movie_by_title(db, tmp, list);
    printf("List %s flag : %d \n", list, flag);
    
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

void select_movie_by_title(sqlite3* db, const char* movie_title, char list[]){
    
    printf("%s \n", movie_title);
    int movie_search_cnt = 0;
    sqlite3_stmt *stmt;
    int rc;
    const char *sql;
    
    rc = sqlite3_open("users.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }

    // SQL query to select a movie by name
    sql = "SELECT * FROM Movie WHERE Title = ?;";

    // Prepare the SQL statement
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        //sqlite3_close(db);
        return;
    }
    else{
        printf("Query \n");
    }

    // Bind the movie name to the SQL query
    sqlite3_bind_text(stmt, 1, movie_title, -1, SQLITE_STATIC);

    // Execute the query and process the results

    
    char* movie_data[5];
    //char* buf = (char*)malloc(sizeof(char) * MAXLINE);
    while ((rc=sqlite3_step(stmt)) == SQLITE_ROW) {
        int movie_id = sqlite3_column_int(stmt, 0);

        int director_id = sqlite3_column_int(stmt, 1);
        const unsigned char *title = sqlite3_column_text(stmt, 2);
        const char *category = (const char*)sqlite3_column_text(stmt, 3);
        int duration = sqlite3_column_int(stmt, 4);

        
        movie_data[0] = (char*)malloc(sizeof(char) * 32);
        sprintf(movie_data[0], "%d", movie_id);
        
        movie_data[1] = (char*)malloc(sizeof(char) * 32);
        sprintf(movie_data[1], "%d", director_id);
        
        movie_data[2] = (char*)title;

        movie_data[3] = (char*)category;

        movie_data[4] = (char*)malloc(sizeof(char) * 64);
        sprintf(movie_data[4], "%d", duration);
       
       
        char element[MAXLINE] = "";
        printf("Int: %d, String %s \n", movie_id, movie_data[0]);
        strcpy(element, movie_data[0]);
        strcat(element,"#");
        strcat(element, movie_data[2]);
        char temp[100];
        strcpy(temp, element);
        strcat(list, temp);
        strcat(list, "#");
        movie_search_cnt++;
       flag = 1;
    }
    

    if (movie_search_cnt == 0){
        printf("No movie with title %s \n", movie_title);
        return;
    }
    

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Execution error: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Finalize the statement and close the database
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return;
}