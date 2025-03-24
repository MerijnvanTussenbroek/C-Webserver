#include <stdio.h>

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "ListeningSocket.c"
#include "Parser.h"
#include "FileManager.c"
#include "RequestHandler.c"

int main(int argc, char *argv[]);
void setupSocket(struct sockaddr_in *serverpointer);

int size;

// http://localhost:8888/

int main(int argc, char *argv[])
{
    //setupSocket();  
    struct sockaddr_in server;

    WSADATA wsa;

    if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
    {
        printf("\nWSA Startup failed");

        WSACleanup();

        return 1;
    }
    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

    setupSocket(&server);

    return 0;
}

void setupSocket(struct sockaddr_in *serverPointer)
{
    // define a listening socket and a buffer
    SOCKET listeningSocket;
    char buffer[4096];

    struct sockaddr_in client, server;

    server = *serverPointer;

    listeningSocket = initializeListeningSocket(server);

    bindListeningSocket(server, listeningSocket);

    SOCKET acceptingSocket;

    while((acceptingSocket = initializeAcceptingSocket(client, listeningSocket)) != INVALID_SOCKET)
    {
        if((size = recv(acceptingSocket, buffer, 4096, 0)) != SOCKET_ERROR)
        {
            //message recieved
            buffer[size] = '\0';
            puts(buffer);
            Token* request = tokenizeRequest(buffer);
            char* response = processRequest(request);
            if(send(acceptingSocket, response, strlen(response), 0) >= 0)
            {
                //the reply has been sent
                printf("Reply has been sent \n");
                
            }
            else
            {
                //reply could not send
            }
            free(response);
        }
        else
        {
            //message could not be received
        }
    }

    getchar();

    closeListeningSocket(listeningSocket);
    closeAcceptingSocket(acceptingSocket);
    WSACleanup();
}