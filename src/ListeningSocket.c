#include <stdio.h>

#include <winsock2.h>

SOCKET initializeListeningSocket(){

    SOCKET s;

    //Initializes a socket and returns it
    if((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("\nSocket Initialization failed");

        closesocket(s);
        WSACleanup();

        return INVALID_SOCKET;
    }

    printf("\nSocket Initialization successful");

    return s;
}

void bindListeningSocket(struct sockaddr_in server , SOCKET s)
{
    //binds a socket to the port we're listening to
    if((bind(s, (struct sockaddr*)&server, sizeof(server))) == SOCKET_ERROR)
    {
        printf("\nBinding process has failed");

        closesocket(s);
        WSACleanup();

        return;
    }

    printf("\nBinding successful");

    listen(s, 3);
    return;
}

SOCKET initializeAcceptingSocket(struct sockaddr_in client, SOCKET listeningSocket)
{
    //initializes an accepting socket
    SOCKET s;

    int c = sizeof(struct sockaddr_in);

    s = accept(listeningSocket, (struct sockaddr *)&client, &c);

    printf("\naccepting socket initialized");

    return s;
}

//closes the sockets
//naming was done for convention
void closeListeningSocket(SOCKET s){

    printf("\nclosing listening socket");
    closesocket(s);

    return;
}

void closeAcceptingSocket(SOCKET s){

    printf("\nclosing accepting socket");
    closesocket(s);

    return;
}