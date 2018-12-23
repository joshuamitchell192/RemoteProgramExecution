# RemoteProgramExecution
A server client application that provides an CLI to upload, download, view and execute programs on a remote server.

## How to run:
### Linux
Execute the a.out file to start the program for both the server and client.
### Windows
Execute the a.exe file to start the program for both the server and client.

## How to compile:
Run the following commands for the respective OS on the bash and mingw cli
### Linux
g++ -std=c++11 -o server server.cpp serverStartup.cpp manageConnections.cpp handleRequests.cpp sysCmd.cpp getCmd.cpp listCmd.cpp putCmd.cpp
### Windows
g++ -std=c++11 -o server server.cpp serverStartup.cpp manageConnections.cpp handleRequests.cpp sysCmd.cpp getCmd.cpp listCmd.cpp putCmd.cpp -l ws2_32
