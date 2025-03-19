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

    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

    getchar();

    initializeListeningSocket(server, listeningSocket);

    if(listeningSocket == INVALID_SOCKET){
        return 1;
    }

    bindListeningSocket(server, listeningSocket);

    getchar();

    SOCKET acceptingSocket;

    initializeAcceptingSocket(server, acceptingSocket, listeningSocket);

    getchar();

    closeListeningSocket(listeningSocket);
    closeAcceptingSocket(acceptingSocket);
    WSACleanup();

    getchar();

    return 0;
}