#include <stdio.h>

#include <winsock2.h>

#include "ListeningSocket.h"
#include "Parser.h"
#include "FileManager.h"
#include "RequestHandler.h"

int main();
void setupSocket(struct sockaddr_in *serverpointer);

int size;

// http://localhost:8888/

int main()
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

    WSACleanup();

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

    printf("\nBegin listening");

    while((acceptingSocket = initializeAcceptingSocket(client, listeningSocket)) != INVALID_SOCKET)
    {
        if((size = recv(acceptingSocket, buffer, 4096, 0)) != SOCKET_ERROR)
        {
            //message recieved
            buffer[size] = '\0';
            printf("\n\n\n");
            puts(buffer);
            Token* request = tokenizeRequest(buffer);

            for(int i = 0; request[i].type != TOKEN_END ; i++)
            {
                printToken(request[i]);
            }

            char* response = processRequest(request);
            if(send(acceptingSocket, response, strlen(response), 0) >= 0)
            {
                //the reply has been sent
                printf("\nReply has been sent");

                printf("\nThe reply:\n %s \n \n", response);
                
            }
            else
            {
                //reply could not send
                printf("\nReply could not be sent");
            }
            free(response);
        }
        else
        {
            //message could not be received
        }

        closesocket(acceptingSocket);

        printf("\nBegin listening again\n\n");
    }

    printf("\n Something went wrong, listening has stopped");

    getchar();

    closeListeningSocket(listeningSocket);
    closeAcceptingSocket(acceptingSocket);
}