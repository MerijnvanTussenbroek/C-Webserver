#ifndef LISTENINGSOCKET_H
#define LISTENINGSOCKET_H

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

SOCKET InitializeListeningSocket(struct sockaddr_in server);
void BindListeningSocket(struct sockaddr_in server, SOCKET s);
void CloseListeningSocket(SOCKET s);

#endif