#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "../lib/message.h"
#include "../lib/function.h"
#include "movie_func.h"
#include "database/sqlite/sqlite3.h"
#define DBNAME "users.db"
// booking:
// receive movie id from client
// send available cinema
// receive cinema
// send showtime
// receive showtime
//...




// void send_cinema_lists(int socketfd, sqlite3* db, char* movie_id){
//     const char* get_cinema =
//     "SELECT Cinema.Cinema_id, Cinema.Cinema_name FROM Cinema "
//     "INNER JOIN " 
//     "Theatre ON Theatre.Cinema_id = Cinema.Cinema_id "
//     "INNER JOIN "
//     "Showtime ON Showtime.Theatre_id = Theatre.Theatre_id "
//     "WHERE Showtime.Movie_id = ?"
//     ";";
//     int cnt = 0;
//     sqlite3_stmt *stmt;
//     int rc;
    
//     rc = sqlite3_open(DBNAME, &db);
//     if (rc) {
//         fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
//         return;
//     }
//     rc = sqlite3_prepare_v2(db, get_cinema, -1, &stmt, 0);
//     if (rc != SQLITE_OK) {
//         fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
//         sqlite3_close(db);
//         return;
//     }
//     sqlite3_bind_text(stmt, 1, movie_id, -1, SQLITE_STATIC);

//     while ((rc = sqlite3_step(stmt)) == SQLITE_ROW){
//         const unsigned char* cinema_id = sqlite3_column_text(stmt, 0);
//         const unsigned char* cinema_name = sqlite3_column_text(stmt, 1);
//         printf("%s \n", cinema_name);
//         cnt++;
//         flag = 1;
//     }
// }


// void sendListMovies(int socketfd)
// {
//     const char* sql = "SELECT * FROM Movie WHERE Title = ";

//     int num = 5;
//     int id[] = {1, 2, 4, 9, 10};
//     char *name[] = {"Dune", "A Quiet Place", "Inception", "Gone girl", "Interstella", "Spider-man: Into the Spider-verse"};

//     sendInt(socketfd, num);

//     for (int i = 0; i < num; i++)
//     {
//         sendInt(socketfd, id[i]);
//         sendStr(socketfd, name[i]);
//     }
// }

void sendListCinemas(int socketfd)
{
    int num = 4;
    char id[] = {1, 2, 3, 4};
    char *name[] = {"CGV Aeon Ha Dong", "CGV Vincom Royal City", "Lotte Ha Dong", "BHD Pham Ngoc Thach"};

    sendInt(socketfd, num);

    for (int i = 0; i < num; i++)
    {
        sendInt(socketfd, id[i]);
        sendStr(socketfd, name[i]);
    }
}

void sendListTimes(int socketfd)
{
    int num = 6;
    char id[] = {1, 2, 3, 4, 5, 6};
    char *name[] = {"09:00", "12:00", "15:15", "18:25", "21:00", "23:30"};

    sendInt(socketfd, num);

    for (int i = 0; i < num; i++)
    {
        sendInt(socketfd, id[i]);
        sendStr(socketfd, name[i]);
    }
}

void sendListSeats(int socketfd)
{
    int row = 4;
    int col = 4;
    int size = row * col;
    char id[size];
    //char status[] = {0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0};
    char status[size];
    for (int i = 0; i < size; i++)
    {
    	id[i] = i;
    	status[i] = 0;
    }
    

    sendInt(socketfd, row);
    sendInt(socketfd, col);

    for (int i = 0; i < row * col; i++)
    {
        sendInt(socketfd, id[i]);
        sendInt(socketfd, status[i]);
    }
}

void sendListPayments(int fare, int socketfd)
{
    //int num = 2;
    //int id[] = {1, 2};
    //char *name[] = {"Thanh toan truc tiep tai rap", "Thanh toan online"};

    sendInt(socketfd, fare);
    //sendInt(socketfd, num);
    //for (int i = 0; i < num; i++)
    //{
    //   sendInt(socketfd, id[i]);
    //    sendStr(socketfd, name[i]);
    //}
    
    
}

void confirmOrder(int socketfd)
{
    // TODO: check the validity of the order
    sendInt(socketfd, SUCCESS);
}
