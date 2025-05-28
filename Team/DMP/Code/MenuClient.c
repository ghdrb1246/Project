#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MenuClient.h"
#include "MumeIO.h"

#ifdef _WIN32
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #define CLOSESOCKET close
#endif

int main() {
    #ifdef _WIN32
        WSADATA wsa;
        WSAStartup(MAKEWORD(2, 2), &wsa);
    #endif

    SOCKET sock;
    struct sockaddr_in server;
    char sendBuf[BUF_SIZE], recvBuf[BUF_SIZE];
    int choice;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        perror("서버 연결 실패");
        return 1;
    }

    /* 
    while (1) {
        choice = showMainMenu();

        if (choice == 1) {
            char id[50], pw[50];
            printf("ID 입력: "); 
            fgets(id, sizeof(id), stdin);
            
            printf("PW 입력: "); 
            fgets(pw, sizeof(pw), stdin);

            id[strcspn(id, "\n")] = 0;
            pw[strcspn(pw, "\n")] = 0;
            
            sprintf(sendBuf, "SIGNUP %s %s", id, pw);
            sendRequest(sock, sendBuf);
        } 
        else if (choice == 2) {
            char id[50], pw[50];

            printf("ID 입력: "); fgets(id, sizeof(id), stdin);
            printf("PW 입력: "); fgets(pw, sizeof(pw), stdin);
            
            id[strcspn(id, "\n")] = 0;
            pw[strcspn(pw, "\n")] = 0;
            
            sprintf(sendBuf, "LOGIN %s %s", id, pw);
            sendRequest(sock, sendBuf);
        } 
        else if (choice == 3) {
            printf("프로그램을 종료합니다.\n");
            break;
        } 
        else {
            printf("Err\n");
        }
    } 
    */

    runMenu();

    CLOSESOCKET(sock);
    
    #ifdef _WIN32
        WSACleanup();
    #endif

    return 0;
}

void sendRequest(SOCKET sock, const char *message) {
    printf("sendRequest\n");
    char recvBuf[BUF_SIZE];
    send(sock, message, strlen(message), 0);
    int len = recv(sock, recvBuf, BUF_SIZE - 1, 0);

    if (len > 0) {
        recvBuf[len] = '\0';
        printf("[응답] %s\n", recvBuf);
    }
}