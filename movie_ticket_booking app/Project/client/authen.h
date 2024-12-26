#ifndef AUTHEN_H
#define AUTHEN_H
#include "../lib/message.h"

int logIn(int socketfd, char *uname, char role[]);
int signup(int socketfd);
#endif