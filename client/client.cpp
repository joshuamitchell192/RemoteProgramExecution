/*
    Description:
        Main file the client that handles most of its functionality.
        Connects to the server and then calls the commandline function. 
        The client forks on new queries and runs infinitly until the quit command is passed
        or an error occurs or it's forced to exit.


*/

#include "client.h"

int main(int argc, char *argv[]){

    int iResult;

    if (argc != 2) {
        printf("usage: %s server-name\n", argv[0]);
        return 1;
    }
    //Connect to the server
    #ifdef _WIN32
    SOCKET ConnectSocket = connectServer(argv[1]);
    #elif __linux__
    int ConnectSocket = connectServer(argv[1]);
    #endif

    // Pass commands to the server and wait for a response
    commandLine(ConnectSocket);

    return 0;

}

/*---------------------------------------------------------------------------------------*/

#ifdef _WIN32
void commandLine(SOCKET ConnectSocket) {
#elif __linux__
void commandLine(int ConnectSocket) {
#endif

    
    char input[BUFLEN];
    
    char recvbuf[2048];
    int iResult;
    int recvbuflen = 2048;
    printf("Enter a command: ");
    
    

    

    while (fgets(input, BUFLEN, stdin) != NULL) {
        /*
            Send commands to the server until the user quits or an error occurs
        */

        char ** commands = separateCommands(input);

        #ifdef __linux__
        signal(SIGCHLD, sig_child);
        pid_t pid;
        if ((pid = fork()) == 0) {
            
            char *response;

            if ((strcmp(commands[0], "quit") == 0) || (strcmp(commands[0], "-q") == 0)){
                printf("Quitting application\n");
                break;
            }
            
            // Check if enough commands were passed for get 
            else if ((strcmp(commands[0], "get") == 0) && k > 1){
                sendToServer(ConnectSocket, input, BUFLEN);
                char continueChar[BUFLEN];
                // Receive from the server until "EOF" is send
                while (strcmp(recvbuf, "EOF") != 0) {
                    response = receive(ConnectSocket, iResult, recvbuf, recvbuflen);

                    // break before printing response if EOF is passed
                    if (strcmp(recvbuf, "EOF") == 0) {
                        memset(recvbuf, '\0', recvbuflen);
                        break;
                    }
                    else
                        printf("\n --- Response ---\n%s\n\n", recvbuf);
                    
                    //printf("Buffer Received: \n\n%s", recvbuf);
                    memset(recvbuf, '\0', recvbuflen);
                    printf("Enter any character to continue...\n");

                    fgets(continueChar, BUFLEN, stdin);
                    sendToServer(ConnectSocket, continueChar, BUFLEN);
                }  
                printf("\n--- End of File ---\n");

            }
            // Check if enough arguments are passed to the put command
            else if ((strcmp(commands[0], "put") == 0) && k > 1) {
                // Send the input to the server so that it can see all the files 
                // and the flag. Then call the put function to start uploading
                // the files.
                sendToServer(ConnectSocket, input, BUFLEN);
                put(ConnectSocket, commands, k);
                
            }
            
            
            
            else if ((strcmp(commands[0], "list") == 0) || (strcmp(commands[0], "sys") == 0)){
                sendToServer(ConnectSocket, input, (int)strlen(input));
                
                printf("\n--- Response --- \n%s\n", receive(ConnectSocket, iResult, recvbuf, recvbuflen));
                memset(recvbuf, '\0', 2048);
                
            }
            else {
                
                printf("Command is not accepted. Please use the following:\nput\nget\nlist\nsys\n");
            }
            
            //printf("\n--- Response Time --- \n%s\n", receive(ConnectSocket, iResult, recvbuf, recvbuflen));
            memset(recvbuf, '\0', 2048);

            printf("\nEnter a command: ");
            
        
        }
        else if (pid < 0) {
            perror("Command line fork failed with error");
        }
        
        #endif
        
        
        #ifdef _WIN32
        printf("returned commands: %s %s", commands[0], commands[1]);
        #endif
        
        
    }
    #ifdef __linux__
    close(ConnectSocket);
    exit(0);
    #elif _WIN32
    closesocket(ConnectSocket);
    exit(0);
    #endif
       
}

/*---------------------------------------------------------------------------------------*/

char** separateCommands(char * input) {
    char *inputCopy = input;
    char delim[5] = " \n\0";
    printf("input %s\n", input);
    strcpy(inputCopy, input);

    static char *commands[64];
    memset(commands, '\0', 64);

    int k = 0;

    // Seperate the input into an array of commands
    commands[k] = strtok(inputCopy, delim);
    
    while( commands[k] != NULL ) {
        k++;
        commands[k] = strtok(NULL, delim);
    }

    printf("Commands %s %s\n", commands[0], commands[1]);
    return commands;
}

/*---------------------------------------------------------------------------------------*/


#ifdef _WIN32
char* receive(SOCKET ConnectSocket, int iResult, char *recvbuf, int recvbuflen) {
#elif __linux__
char* receive(int ConnectSocket, int iResult, char *recvbuf, int recvbuflen) {
#endif

    /*
        Receives messages from the client and handles and errors that may occur.

    */
    do {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0){
            printf("Bytes Received: %d\n", iResult);
            return recvbuf;

        }
        else if ( iResult == 0 ){
            printf("Connection Closed\n");
        }
        // Printout error codes
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

/*---------------------------------------------------------------------------------------*/


#ifdef _WIN32
void sendToServer(SOCKET ConnectSocket, char *buffer, int buflen)
{
    /*
        Sends messages to the server and handles any errors for windows
    */
    int iSendResult = send(ConnectSocket, buffer, buflen, 0);
    if (iSendResult == SOCKET_ERROR){
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        
        WSACleanup();
        
        exit(1);
    }

}

#elif __linux__
void sendToServer(int ConnectSocket, char *buffer, int buflen)
{
    /*
        Sends messages to the server and handles any errors for linux
    */
    int iSendResult = send(ConnectSocket, buffer, buflen, 0);
    if (iSendResult < 0 ){
        //printf("send failed with error: %s\n", strerror(errno));
        perror("send failed with error:");
        close(ConnectSocket);
        exit(1);
    }
}
#endif

/*---------------------------------------------------------------------------------------*/


#ifdef __linux__
void sig_child(int signum) {
    pid_t pid;
    int stat;
    // -1 means to wait for first child to terminate.
    // WNOHANG tells the kernel not to block if there are no terminated children.
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
        printf("child %d terminated\n", pid);
    return;
}
#endif

/*---------------------------------------------------------------------------------------*/


#ifdef _WIN32
void put(SOCKET ConnectSocket, char **commands, int k) {
#elif __linux__
void put(int ConnectSocket, char **commands, int k) {
#endif

    /*
        Uploads each file to the server by reading them in completely with fread.

    */
    int iResult;
    printf("Running put command\n");
    int overwrite = 0;
    int noFiles = k - 2;
    if (strcmp(commands[k-1], "-f") == 0){
        overwrite = 1;
        noFiles--;

    }
    printf("Num files: %d\n", noFiles);
    char buf[LGBUFLEN];
    char response[64];
    // iterate through all of the files to be uploaded
    for (int i = 2; i < noFiles+2; i++) {
        printf("Opening file: %s\n", commands[i]);
        FILE *fp = fopen(commands[i], "r");
        
        if (fp != NULL) {
            memset(buf, '\0', LGBUFLEN);
            fread(buf, sizeof(char), LGBUFLEN, fp);
            printf("File upload buffer %s\n", buf);
            buf[strlen(buf)] = '\0';
            sendToServer(ConnectSocket, buf, LGBUFLEN);
            receive(ConnectSocket, iResult, response, 64);
            fclose(fp);
        } else {
            printf("File %s doesn't exist\n", commands[i]);
            exit(1);
        }
    }
}