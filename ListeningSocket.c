#include <stdio.h>

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

SOCKET initializeListeningSocket(struct sockaddr_in server);
void bindListeningSocket(struct sockaddr_in server, SOCKET s);
SOCKET initializeAcceptingSocket(struct sockaddr_in server, SOCKET listeningSocket);
void closeListeningSocket(SOCKET s);
void closeAcceptingSocket(SOCKET s);

SOCKET initializeListeningSocket(struct sockaddr_in server){

    SOCKET s;


    if((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("\nSocket Initialization failed");

        closesocket(s);
        WSACleanup();

        return INVALID_SOCKET;
    }

    return s;
}

void bindListeningSocket(struct sockaddr_in server , SOCKET s)
{

    if((bind(s, (struct sockaddr*)&server, sizeof(server))) == SOCKET_ERROR)
    {
        printf("\nBinding process has failed");

        closesocket(s);
        WSACleanup();

        return;
    }

    listen(s, 3);
    return;
}

SOCKET initializeAcceptingSocket(struct sockaddr_in client, SOCKET listeningSocket)
{
    SOCKET s;

    int c = sizeof(struct sockaddr_in);

    s = accept(listeningSocket, (struct sockaddr *)&client, &c);

    return s;
}

void closeListeningSocket(SOCKET s){
    closesocket(s);
    return;
}

void closeAcceptingSocket(SOCKET s){
    closesocket(s);
    return;
}