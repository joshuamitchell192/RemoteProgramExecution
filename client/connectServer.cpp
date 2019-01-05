/*
    Description :
        Connect to the server specified by the PORT macro and the serverAddress parameter

    Parameters :
        - serverAddress - Char array of server address to connect to.
    Returns :
        - ConnectSocket - A socket that has connected with the server

*/

#include "client.h"

#ifdef _WIN32
SOCKET connectServer(char *serverAddress) {
#elif __linux__
int connectServer(char *serverAddress) {
#endif

    int iResult;

    struct sockaddr_in Address;

    #ifdef _WIN32
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;

    iResult = WSAStartup(MAKEWORD(2, 2),&wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        _exit(1);
    }

    #elif __linux__
    int ConnectSocket = 0;
    #endif

    // Zero Address struct memory
    memset(&Address, '0', sizeof(Address));

    // Set Address struct elements
    Address.sin_family = AF_INET; // IPv4 
    Address.sin_addr.s_addr = inet_addr(serverAddress); // Address to connect to server
    Address.sin_port = htons( PORT ); 
    
    ConnectSocket = socket(AF_INET, SOCK_STREAM, 0); 

    #ifdef _WIN32
    if (ConnectSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        _exit(1);
    }
    #elif __linux__
    if (ConnectSocket < 0) {
        perror("Socket failed with error: \n");
        _exit(1);
    }
    #endif

    /* 
    
        Connect to server 
    
    */
    iResult = connect(ConnectSocket, (struct sockaddr *)&Address, sizeof(Address));
    #ifdef _WIN32
    if (iResult == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
    }
    #elif __linux__
    if (iResult < 0) {
        close(ConnectSocket);
        ConnectSocket = -1;
    }
    #endif

    
    #ifdef _WIN32
    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        _exit(1);
    }
    #elif __linux__
    if (ConnectSocket < 0 ) {
        perror("Unable to connect to server\n");
        _exit(1);
    }
    #endif

    printf("Connected to Server...\n");
    return ConnectSocket;
    

    

    
}