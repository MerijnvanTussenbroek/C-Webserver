#include <stdio.h>

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

void initializeListeningSocket(struct sockaddr_in server, SOCKET s);
void bindListeningSocket(struct sockaddr_in server, SOCKET s);
void initializeAcceptingSocket(struct sockaddr_in server, SOCKET acceptingSocket, SOCKET listeningSocket);
void closeListeningSocket(SOCKET s);
void closeAcceptingSocket(SOCKET s);

void initializeListeningSocket(struct sockaddr_in server, SOCKET s){

    WSADATA wsa;

    if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
    {
        printf("\nWSA Startup failed");
        s = INVALID_SOCKET;
        WSACleanup();

        return;
    }

    if((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("\nSocket Initialization failed");

        closesocket(s);
        WSACleanup();

        return;
    }

    if((connect(s, (struct sockaddr*)&server, sizeof(server)) < 0))
    {
        printf("\nMaking the connection has failed");

        closesocket(s);
        WSACleanup();

        return;
    }

    return;
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

void initializeAcceptingSocket(struct sockaddr_in server, SOCKET acceptingSocket, SOCKET listeningSocket)
{

    return;
}

void closeListeningSocket(SOCKET s){
    closesocket(s);
    return;
}

void closeAcceptingSocket(SOCKET s){
    closesocket(s);
    return;
}