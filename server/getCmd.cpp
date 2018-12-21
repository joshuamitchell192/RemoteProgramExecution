/*
    Description
        Send 40 lines of a specified file from a specifed directory.
        Wait for the client to continue before sending the next 40 lines.
    Parameters:
        Commands - the array of string of commands
*/

#include "server.h"

#ifdef __linux__
void getCmd(int ClientSocket, char **commands) {
    DIR *directory;
    struct dirent *dp;
    char *fileName;
    char fullFileName[256];
    directory = opendir(commands[1]);
    // Check if the directory exists
    if (directory == NULL) {
        printf("Directory %s couldn't be found", commands[1]);
        exit(1);
    }
    // loop through each file in the directory
    do {
        errno = 0;
        // Open the next file
        if ((dp = readdir(directory)) != NULL) {
            // Check if its the file we're looking for
            if (strcmp(dp->d_name, commands[2]) == 0) {
                printf("found %s\n", commands[2]);
                
                // Get the full path to the file
                fileName = dp->d_name;
                strcpy(fullFileName, commands[1]);
                strcat(fullFileName, "/");
                strcat(fullFileName, fileName);
                
                printf("File name: %s\n", fileName);
                sendFile(ClientSocket, fullFileName);
                closedir(directory);
                return;
            }
            else continue;
            
        }
    } while (dp != NULL);

    if (errno != 0) {
        perror("Error reading directory");
        exit(1);
    }
    else {
        printf("Couldn't find %s\n", commands[2]);
        exit(1);
    }
    closedir(directory);

#elif _WIN32
void getCmd(SOCKET ClientSocket, char **commands) {
    
#endif
}
#ifdef _WIN32
void sendFile(SOCKET ClientSocket, char *fileName) {
#elif __linux__
void sendFile(int ClientSocket, char *fileName) {
    /*
        Send the file specified 40 lines at a time.
        
    */

    char buf[ 2048 ]={0,};
    char line[1024]={0,};
    char recvbuf[BUFLEN];
    int count = 1, iResult;
    FILE *fp;
    int c;
    

    fp = fopen(fileName, "r");
    // Read in the file, one line at a time
    while (fgets(line, BUFLEN, fp) != NULL) {
        // Concatenate each line into a larger buffer
        strcat(buf, line);
        count++;
        if (count == 40) {
            // Send the 40 lines to the client
            send_to_client(ClientSocket, buf, 2048);
            memset(buf, '\0', 2048);
            count = 1;
            // Wait for the client to continue receiving
            iResult = recv(ClientSocket, recvbuf, BUFLEN, 0);
            if (iResult == 0) {
                printf("Get command child connection closed...\n");
            }
            else if (iResult < 0) {
                perror("recv failed with error: ");
                close(ClientSocket);
                exit(1);
            }
        }
    }
    if (count < 40) {
        send_to_client(ClientSocket, buf, 2048);
    }
    send_to_client(ClientSocket, "EOF", (int)strlen("EOF"));
    fclose(fp);

#endif
}
