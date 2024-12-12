#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "../lib/message.h"
#include "../lib/function.h"
#include "movie_function.h"

#define MAXLINE 2048
//int socketfd;
message ticket;

// void printTicket()
// {
//     printf("\n----------------------------\n");
//     printf("\tVE XEM PHIM\n");
//     printf("----------------------------\n");
//     printf("%s\n", ticket.order.uname);
//     printf("%s\n", ticket.order.movie);
//     printf("%s\n", ticket.order.cinema);
//     printf("%s\n", ticket.order.time);
//     printf("Seat(s):");
//     for (int i = 0; i < ticket.order.seat_num; i++)
//     {
//         printf(" %d", ticket.order.seat_id[i]);
//     }
//     printf("\n");
//     printf("Total price: %d VND\n", ticket.order.total);
//     //printf("\n%s\n", ticket.order.pay);
//     //if (strcmp(ticket.order.pay, "Thanh toan online") == 0)
//     //{
//     //    printf("Card Number: %s\n", ticket.order.card);
//     //    printf("Date expire: %s\n", ticket.order.valid_date);
//     //    printf("CCV: %d\n", ticket.order.ccv);
//     //}
//     printf("----------------------------\n");
// }

// void confirmOrder()
// {
//     sendInt(socketfd, CONFIRM);
//     recvInt(socketfd);
//     printTicket();
// }

// void selectPayment()
// {
//     int select, valid = 0, indx;

//     printf("Total cost: %d VND\n", ticket.order.total);
//     for (int i = 0; i < ticket.pay.num; i++)
//     {
//         printf("%d: %s\n", ticket.pay.id[i], ticket.pay.name[i]);
//     }
//     do
//     {
//         printf("Please enter the ID of payment type: ");
//         scanf("%d", &select);
//         if (valueInArr(select, ticket.pay.id, ticket.pay.num) == 1)
//         {
//             printf("ID is invalid !\n");
//         }
//         else
//             valid = 1;
//     } while (valid == 0);
//     printf("\n");

//     indx = getIndex(ticket.pay.id, ticket.pay.num, select);
//     ticket.order.pay_id = select;
//     strcpy(ticket.order.pay, ticket.pay.name[indx]);

//     sendInt(socketfd, ticket.order.pay_id);
//     sendStr(socketfd, ticket.order.pay);

//     if (select == 2)
//     {
//         printf("Card Number: ");
//         scanf("%s", ticket.order.card);
//         printf("Date expire: ");
//         scanf("%s", ticket.order.valid_date);
//         printf("CCV: ");
//         scanf("%d", &ticket.order.ccv);

//         sendStr(socketfd, ticket.order.card);
//         sendStr(socketfd, ticket.order.valid_date);
//         sendInt(socketfd, ticket.order.ccv);
//     }

//     confirmOrder();
// }

// void receivePayment()
// {
//     sendInt(socketfd, PAY);
//     ticket.order.total = recvInt(socketfd);
//     printf("Total cost: %d VND\n", ticket.order.total);
//     //ticket.pay.num = recvInt(socketfd);
//     //for (int i = 0; i < ticket.pay.num; i++)
//     //{
//     //    ticket.pay.id[i] = recvInt(socketfd);
//     //    recvStr(socketfd, ticket.pay.name[i]);
//     //}
//     //selectPayment();
//     confirmOrder();
// }

// void selectSeat()
// {
//     int valid, count = 0, num, select, available_seats;
//     char status;

//     for (int i = 0; i < ticket.seat.row * ticket.seat.col; i++)
//     {
//         status = ticket.seat.status[i] == 0 ? '-' : 'x';
//         printf("%2d: %c\t", ticket.seat.id[i], status);
//         count++;
//         if (count == ticket.seat.col)
//         {
//             count = 0;
//             printf("\n");
//         }
//     }
//     available_seats = countAvailableSeats(ticket.seat.status, ticket.seat.row * ticket.seat.col);
//     do
//     {
//         printf("Please enter number of seat you want to book: ");
//         scanf("%d", &num);
        
//         if (num <= 0 || num > available_seats)
//         {
//             printf("Number of seats is invalid! \n");
//         }
//     } while (num <= 0 || num > available_seats);

//     for (int i = 0; i < num; i++)
//     {
//         valid = 0;
//         do
//         {
//             printf("Please enter the ID of seat no.%d: ", i + 1);
//             scanf("%d", &select);

//             if ((valueInArr(select, ticket.seat.id, ticket.seat.row * ticket.seat.col) == 0) && (valueInArr(select, ticket.order.seat_id, i) == 1) &&
//                 (ticket.seat.status[select - 1] == 0))
//             {
//                 ticket.order.seat_id[i] = select;
//                 valid = 1;
//             }
//             else
//             {
//                 printf("ID is invalid or seat is already taken!\n");
//             }
//         } while (valid == 0);
//     }
//     printf("\n");

