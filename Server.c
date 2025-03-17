#include <stdio.h>

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "ListeningSocket.c"
#include "Parser.c"

int main(int argc, char *argv[]);
char buffer[2048];

int main(int argc, char *argv[])
{
    SOCKET ListeningSocket;

    struct sockaddr_in Server;

    Server.sin_family = AF_INET;
    Server.sin_addr.s_addr = INADDR_ANY;
    Server.sin_port = htons( 8888 );

    ListeningSocket = InitializeListeningSocket(Server);
    BindListeningSocket(Server, ListeningSocket);

    CloseListeningSocket(ListeningSocket);
    WSACleanup();

    return 0;
}