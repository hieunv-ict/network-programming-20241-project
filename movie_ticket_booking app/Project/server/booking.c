#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "../lib/message.h"
#include "../lib/function.h"

int socketfd;

void setFd(int fd)
{
    socketfd = fd;
}

void sendListMovies()
{
    int num = 5;
    int id[] = {1, 2, 4, 9, 10};
    char *name[] = {"Dune", "A Quiet Place", "Inception", "Gone girl", "Interstella", "Spider-man: Into the Spider-verse"};

    sendInt(socketfd, num);

    for (int i = 0; i < num; i++)
    {
        sendInt(socketfd, id[i]);
        sendStr(socketfd, name[i]);
    }
}

void sendListCinemas()
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

void sendListTimes()
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

void sendListSeats()
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

void sendListPayments(int fare)
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

void confirmOrder()
{
    // TODO: check the validity of the order
    sendInt(socketfd, SUCCESS);
}
