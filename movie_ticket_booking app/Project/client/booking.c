#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "../lib/message.h"
#include "../lib/function.h"
#include "movie_function.h"

#define MAXLINE 2048
#define MAXSEAT 100
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


//char* ticket.order.movie;
//char* ticket.order.cinema_id;
//char* ticket.order.showtime_id;
char* booking_seats[];
int seat_count = 0;
void select_movie_to_book(int socketfd);
void select_cinema_to_book(int socketfd);
void select_showtime_to_book(int socketfd);

void select_seats(int socketfd, char seat_list[][20]);
void print_ticket();

void parse_response(char fields[][50]){
    int index = 1;
    while (strcmp(fields[index], "") != 0){
        printf("%s. %s \n",fields[index], fields[index+1]);
        index+=2;
    }
}

void parse_seat(char fields[][50], char* seat_list[]){
    int index = 1;
    seat_count = 0;
    char* seat_status = "Available";
    while (strcmp(fields[index], "") != 0){
        for (int i = 0; i < 4; i++){
            if (strcmp(fields[index+i*2], "") != 0){
                if (strcmp(fields[index+i*2+1], "0") != 0){
                    seat_status = "Reserved";
                }
                printf("%s-%s ",fields[index+i*2], seat_status);
                seat_list[seat_count] = fields[index+i*2];
                
                seat_count++;
            } 
        }
        printf("\n");
        index+=8;
    }
}

int get_response_list(int socketfd, char* msg_fields[], int fields_cnt, char fields[][50]){
    // send message
    char* message = concatenate_strings(msg_fields, fields_cnt);
    sendStr(socketfd, message);
    // get response
    char response [MAXLINE];
    int n = recvStr(socketfd, response);
    int response_cnt = 0;
    
    char* signal_str;
    
    if (n > 0){
        parse_message(response, fields, &response_cnt);
        signal_str = fields[0];
        //parse_response(fields);
        return get_signal_from_string(signal_str);
    }
    else{
        printf("Connection error \n");
    }
}
int get_seat_map(int socketfd, char* msg_fields[], int fields_cnt){
    // send message
    char* message = concatenate_strings(msg_fields, fields_cnt);
    sendStr(socketfd, message);
    // get response
    char response [MAXLINE];
    int n = recvStr(socketfd, response);
    char* signal_str;
    char fields[100][50];
    if (n > 0){
        //parse_message(response, fields, &response_cnt);
        int index = 1;
        char* seat_status = "Available";
        while (strcmp(fields[index], "") != 0){
            if (strcmp(fields[index+1], "0") != 0){
                seat_status = "Reserved";
            }
            printf("%s. %s ",fields[index], seat_status);
            if (index % 8 == 0){
                printf("\n");
            }
            index+=2;
            
    }
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
        // select movie
        select_movie_to_book(socketfd);   
    }
}

void select_movie_to_book(int socketfd){
    printf("Enter movie id: ");
    int tmp;
    scanf("%d", &tmp);
    char movie_id[10];
    sprintf(movie_id, "%d", tmp);
    strcpy(ticket.order.movie_id, movie_id);
    char* signal = get_string_from_signal(MOVIE);
    char* msg_fields[2] = {signal, ticket.order.movie_id};
    char result[100][50];
    int signal_res = get_response_list(socketfd, msg_fields, 2, result);
    if (signal_res == MOVIECINEMA){
        printf("Select a cinema. \n");
        parse_response(result);
        select_cinema_to_book(socketfd);
    }
    else{
        printf("The movie is not screened in any cinema. \n");
    }
}

void select_cinema_to_book(int socketfd){
    printf("Enter cinema id: ");
    char cinema_id[10];
    scanf("%s", cinema_id);
    strcpy(ticket.order.cinema_id,cinema_id);
    char* signal = get_string_from_signal(CINEMA);
    char* msg_fields[3] = {signal, ticket.order.movie_id, ticket.order.cinema_id};
    char result[100][50];
    int signal_res= get_response_list(socketfd, msg_fields, 3, result);
    if (signal_res == CINEMASHOWTIME){
        printf("Select a showtime: \n");
        parse_response(result);
        select_showtime_to_book(socketfd);
    }
    else{
        printf("The cinema you chose has no showtime. \n");
    }
}

