/*
    Description :
        Sets up the server to start listening for incoming connections
    
        Creates a socket for listening, binds to any available interface and 
        calls the listen function.

    Parameters :
        - Address - struct that defines the servers variables for binding etc.

    Returns : 
        - a socket that listens for incoming client connections

*/



#include "server.h"


#ifdef _WIN32
SOCKET serverStartup(struct sockaddr_in Address){
    SOCKET ListenSocket = INVALID_SOCKET;
#elif __linux__
int serverStartup(struct sockaddr_in Address){
    int ListenSocket;
#endif

    int iResult;
    // startup winsock
    #ifdef _WIN32
    WSADATA wsaData;
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }
    #endif

    // Initialise ListenSocket with ipv4 and stream protocol
    ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
    #ifdef _WIN32
    // Print error codes that occur when creating the listen socket for
    // windows and linux
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        exit(1);
    }
    #elif __linux__
    if (ListenSocket < 0) {
        perror("Socket failed with error");
        exit(1);
    }
    #endif    
    
    
    // Bind to the specified address from the Address sockaddr_in struct
    iResult = bind(ListenSocket, (struct sockaddr *)&Address, sizeof(Address));
    #ifdef _WIN32
    // print error codes for when bind fails on windows and linux
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }
    #elif __linux__
    if (iResult < 0) {
        perror("Bind failed with error");
        close(ListenSocket);
        exit(1);
    }
    #endif

    /*
        Call listen function with the ListenSocket socket and set the max number of 
        clients to a value that the server computer can reasonably handle.
    */
    iResult = listen(ListenSocket, SOMAXCONN);
    #ifdef _WIN32
    // prints the error codes for when listen function fails.
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        #ifdef _WIN32
        WSACleanup();
        #endif
        exit(1);
    }
    #elif __linux__
    if (iResult < 0) {
        perror("Listen Failed with error:");
        close(ListenSocket);
        exit(1);
    }
    #endif

    printf("Listening on port %d\n", PORT);
    printf("Waiting on connections...\n");

    return ListenSocket;
    
} 