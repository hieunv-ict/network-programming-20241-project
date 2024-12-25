#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "message.h"
#define MAXLINE 100
#define MAX_FIELDS 100
#define DELIMITER "#"
#define EOL "\0"

void sendStr(int fd, char *str)
{
    int len;
    len = htonl(strlen(str));
    send(fd, &len, sizeof(len), 0); // Send len of the string
    int n = send(fd, str, strlen(str), 0);  // Send the string
    
    if (n < 0){
        perror("send failed");
        // Or use strerror
        //printf("Error: %s\n", strerror(errno));
    }
}

int recvStr(int fd, char *str)
{
    int len = 0;
    char content[MAXLINE];

    memset(&content, 0, MAXLINE);
    int n = recv(fd, &len, sizeof(len), 0);
    recv(fd, content, ntohl(len), 0);
    strcpy(str, content);
    return n;
    // printf("%s\n", str);
}

void sendInt(int fd, int i)
{
    int content = htonl(i);
    send(fd, &content, sizeof(content), 0);
}

int recvInt(int fd)
{
    int num;
    recv(fd, &num, sizeof(num), 0);
    // printf("%d\n", ntohl(num));
    return ntohl(num);
}

void subStr(char *str, char *buffer, int start, int end)
{
    int j = 0;
    for (int i = start; i <= end; ++i)
    {
        buffer[j++] = str[i];
    }
    buffer[j] = 0;
}

int valueInArr(int val, int arr[], int size)
{
    if (size == 0)
        return 1;
    for (int i = 0; i < size; i++)
    {
        if (arr[i] == val)
            return 0;
    }
    return 1;
}

void getName(char *str, char *buffer, int size, int selectId, int name_len[], int id[])
{
    int start = 0;

    for (int i = 0; i < size; i++)
    {
        if (id[i] == selectId)
            selectId = i;
    }

    for (int i = 0; i < size; i++)
    {
        if (i == selectId)
            subStr(str, buffer, start, start + name_len[i] - 1);
        start += name_len[i];
    }
}

int getIndex(int *arr, int num, int select)
{
    int indx = 0;
    for (int i = 0; i < num; i++)
    {
        if (arr[i] == select)
        {
            indx = i;
            break;
        }
    }
    return indx;
}

int countAvailableSeats(int id[], int size)
{
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        if (id[i] == 0)
        {
            count++;
        }
    }
    return count;
}

// get message and parse the message by #. The array of datafield is store in fields
void parse_message(const char *message, char fields[][128], int *fieldCount) {
    char *token;
    char tempMessage[200]; // Temporary storage for the message

    // Make a copy of the message to avoid modifying the original
    strncpy(tempMessage, message, sizeof(tempMessage));
    tempMessage[sizeof(tempMessage) - 1] = '\0'; // Ensure null termination

    *fieldCount = 0;

    // Split the string using the delimiter
    token = strtok(tempMessage, DELIMITER);
    while (token != NULL && *fieldCount < MAX_FIELDS) {
        strncpy(fields[*fieldCount], token, 50);
        fields[*fieldCount][49] = '\0'; // Ensure null termination
        (*fieldCount)++;
        token = strtok(NULL, DELIMITER);
    }
}
char* concatenate_strings(char **strings, int count) {
    int totalLength = 0;
    for (int i = 0; i < count; i++) {
        totalLength += strlen(strings[i]) + 1; // +1 for the delimiter or null terminator
    }

    char *result = (char *)malloc(totalLength);
    if (!result) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    result[0] = '\0'; // Initialize as empty string

    for (int i = 0; i < count; i++) {
        strcat(result, strings[i]);
        if (i < count - 1) {
            strcat(result, DELIMITER);
        }
    }
    strcat(result, EOL);
    return result;
}

int get_array_length(char *array[]) {
    int count = 0;
    while (strcpy(array[count],"\0") != 0) {
        count++;
    }
    
    return count;
}

// convert enum to string
char* signal_string[] = {"LOGIN", "SIGNUP", "SEARCH", "SEARCHFOUND", "SEARCHNOTFOUND", "BROWSE", "BROWSEFOUND", "BROWSENOTFOUND",  "SUCCESS",
    "FAILURE", "BOOKING", "MOVIELIST", "NOMOVIE", "MOVIE", "MOVIECINEMA", "NOMOVIECINEMA", "CINEMA", "CINEMASHOWTIME", "SHOWTIME", "SHOWTIMESEATS", "BOOKINFO", "PRICE"};
char* get_string_from_signal(enum state_signal i){
    return signal_string[i];
}

enum state_signal get_signal_from_string(char* s){
    int len = sizeof(signal_string) / sizeof(signal_string[0]);
    for (int i = 0; i < len; i++){
        if (strcmp(signal_string[i], s) == 0){
            return i;
        }
    }
}

