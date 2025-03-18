#include <stdio.h>

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "ListeningSocket.c"
#include "Parser.c"

int main(int argc, char *argv[]);
char buffer[4096];

int main(int argc, char *argv[])
{

    char* request = "GET /index.html HTTP/1.1\r\n"
                    "Host: www.example.com\r\n"
                    "User-Agent: Mozilla/5.0\r\n"
                    "Accept: text/html\r\n"
                    "\r\n";


    printf("begin tokenizing \n");
    getchar();

    Token* tokenizedRequest = tokenizeRequest(request);

    for(int i = 0; i < 100; i ++)
    {
        printToken(tokenizedRequest[i]);
    }

    printf("Press enter to leave...");
    getchar();
    
    //SOCKET ListeningSocket;

    //struct sockaddr_in Server;

    //Server.sin_family = AF_INET;
    //Server.sin_addr.s_addr = INADDR_ANY;
    //Server.sin_port = htons( 8888 );

    //ListeningSocket = InitializeListeningSocket(Server);

    //if(ListeningSocket == INVALID_SOCKET){
    //    return 1;
    //}

    //BindListeningSocket(Server, ListeningSocket);

    //CloseListeningSocket(ListeningSocket);
    //WSACleanup();

    return 0;
}