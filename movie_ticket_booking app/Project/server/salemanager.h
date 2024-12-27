#ifndef SALEMANAGER_H
#define SALEMANAGER_H

int add_new_movie(int socketfd, sqlite3* db, char title[], char category[], char director[], char duration[]);

#endif