#include <stdio.h>

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "ListeningSocket.c"
#include "Parser.c"
#include "FileManager.c"

int main(int argc, char *argv[]);
void setupSocket();

int main(int argc, char *argv[])
{
    //setupSocket();    

    return 0;
}

void setupSocket()
{
    SOCKET listeningSocket;
    char buffer[4096];

    struct sockaddr_in server, client;

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

    WSADATA wsa;

    if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
    {
        printf("\nWSA Startup failed");

        WSACleanup();

        return;
    }

    listeningSocket = initializeListeningSocket(server);

    bindListeningSocket(server, listeningSocket);

    SOCKET acceptingSocket;

    while((acceptingSocket = initializeAcceptingSocket(client, listeningSocket)) != INVALID_SOCKET)
    {

    }

    getchar();

    closeListeningSocket(listeningSocket);
    closeAcceptingSocket(acceptingSocket);
    WSACleanup();
}