//     ticket.order.seat_num = num;
//     sendInt(socketfd, ticket.order.seat_num);
//     for (int i = 0; i < ticket.order.seat_num; i++)
//     {
//         sendInt(socketfd, ticket.order.seat_id[i]);
//     }

//     receivePayment();
// }

// void receiveSeat()
// {
//     sendInt(socketfd, SEAT);
//     ticket.seat.row = recvInt(socketfd);
//     ticket.seat.col = recvInt(socketfd);

//     for (int i = 0; i < ticket.seat.row * ticket.seat.col; i++)
//     {
//         ticket.seat.id[i] = recvInt(socketfd);
//         ticket.seat.status[i] = recvInt(socketfd);
//     }

//     selectSeat();
// }

char* movie_booking;
char* cinema_booking;
char* showtime_booking;
void select_movie_to_book(int socketfd);
void select_cinema_to_book(int socketfd);
void select_showtime_to_book(int socketfd);

void parse_response(char fields[][50]){
    int index = 1;
    while (strcmp(fields[index], "") != 0){
        printf("%s. %s \n",fields[index], fields[index+1]);
        index+=2;
    }
}

int get_response_list(int socketfd, char* msg_fields[], int fields_cnt){
    // send message
    char* message = concatenate_strings(msg_fields, fields_cnt);
    printf("Test: %s\n", message);
    sendStr(socketfd, message);
    // get response
    char response [MAXLINE];
    int n = recvStr(socketfd, response);
    int response_cnt = 0;
    
    char* signal_str;
    char fields[100][50];
    if (n > 0){
        parse_message(response, fields, &response_cnt);
        signal_str = fields[0];
        parse_response(fields);
        return get_signal_from_string(signal_str);
    }
    else{
        printf("Connection error \n");
    }
}

void booking(int socketfd, char *uname)
{
    int choice;
    // socketfd = fd;
    // strcpy(ticket.order.uname, uname);
    do
    {
        printf("\n1. Movie Booking\n");
        printf("2. Back\n");
        printf("Please enter your choice: ");
        scanf("%d", &choice);
    } while (choice < 1 || choice > 2);
    printf("\n");
    if (choice == 1)
    {
        // request list of all movie
        char* signal = get_string_from_signal(BOOKING);
        char* msg_fields[1] = {signal};
        //send MOVIE#movie_id
        int signal_res = get_response_list(socketfd, msg_fields, 1);
        if (signal_res == MOVIELIST){
            printf("Select a movie. \n");
            select_movie_to_book(socketfd);
        }
        else{
            printf("There are no movie to be screened. \n");
        }
    }
        //choose movie
        //send MOVIE#movie_id
        //receive list of cinema for that movie
        //sendInt(socketfd, BOOKING);
        //receiveMovie();
}

void select_movie_to_book(int socketfd){
    printf("Enter movie id: ");
    int tmp;
    scanf("%d", &tmp);
    char movie_id[10];
    sprintf(movie_id, "%d", tmp);
    movie_booking = movie_id;
    char* signal = get_string_from_signal(MOVIE);
    char* msg_fields[2] = {signal, movie_booking};
    int signal_res = get_response_list(socketfd, msg_fields, 2);
    if (signal_res == MOVIECINEMA){
        printf("Select a cinema. \n");
        select_cinema_to_book(socketfd);
        // process to select showtime
    }
    else{
        printf("The movie is not screened in any cinema. \n");
    }
}

void select_cinema_to_book(int socketfd){
    printf("Enter cinema id: ");
    char cinema_id[10];
    scanf("%s", cinema_id);
    cinema_booking = cinema_id;
    char* signal = get_string_from_signal(CINEMA);
    char* msg_fields[3] = {signal, movie_booking, cinema_booking};
    int signal_res= get_response_list(socketfd, msg_fields, 3);
    if (signal_res == CINEMASHOWTIME){
        printf("Select a showtime: \n");
        select_showtime_to_book(socketfd);
        // process to select showtime
    }
    else{
        printf("The movie has no showtime. \n");
    }
}

void select_showtime_to_book(int socketfd){
    printf("Enter showtime id: ");
    char showtime_id[20];
    scanf("%s", showtime_id);
    showtime_booking = showtime_id;
    char* signal = get_string_from_signal(SHOWTIME);
    char* msg_fields[4] = {signal, movie_booking, cinema_booking, showtime_booking};
    int signal_res= get_response_list(socketfd, msg_fields, 4);
    if (signal_res == SHOWTIMESEATS){
        printf("Seat map: \n");
        
        // process to select showtime
    }
    else{
        printf("The movie has no showtime. \n");
    }
}



