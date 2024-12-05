#ifndef MESSAGE_H
#define MESSAGE_H

#include "user.h"
#include "movie.h"
#include "cinema.h"
#include "time.h"
#include "seat.h"
#include "pay.h"
#include "order.h"

enum state_signal
{
    LOGIN,
    SIGNUP,
    SEARCH,
    SEARCHFOUND,
    SEARCHNOTFOUND,
    SUCCESS,
    FAILURE,
    BOOKING,
    ORDERS,
    MOVIE,
    CINEMA,
    TIME,
    SEAT,
    PAY,
    CONFIRM
};



#define FARE 50000

typedef struct message
{
    user user;
    movie movie;
    cinema cinema;
    time time;
    seat seat;
    pay pay;
    order order;
} message;

#endif