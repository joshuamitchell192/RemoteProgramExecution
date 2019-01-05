// Change PORT to 80 before submission
#define PORT 8080
#define BUFLEN 512
#define LGBUFLEN 2048

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#endif

#ifdef __linux__
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>


char** separateCommands(char * input);

#ifdef _WIN32
SOCKET connectServer(char *serverAddress);
char* receive(SOCKET ConnectSocket, int iResult, char *recvbuf, int recvbuflen);
void commandLine(SOCKET ConnectSocket);
void put(SOCKET ConnectSocket, char **commands, int k);
void sendToServer(SOCKET ConnectSocket, char *buffer, int buflen);

#elif __linux__
char* receive(int ConnectSocket, int iResult, char *recvbuf, int recvbuflen);
void commandLine(int ConnectSocket);
int connectServer(char *serverAddress);
void sig_child(int signum);
void put(int ConnectSocket, char **commands, int k);
void sendToServer(int ConnectSocket, char *buffer, int buflen);

#endif
