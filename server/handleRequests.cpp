/*
    Description:
        Here the client is placed into an infinite loop where it can request commands.
        If the client disconnects, the SIGCHLD signal will be caught by the sig_child 
        function and the infinite loop will break. After which the client socket will 
        be closed and the child process terminated.

    Parameters:
        ClientSocket - Socket that's handling the clients connection
    
*/

#include "server.h"
#ifdef _WIN32
void handle_request(SOCKET ClientSocket) {
#elif __linux__
void handle_request(int ClientSocket) {
#endif

    char buffer[256], recvbuf[BUFLEN];
    int iResult, recvbuflen = BUFLEN;
    
    do {

        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        
        if (iResult > 0) {
            #ifdef __linux__

            struct timespec start, end;
            char responseTime[64];
            
            printf("Running new process child for query\n");
            signal(SIGCHLD, sig_child);
            pid_t pid;
            if ((pid = fork()) == 0) {
                // Start the timer
                clock_gettime(CLOCK_REALTIME, &start);
                // print out the input received from the client
                printf("Bytes received: %d\n", iResult);
                printf("Recv buf : %s\n", recvbuf);

                
                char *commands[15];
                char delim[5] = " \n\0";
                int k = 0;

                // Separate the input into an array of commmands
                commands[k] = strtok(recvbuf, delim);
                
                while( commands[k] != NULL ) {
                    printf( " %s\n", commands[k] );
                    
                    k++;
                    commands[k] = strtok(NULL, delim);
                }
                
                // Compare the first command against all the expected commands names
                if ((strcmp(recvbuf, "quit") == 0) || (strcmp(recvbuf, "-q") == 0)) {
                    printf("Disconnected Client \n");
                    break;
                }
                
                else if (strcmp(commands[0], "put") == 0) {
                    // Check if enough commands are passed
                    printf("running put command\n");
                    if (k <= 2) {
                        printf("Put Usage : \"put directory sourcefile(s) [-f]\"\n");
                        //exit(1);
                    }
                    else {
                        putCmd(ClientSocket, commands, k);
                        exit(0);
                    }
                    
                    
                    
                }
                else if (strcmp(commands[0], "get") == 0) {
                    printf("running get command\n");
                    // Check if correct number of commands are passed
                    if (k != 3) {
                        printf("Get Usage : \"get directory sourcefile\"\n3 Arugments were not passed.\n");
                        exit(1);
                    }
                    else {
                        getCmd(ClientSocket, commands);
                        exit(0);
                    }
                }
                else if (strcmp(commands[0], "run") == 0) {
                    printf("Running run command\n");
                }
                else if (strcmp(commands[0], "list") == 0) {
                    
                    // Check if a correct number of commands are passed
                    if (k <= 3 && k >= 1) {
                        printf("Running list command\n");
                        listCmd(ClientSocket, commands, k);
                        exit(0);
                    }
                    else {
                        printf("list usage: \"list [-l] directory\"\n");
                        exit(1);
                    }

                }
                else if (strcmp(commands[0], "sys") == 0) {
                    printf("Running sys command\n");
                    sysCmd(ClientSocket);
                    exit(0);
                }
                // clock_gettime(CLOCK_REALTIME, &end);
                // long tdiff = ((end.tv_sec-start.tv_sec)*(1000*1000*1000) + (end.tv_nsec-start.tv_nsec)) / 1000 ;
                // snprintf(responseTime, 64, " %lu ", tdiff);
                // send_to_client(ClientSocket, responseTime, 64);
                //break;
            }
            else if (pid < 0) {
                perror("Handle request fork failed with error");
            }

            
            #endif
        }
        // True if the client disconnects
        else if (iResult == 0) {
            printf("Connection closing...\n");
        }
        // Error handling for recv
        #ifdef _WIN32
        else {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            
            exit(1);
        }
        #elif __linux__
        else {
            perror("recv failed with error: ");
            close(ClientSocket);
            exit(1);
        }
        #endif

        // reset the buffer so it doesn't retain values between runs
        memset(buffer, '\0', sizeof(buffer));
    } while (iResult > 0);


}

#ifdef _WIN32
void send_to_client(SOCKET ClientSocket, char *buffer, int buflen)
{
    int iSendResult = send(ClientSocket, buffer, buflen, 0);
    if (iSendResult == SOCKET_ERROR){
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        
        WSACleanup();
        
        exit(1);
    }

}

#elif __linux__
void send_to_client(int ClientSocket, char *buffer, int buflen)
{
    int iSendResult = send(ClientSocket, buffer, buflen, 0);
    if (iSendResult < 0 ){
        //printf("send failed with error: %s\n", strerror(errno));
        perror("send failed with error:");
        close(ClientSocket);
        exit(1);
    }
}
#endif

#ifdef _WIN32
char* receive(SOCKET ClientSocket, char *recvbuf, int recvbuflen) {
#elif __linux__
char* receive(int ClientSocket,char *recvbuf, int recvbuflen) {
#endif
    int iResult;
    do {
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0){
            return recvbuf;

        }
        else if ( iResult == 0 ){
            printf("Connection Closed\n");
        }
        else if (iResult == -1) {
            #ifdef __linux__
            perror("Receive Failed with error\n");
            #elif _WIN32
            printf("Receive failed with error: %s\n", WSAGetLastError());
            #endif
        }
        //else printf("receive failed with error: %d\n", WSAGetLastError());
    } while (iResult > 0);
}