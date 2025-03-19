#ifndef LISTENINGSOCKET_H
#define LISTENINGSOCKET_H

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

void initializeListeningSocket(struct sockaddr_in server);
void bindListeningSocket(struct sockaddr_in server, SOCKET s);
void initializeAcceptingSocket(struct sockaddr_in server, SOCKET acceptingSocket, SOCKET listeningSocket);
void closeListeningSocket(SOCKET s);
void closeAcceptingSocket(SOCKET s);

#endif