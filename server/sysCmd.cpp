/*
    Description:
        Executes the sys command passed to the client with the popen function.

        First issues lsb_release -a to get the Operating System name and version, then 
        concatenates each line into a char array buffer. Issue the lshw -class processor
        command to get the processor type and other information. Then also concatenates 
        it to the end of the buffer line by line. 

        Once both commands are in the buffer, it's send to the client to be printed.

    Parameters:
        int ClientSocket - Socket that accepted the client's connection.

    Returns - void
2
*/
#include "server.h"

#ifdef __linux__
void sysCmd(int ClientSocket) {
    FILE *sys;
    sys = popen("lsb_release -a", "r");
    
    char buf[ BUFLEN ]={0,};
    char line[BUFLEN]={0,};

    // Get the operating system information
    while( fgets( line, BUFLEN,  sys ) != NULL) {
        //fprintf( stdout, "%s", line  );
        strcat(buf, line);
    
    }
    
    strcat(buf, "\n\n");

    // get the CPU information
    sys = popen("lshw -class processor", "r");
    while (fgets(line, BUFLEN, sys) != NULL) {
        strcat(buf, line);
    }
    fclose(sys);

    send_to_client(ClientSocket, buf, BUFLEN);   
    
#elif _WIN32
void sysCmd(SOCKET ClientSocket) {
#endif
    
    
}