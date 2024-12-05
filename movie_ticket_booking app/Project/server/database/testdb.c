#include <stdio.h>
#include "sqlite3.h"

int create_table(sqlite3* db, char* query);
int main(){
    sqlite3* testdb;
    char *zErrMsg = 0;
   int rc;

   rc = sqlite3_open("test.db", &testdb);

   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(testdb));
      return(0);
   } else {
      fprintf(stderr, "Opened database successfully\n");
   }

   char* director_table = "DROP TABLE IF EXISTS Director; "\
   "CREATE TABLE Director("\
   "Director_ID TEXT PRIMARY KEY NOT NULL,"\
   "Name TEXT NOT NULL)";
   return create_table(testdb, director_table);
}

int create_table(sqlite3* db, char* query){
   char* err_msg;
   int rc;
   rc = sqlite3_exec(db, query, 0, 0, &err_msg);
   if (rc != SQLITE_OK){
      fprintf(stderr, "SQL error: %s \n", err_msg);
      sqlite3_free(err_msg);
      sqlite3_close(db);
      return 1;
   }
   sqlite3_close(db);
   return 0;
}

int insert_table(sqlite* db, char* query){}