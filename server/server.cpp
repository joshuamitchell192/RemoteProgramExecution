
/*
    Main file/function that encapsulates all other functions. 
    Here 
*/
#include "server.h"

int main(int argc, char* argv[]) {

    int iResult, iSendResult;
    struct sockaddr_in Address;

    // Zero server Address struct memory 
    memset(&Address, '0', sizeof(Address));

    // specify server struct variables
    Address.sin_family = AF_INET;
    Address.sin_addr.s_addr = INADDR_ANY;
    Address.sin_port = htons(PORT);

    // Platform specific variable types
    #ifdef _WIN32
    WSADATA wsaData;
    SOCKET ListenSocket = serverStartup(Address);
    #elif __linux__
    int ListenSocket = serverStartup(Address);
    #endif

    manageConnections(ListenSocket);

}
