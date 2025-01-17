#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../lib/message.h"
#include "authen.h"
#include "booking.h"
#include "salemanager.h"
#include "movie_function.h"
#include "admin.h"
#define MAXLINE 4096   /*max text line length*/
#define SERV_PORT 3000 /*port*/

int socketfd;
message mess;

void connectToServer(char *ip)
{
    struct sockaddr_in servaddr;
    //Create a socket for the client
    //If socketfd<0 there was an error in the creation of the socket
    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("[-]Problem in creating the socket");
        exit(2);
    }
    printf("[+]Client Socket is created.\n");

    //Creation of the socket
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ip);
    servaddr.sin_port = htons(SERV_PORT); //convert to big-endian order

    //Connection of the client to the socket
    if (connect(socketfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("[-]Problem in connecting to the server");
        exit(3);
    }
    printf("[+]Connected to Server.\n");
}

int menuAuthen()
{
    int choice;
    do
    {
        printf("\n1. Login\n");
        printf("2. Signup\n");
        printf("3. Search movies by name\n");
        printf("4. Browse movies \n");
        printf("5. View movie details \n");
        printf("6. Exit\n");
        printf("Please enter your choice: ");
        scanf("%d", &choice);
    } while (choice < 1 || choice > 6);
    return choice;
}

int main(int argc, char **argv)
{
    int choice = 0, re;
    int exit_flag = 1;
    char uname[MAXLINE];
    if (argc != 2)
    {
        perror("[-]Missing IP address of the server");
        exit(1);
    }

    connectToServer(argv[1]);

    do{
        choice = menuAuthen();
        switch (choice)
        {
        case 1:
            char role[MAXLINE];
            do
            {
                re = logIn(socketfd, uname, role);
                if (re == FAILURE)
                    printf("WRONG PASSWORD!\n\n");
                else if (re == USERNOTFOUND){
                    printf("User not found! \n\n");
                }

            } while (re != SUCCESS);
            if(strlen(role) == 0) {
                printf("Error.\n");
                break;
            }
            printf("\n%s login successful!\n", uname);
            
            //TODO: add feature after logging in here:
            if (strcmp(role, "Ticket Buyer") == 0)
                booking(socketfd, uname);
            else if (strcmp(role, "Sale Manager") == 0) 
                salemanager_menu(socketfd);
            else if (strcmp(role, "Admin") == 0){
                change_user_role(socketfd);
            }
            break;
        case 2:
            do
            {
                re = signup(socketfd);
                if (re != SUCCESS)
                    printf("Signup fail!\n\n");

            } while (re != SUCCESS);
            
            printf("\nSign up successful!\n");
            
            break;

        case 3:
            do
            {
                re = search_movie_by_title(socketfd);
                if (re != SEARCHFOUND)
                    printf("Cannot found movie\n\n");

            } while (re != SEARCHFOUND);
            // print all movie with coressponding id
            break;

        case 4:
            do
            {
                printf("Browse movie by the following category: \n");
                printf("1. Movie genre \n");
                printf("2. Cinema \n");
                printf("3. Showtime \n");
                printf("Enter category: ");
                int category = 0;
                scanf("%d", &category);
                if(category > 3 || category < 1) {
                    printf("Category invalid. Try again!\n");
                    continue;
                }
                re = browse_movie(socketfd, category);
                if (re != BROWSEFOUND)
                    printf("There are no movie satisfying your need. \n\n");

            } while (re != BROWSEFOUND);
            // printf("This is the list of movies that meet your need: \n");
            break;

        case 5:
            int signal = view_movie_detail(socketfd);
            if (signal != SUCCESS){
                printf("Movie id is invalid.");
            }
            break;
        case 6:
            printf("You have exited!\n");
            exit_flag = 0;
            break;
        default:
            break;
        }
    }while(exit_flag != 0);
    
    
    return 0;
}