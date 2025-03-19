#include <stdio.h>

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "ListeningSocket.c"
#include "Parser.c"

int main(int argc, char *argv[]);
char buffer[4096];

int main(int argc, char *argv[])
{
    SOCKET listeningSocket;

    struct sockaddr_in server, client;

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

    WSADATA wsa;

    if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
    {
        printf("\nWSA Startup failed");

        WSACleanup();

        return 1;
    }

    listeningSocket = initializeListeningSocket(server);

    bindListeningSocket(server, listeningSocket);

    SOCKET acceptingSocket;

    acceptingSocket = initializeAcceptingSocket(client, listeningSocket);

    if(acceptingSocket == INVALID_SOCKET)
    {
        printf("\naccepting failed");

        WSACleanup();
    }

    getchar();

    closeListeningSocket(listeningSocket);
    closeAcceptingSocket(acceptingSocket);
    WSACleanup();

    return 0;
}