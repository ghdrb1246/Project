#include <stdio.h>
#include <string.h>
#include "InputHandler.h"
#include "ClientController.h"

#ifdef _WIN32
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
#endif

void sendRequest(int sock, const char *message) {
    char recvBuf[1024];
    send(sock, message, strlen(message), 0);
    int len = recv(sock, recvBuf, 1023, 0);
    if (len > 0) {
        recvBuf[len] = '\0';
        printf("[응답] %s\n", recvBuf);
    }
}

MenuState handleMainMenu(int sock) {
    int choice = inputMenu();
    char sendBuf[1024];
    char id[50], pw[50];

    switch (choice) {
        case 1:
            inputCredentials(id, pw);
            sprintf(sendBuf, "SIGNUP %s %s", id, pw);
            sendRequest(sock, sendBuf);
            return STATE_MAIN_MENU;
        case 2:
            inputCredentials(id, pw);
            sprintf(sendBuf, "LOGIN %s %s", id, pw);
            sendRequest(sock, sendBuf);
            return STATE_USER_MENU;
        case 3:
            printf("프로그램을 종료합니다.\n");
            return STATE_EXIT;
        default:
            sprintf(sendBuf, "MENU %d", choice);
            sendRequest(sock, sendBuf);
            return STATE_MAIN_MENU;
    }
}

MenuState handleUserMenu(int sock) {
    userMenu();
    
    int choice = inputMenu();
    char sendBuf[256];

    switch (choice) {
        case 1:
            sprintf(sendBuf, "GET_RECORD");
            sendRequest(sock, sendBuf);
            return STATE_USER_MENU;
        case 2:
            printf("로그아웃합니다.\n");
            return STATE_MAIN_MENU;
        case 0:
            return STATE_MAIN_MENU;
        default:
            printf("잘못된 선택입니다.\n");
            return STATE_USER_MENU;
    }
}