/*
    Description
        Creates a new directory if none exists or overwrites 
        if -f flag is given.
*/

#include "server.h"

#ifdef _WIN32
void putCmd(SOCKET ClientSocket, char **commands, int noCommands) {
#elif __linux__ 
void putCmd(int ClientSocket, char **commands, int noCommands) {
#endif
    // Create the directry
    char buf[2048] = {0, };
    char line[1024] = {0, };
    char mkdirCommand[256] = {0,};
    FILE *fp;
    bool overwrite = false;
    #ifdef __linux__
    DIR *directory;
    char *directoryName = commands[1];
    int noFiles = noCommands -2;
    printf("Number of commands - %d\n", noCommands);
    // Check if directory exists
    if ((directory = opendir(commands[1])) != NULL) {
        // Check if overwrite flag is passed
        if (strcmp(commands[noCommands -1], "-f")) {
            overwrite = true;
            noFiles--;
            // Remove the existing directory
            strcpy(mkdirCommand, "rm -r ");
            strcat(mkdirCommand, directoryName);
            fp = popen(mkdirCommand, "r");
            pclose(fp);
            printf("Folder : %s overwritten\n", directoryName);
            // Create the new directory
            strcpy(mkdirCommand, "mkdir ");
            strcat(mkdirCommand, directoryName);
            fp = popen(mkdirCommand, "r");
            pclose(fp);
            
        }
        else {
            // Create the new directory
            strcpy(mkdirCommand, "mkdir ");
            strcat(mkdirCommand, directoryName);
            #ifdef __linux__
            fp = popen(mkdirCommand, "r");
            pclose(fp);
            #endif
        }
        
    } else printf("Directory %s doesn't exist\n", commands[1]);
    
    char files[64][256];
    // Check if no files were given. 
    if (overwrite && (noCommands == 3) || !overwrite && (noCommands == 2)){
        printf("No files were given\nPut Usage : \"Put directory sourcefile(s) [-f]\"\n");
        exit(1);
    }
    // Put all filenames into an array;
    for (int i = 2; i < noCommands - 2; i++) {
        strcpy(files[i], commands[i]);
        printf("File Name ; %s\n", files[i]);
    }

    closedir(directory);

    for (int i = 0; i < noCommands -2; i++) {

    }
    #endif
    

    

    // handle overwrite


}