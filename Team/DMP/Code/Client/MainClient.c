#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
    #define CLOSESOCKET closesocket
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #define SOCKET int
    #define CLOSESOCKET close
#endif

#include "MenuIO.h"
#include "ClientController.h"
#include "MenuState.h"

#define PORT 12345
#define SERVER_IP "127.0.0.1"

int main() {
    #ifdef _WIN32
        WSADATA wsa;
        WSAStartup(MAKEWORD(2, 2), &wsa);
    #endif

    SOCKET sock;
    struct sockaddr_in server;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        perror("서버 연결 실패");
        return 1;
    }

    // 메뉴 이동
    MenuState state = STATE_MAIN_MENU;
    while (state != STATE_EXIT) {
        switch (state) {
            case STATE_MAIN_MENU :             state = handleMainMenu(sock); break;
            case STATE_USER_MENU :             state = handleUserMenu(sock); break;
            default: state = STATE_EXIT;                                     break;
        }
    }

    CLOSESOCKET(sock);
    
    #ifdef _WIN32
        WSACleanup();
    #endif
    
    return 0;
}