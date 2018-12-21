/*
    Description:
        Executes the list command in 3 different ways
        if the -l flags is passed then the long list version is returned

*/

#include "server.h"
#include <regex>

#ifdef _WIN32
void listCmd(SOCKET ClientSocket, char **commands, int noCommands) {
#elif __linux__
void listCmd(int ClientSocket, char **commands, int noCommands) {

    char *fileName;
    char fullFileName[256] = {0, };
    DIR *directory;
    struct dirent *dp;
    FILE *file;
    char buf[ 1024 ]={0,};
    printf("number of commands: %d\n", noCommands);
    for (int i = 0; i <= noCommands -1; i++) {
        printf("Command %d : %s\n", i, commands[i]);
    }
    // Check if the second command is a directory name, if it is then don't return the list of directories
    if((*commands[1]>='a' && *commands[1]<='z') || (*commands[1]>='A' && *commands[1]<='Z') || *commands[1] == '-' ) {
        
        if ((strcmp(commands[1], "-l") == 0) && noCommands > 2) {
            printf("Long list\n");

            
            
            char *line;
            // Append the directory name to get the full relatve path.
            strcat(fullFileName, "./");
            strcat(fullFileName, commands[2]);
            directory = opendir(commands[2]);
            if (directory == NULL) {
                printf("Directory %s couldn't be found", commands[2]);
                exit(1);
            }
            do {
                errno = 0;
                // Get the next file in the directory
                if ((dp = readdir(directory)) != NULL) {
                    fileName = dp->d_name;
                    
                    
                    strcat(fullFileName, "/");
                    strcat(fullFileName, fileName);
                    // ignore . and .. files
                    if (strcmp(fileName, ".") == 0 || strcmp(fileName, "..") == 0) {
                        continue;
                    }
                    
                    strcat(buf, "File name: "); 
                    strcat(buf, fileName);
                    longList(fullFileName, buf);
                    printf("Long buffer: %s", buf);
                    

                }
                
                
            } while (dp != NULL);
            send_to_client(ClientSocket, buf, 1024);
        }
        else {
            // List the files in the directory
            directory = opendir(commands[1]);
            if (directory == NULL ){
                printf("Directory %s couldn't be found", commands[2]);
                exit(1);
            }
            do {
                errno = 0;
                if ((dp = readdir(directory)) != NULL) {
                    fileName = dp->d_name;
                    if ( strcmp(fileName, ".") == 0 || strcmp(fileName, "..") == 0) {
                        continue;
                    }
                    strcat(buf, "File name: ");
                    strcat(buf, fileName);
                    strcat(buf, "\n");
                }
            } while (dp != NULL);
            send_to_client(ClientSocket, buf, 1024);
            memset(buf, '\0', 1024);
        }
    }
    else {
        printf("list directories\n");
        memset(buf, '\0', 1024);
        findDirectories(buf);
        send_to_client(ClientSocket, buf, 1024);
        memset(buf, '\0', 1024);
        //(directoryList. '\0', )
    }
    
#endif    
}


void longList(char *fileName, char *buf) {
    /*
        Concatenates the file size, creation data and access permissions to the buffer
    */
    printf("Long list executing\n");
    #ifdef __linux__
    //FILE *file = fopen(fileName, "r");

    char fileSize[64];
    char cTime[64];
    struct stat st;
    stat(fileName, &st);
    // Get the file size
    snprintf(fileSize, 64, " %ld ",st.st_size);
    printf("file size %s\n", fileSize);
    strcat(buf, " - File Size: ");
    strcat(buf, fileSize);

    // Get the creation date
    struct tm *tm;
    tm = localtime(&st.st_ctime);
    strftime(cTime, 64, "%d - %m - %y", tm);

    strcat(buf, " - Creation Date : ");
    strcat(buf, cTime);
    printf("Creation Time %s\n", cTime);

    // Get the access permissions
    strcat(buf, " - Access Permission");
    strcat(buf, " User: ");
    if (st.st_mode & S_IRUSR){
        strcat(buf, "R");
    } else strcat(buf, "-");
    if (st.st_mode & S_IWUSR){
        strcat(buf, "W");
    } else strcat(buf, "-");
    if (st.st_mode & S_IXUSR){
        strcat(buf, "X");
    } else strcat(buf, "-");
    strcat(buf, "Group: ");
    if (st.st_mode & S_IRGRP) {
        strcat(buf, "R");
    } else strcat(buf, "-");
    if (st.st_mode & S_IWGRP){
        strcat(buf, "W");
    } else strcat(buf, "-");
    if (st.st_mode & S_IXGRP) {
        strcat(buf, "X");
    } else strcat(buf, "-");
    
    strcat(buf, "\n");
    #endif

}

void findDirectories(char *directoryList){
    //char directoryList[256] = {0,};
    #ifdef __linux__
    const char* PATH = ".";
    char *fileName;

    DIR *dir = opendir(PATH);

    struct dirent *dp;

    do {
        errno = 0;
        // iterate through all files and check if they're directories
        if (((dp = readdir(dir)) != NULL) && dp->d_type == DT_DIR) {
            fileName = dp->d_name;
            // Concatenate the directory name into a list
            strcat(directoryList, "Folder: ");
            strcat(directoryList, fileName);
            strcat(directoryList, "\n");
        }
    } while (dp != NULL);

    closedir(dir);

    #endif
}