void select_showtime_to_book(int socketfd){
    printf("Enter showtime id: ");
    char showtime_id[20];
    scanf("%s", showtime_id);
    strcpy(ticket.order.showtime_id,showtime_id);
    char* signal = get_string_from_signal(SHOWTIME);
    char* msg_fields[4] = {signal, ticket.order.movie_id, ticket.order.cinema_id, ticket.order.showtime_id};
    char result[100][50];
    int signal_res = get_response_list(socketfd, msg_fields, 4, result);
    
    if (signal_res == SHOWTIMESEATS){
        
        char* seat_list[MAXSEAT];
        parse_seat(result, seat_list);
        printf("There are %i seats \n", seat_count);
        select_seats(socketfd, seat_list);
    }
    else{
        printf("The movie is not screened in any theatre. \n");
    }
}

int check_valid_seat(char* input, char* seat_list[]){
    
    for (int i = 0; i < seat_count; i++){
        //printf("%s ", seat_list[i]);
        if (seat_list[i] == NULL){
            continue;
        }
        if (strcmp(seat_list[i], input) == 0){
            return 1;
        }

    }

    for (int i = 0; i < ticket.order.seat_num; i++){
        if (ticket.order.seat_id[i] == NULL){
            continue;
        }
        if (strcmp(input, ticket.order.seat_id[i]) == 0){
            printf("Duplicate seat \n");
            return 0;
        }
        else{
            if (i == ticket.order.seat_num-1){
                return 1;
            }
        }
    }
    printf("Invalid seat \n");
    return 0;
}
void select_seats(int socketfd, char seat_list[][20]){
    
    char num_tmp[5];
    int seat_num = 0;
    do{
        printf("Enter number of seats: ");
        scanf("%s", num_tmp);
        seat_num = atoi(num_tmp);
        
    }while (seat_num <= 0 || seat_num > seat_count);
    ticket.order.seat_num = seat_num;

    char seat_tmp[10];
    for (int i = 0; i < seat_num; i++){
        do{
            printf("Select seat for ticket %i: ", i+1);
            scanf("%s", seat_tmp);
        }while (check_valid_seat(seat_tmp, seat_list) == 0);
        // ticket.order.seat_id[i] = seat_tmp;
        strcpy(ticket.order.seat_id[i], seat_tmp);
        
    }

    for (int i = 0; i < ticket.order.seat_num; i++)
    {
        printf(" %s %d\n", ticket.order.seat_id[i], i);
    }
}

// send all information about booking and get price of the booking 
void send_booking_info(int socketfd){
    char* signal = get_string_from_signal(BOOKINFO);
    int cnt = 4 + ticket.order.seat_num;
    char* msg_fields[] = {signal, ticket.order.movie_id, ticket.order.cinema_id, ticket.order.showtime_id};
    for (int i = 4; i < cnt; i++){
        msg_fields[i] = ticket.order.seat_id[i-4];
    }
    char result[100][50];
    int signal_res = get_response_list(socketfd, msg_fields, cnt, result);
    
    if (signal_res == PRICE){
        parse_response(result);
        
    }
    else{
        printf("Your booking is not successful. \n");
    }
}

void print_ticket()
{
    printf("\n----------------------------\n");
    printf("\tBooking info\n");
    printf("----------------------------\n");
    printf("Username\n");
    printf("%s\n", ticket.order.movie_id);
    printf("%s\n", ticket.order.cinema_id);
    printf("%s\n", ticket.order.showtime_id);
    printf("Seat(s):");
    
    for (int i = 0; i < ticket.order.seat_num; i++)
    {
        printf(" %s\n", ticket.order.seat_id[i]);
    }
    printf("\n");
    //printf("Total price: %d VND\n", ticket.order.total);

    //printf("\n%s\n", ticket.order.pay);
    //if (strcmp(ticket.order.pay, "Thanh toan online") == 0)
    //{
    //    printf("Card Number: %s\n", ticket.order.card);
    //    printf("Date expire: %s\n", ticket.order.valid_date);
    //    printf("CCV: %d\n", ticket.order.ccv);
    //}
    printf("----------------------------\n");
}


