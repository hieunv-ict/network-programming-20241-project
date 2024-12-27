#include <stdio.h>
#include <string.h>
#include "../lib/message.h"
#include "../lib/function.h"
#include "database/sqlite/sqlite3.h"
#include "database_helper.h"
#define MAXLINE 1024

void update_user_role(int socketfd, sqlite3* db, char* username, char* role){
    const char* update_role_sql = 
    "UPDATE users "
    "SET role = ? "
    "WHERE username = ?";
    char* value[2] = {role, username};
    insert_record(db, update_role_sql, value, 2);
    sendStr(socketfd, "SUCCESS");
}