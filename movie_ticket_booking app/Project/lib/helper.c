#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "message.h"    
#define MAX_FIELDS 100
#define DELIMITER "#"
#define EOL '\0'

// get message and parse the message by #. The array of datafield is store in fields
void parse_message(const char *message, char fields[][50], int *fieldCount) {
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
    return result;
}

int get_array_length(char *array[]) {
    int count = 0;
    while (array[count] != '\0') {
        count++;
    }
    return count;
}

// convert enum to string
char* get_signal(enum state_signal i){
    char* signal_string[] = {"LOGIN", "SIGNUP", "SEARCH"};
    return signal_string[i];
}



// int main() {
//     // const char *message = "SIGNUP#hieu nv#123456";
//     // char fields[MAX_FIELDS][50];
//     // int fieldCount = 0;

    
//     char *username = "newuser";
//     char *password = "mypassword";
//     char *signal = get_signal(LOGIN);
//     char *fields[] = {signal, username, password, EOL};
   
//     int fieldCount = getArrayLength(fields);

    
//     // Concatenate
//     char *result = concatenateStrings(fields, fieldCount);
//     printf("Concatenated string: %s\n", result);

    

//     // Parse
//     char datafields[MAX_FIELDS][50];
//     parseMessage(result, datafields, &fieldCount);
//     printf("Parsed fields:\n");
//     for (int i = 0; i < fieldCount; i++) {
//         printf("Field %d: %s\n", i + 1, fields[i]);
//     }
    
//     return 0;
// }


