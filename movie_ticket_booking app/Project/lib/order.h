#ifndef ORDER_H
#define ORDER_H
typedef struct order
{
    char uname[100];

    char movie_id[8];
    char movie[64];

    char cinema_id[8];
    char cinema[64];

    char showtime_id[8];
    char showtime[64];

    int seat_num;
    char seat_id[100][20];

    int total;
    char pay[50];
    int pay_id;
    char card[20];
    int ccv;
    char valid_date[10];
} order;
#endif