#ifndef BOOKING_H
#define BOOKING_H
#include "../lib/message.h"

void booking(int fd, char *uname);
void receiveMovie();
void selectMovie();
void receiveCinema();
void selectCinema();
void receiveTime();
void selectTime();
void receiveSeat();
void selectSeat();
void receivePayment();
void selectPayment();
void confirmOrder();
void printTicket();
int get_response_list(int socketfd, char* msg_fields[], int fields_cnt, char fields[][128]);
#endif