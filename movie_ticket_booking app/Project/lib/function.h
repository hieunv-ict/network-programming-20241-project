#ifndef FUNCTION_H
#define FUNCTION_H
//#define EOL "\0"
void sendStr(int fd, char *str);
int recvStr(int fd, char *str);
void sendInt(int fd, int i);
int recvInt(int fd);

void subStr(char *str, char *buffer, int start, int end);
int valueInArr(int val, int arr[], int size);
void getName(char *str, char *buffer, int size, int selectId, int name_len[], int id[]);
int getIndex(int *arr, int num, int select);
int countAvailableSeats(int id[], int size);

// helper
void parse_message(const char *message, char fields[][128], int *fieldCount);
char* concatenate_strings(char **strings, int count);
int get_array_length(char *array[]);
char* get_string_from_signal(enum state_signal i);
enum state_signal get_signal_from_string(char* s);
#endif