#undef UNICODE
#pragma once

// Change PORT to 80 before submission
#define PORT 8080
#define BUFLEN 512

#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>

#ifdef _WIN32
#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <signal.h>
#include <tchar.h>
#include <strsafe.h>

#elif __linux__
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
#endif

#ifdef _WIN32
int main(int argc, char *argv[]);
SOCKET serverStartup(struct sockaddr_in address);
void send_to_client(SOCKET ClientSocket, char *buffer, int buflen);
void handle_request(SOCKET ClientSocket);
void manageConnections(SOCKET ListenSocket);
struct sThreadInfo;
void sysCmd(SOCKET ClientSocket);
void getCmd(SOCKET ClientSocket, char **commands);
void listCmd(SOCKET ClientSocket, char **commands, int noCommands);
void sendFile(SOCKET ClientSocket, char *fileName);
void longList(char *fileName, char *buf);
void findDirectories(char *directoryList);
void putCmd(SOCKET ClientSocket, char **commands, int noCommands);
char* receive(SOCKET ClientSocket, char *recvbuf, int recvbuflen);



#elif __linux__
int main(int argc, char *argv[]);
int serverStartup(struct sockaddr_in address);
void manageConnections(int ListenSocket);
void handle_request(int ClientSocket);
void send_to_client(int ClientSocket, char *buffer, int buflen);
void sig_child(int signum);
void sysCmd(int ClientSocket);
void getCmd(int ClientSocket, char **commands);
void sendFile(int ClientSocket, char *fileName);
void listCmd(int ClientSocket, char **commands, int noCommands);
void longList(char *fileName, char *buf);
void findDirectories(char *directoryList);
void putCmd(int ClientSocket, char **commands, int noCommands);
char* receive(int ClientSocket, char *recvbuf, int recvbuflen);

#endif
