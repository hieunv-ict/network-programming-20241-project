CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 
LIB = ../lib
DB = database
DBLIB = database/sqlite
all: server

server: server.o booking.o authen.o function.o movie_func.o database.o sqlite3.o cinema.o database_helper.o salemanager.o
	${CC} server.o booking.o authen.o function.o movie_func.o database.o sqlite3.o cinema.o database_helper.o salemanager.o -o server

booking.o: booking.c
	${CC} ${CFLAGS} booking.c

server.o: server.c
	${CC} ${CFLAGS} server.c

authen.o: authen.c
	${CC} ${CFLAGS} authen.c

movie_func.o: movie_func.c
	${CC} ${CFLAGS} movie_func.c

cinema.o: cinema.c
	${CC} ${CFLAGS} cinema.c

database_helper.o: database_helper.c
	${CC} ${CFLAGS} database_helper.c
	
function.o: ${LIB}/function.c
	${CC} ${CFLAGS} ${LIB}/function.c

database.o: ${DB}/database.c
	${CC} ${CFLAGS} ${DB}/database.c

sqlite3.o: ${DBLIB}/sqlite3.c
	${CC} ${CFLAGS} ${DBLIB}/sqlite3.c

salemanager.o: salemanager.c 
	${CC} ${CFLAGS} salemanager.c

clean:
	rm -f *.o *~

