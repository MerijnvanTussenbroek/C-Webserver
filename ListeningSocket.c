#include <stdio.h>

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

SOCKET InitializeListeningSocket(struct sockaddr_in server);
void BindListeningSocket(struct sockaddr_in server, SOCKET s);
void CloseListeningSocket(SOCKET s);

SOCKET InitializeListeningSocket(struct sockaddr_in server){

    WSADATA wsa;

    SOCKET s;

    if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
    {
        printf("\nWSA Startup failed");

        WSACleanup();

        return INVALID_SOCKET;
    }

    if((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("\nSocket Initialization failed");

        closesocket(s);
        WSACleanup();

        return INVALID_SOCKET;
    }

    if((connect(s, (struct sockaddr*)&server, sizeof(server)) < 0))
    {
        printf("\nMaking the connection has failed");

        closesocket(s);
        WSACleanup();

        return INVALID_SOCKET;
    }

    return s;
}

void BindListeningSocket(struct sockaddr_in server , SOCKET s)
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

SOCKET InitializeAcceptingSocket(struct sockaddr_in server, SOCKET s)
{

    return s;
}

void CloseListeningSocket(SOCKET s){
    closesocket(s);
    return;
}