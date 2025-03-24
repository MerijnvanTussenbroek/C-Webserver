
#ifndef LISTENINGSOCKET_H
#define LISTENINGSOCKET_H

#include <winsock2.h>

SOCKET initializeListeningSocket();
void bindListeningSocket(struct sockaddr_in server, SOCKET s);
SOCKET initializeAcceptingSocket(struct sockaddr_in server, SOCKET listeningSocket);
void closeListeningSocket(SOCKET s);
void closeAcceptingSocket(SOCKET s);

#endif