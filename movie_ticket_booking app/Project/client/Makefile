CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 
LIB = ../lib

all: client

client: client.o authen.o booking.o function.o movie_function.o salemanager.o admin.o
	${CC} client.o authen.o booking.o function.o movie_function.o salemanager.o admin.o -o client

client.o: client.c
	${CC} ${CFLAGS} client.c

authen.o: authen.c
	${CC} ${CFLAGS} authen.c

booking.o: booking.c
	${CC} ${CFLAGS} booking.c

movie.o: movie_function.c
	${CC} ${CFLAGS} movie_function.c

admin.o: admin.c
	${CC} ${CFLAGS} admin.c

function.o: ${LIB}/function.c
	${CC} ${CFLAGS} ${LIB}/function.c

salemanager.o: salemanager.c 
	${CC} ${CFLAGS} salemanager.c

clean:
	rm -f *.o *~

