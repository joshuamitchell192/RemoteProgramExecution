# RemoteProgramExecution
A server client application that provides an CLI to upload, download, view and execute programs on a remote server.


## How to compile:
Run the following commands for the respective OS on the bash and mingw cli
### Linux
#### Server
g++ -std=c++11 -o server server.cpp serverStartup.cpp manageConnections.cpp handleRequests.cpp sysCmd.cpp getCmd.cpp listCmd.cpp putCmd.cpp
#### Client
g++ -o client client.cpp connectServer.cpp
### Windows
#### Server
g++ -std=c++11 -o server server.cpp serverStartup.cpp manageConnections.cpp handleRequests.cpp sysCmd.cpp getCmd.cpp listCmd.cpp putCmd.cpp -l ws2_32
#### Client
g++ -o client client.cpp connectServer.cpp -l ws2_32


## How to run:
### Linux
Execute the a.out file to start the program for both the server and client.
### Windows
Execute the a.exe file to start the program for both the server and client.

#
Once the server is running, run the client to connect to the server.

Pass the address of the server (127.0.0.1) as an argument when running the client. 
