#ifndef _MS_
#define _MS_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DBM.h"
#include "InputInfo.h"
#include "CsvDB.h"

#ifdef _WIN32
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
    typedef int socklen_t;
    #define CLOSESOCKET closesocket
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #define SOCKET int
    #define CLOSESOCKET close
#endif

void handleClient(SOCKET clientSock);
void processRequest(SOCKET clientSock, char *request, char *response);

#endif