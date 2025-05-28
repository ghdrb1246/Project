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

#define PORT 12345
#define BUF_SIZE 1024
#define SERVER_IP "127.0.0.1"

int showMainMenu();
void sendRequest(SOCKET sock, const char *message);

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

    while (1) {
        choice = showMainMenu();

        if (choice == 1) {
            char id[50], pw[50];
            printf("ID 입력: "); fgets(id, sizeof(id), stdin);
            printf("PW 입력: "); fgets(pw, sizeof(pw), stdin);

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

    CLOSESOCKET(sock);
    
    #ifdef _WIN32
        WSACleanup();
    #endif

    return 0;
}

int showMainMenu() {
    int meunNumber = 0;
    printf("================== [ 다이어트 관리 프로그램 ] ==================\n");
    printf("-----------------------------------------------------------\n");
    printf("[ 선택 --------------------------------------------------- ]\n");

    printf("1. 회원가입\n");
    printf("2. 로그인\n");
    printf("3. 프로그램 종료\n");

    printf("-----------------------------------------------------------\n");
    
    while (1) {
        printf("메뉴 선택 : ");
        scanf("%d", &meunNumber);

        if (meunNumber < 4) break;
        else printf("%d는 메뉴 번호가 아닙니다. 다시 입력해주세요.\n", meunNumber);
    }

    getchar();
    
    printf("\n-----------------------------------------------------------\n");
    printf("===========================================================\n");

    return meunNumber;
}

void sendRequest(SOCKET sock, const char *message) {
    char recvBuf[BUF_SIZE];
    send(sock, message, strlen(message), 0);
    int len = recv(sock, recvBuf, BUF_SIZE - 1, 0);
    if (len > 0) {
        recvBuf[len] = '\0';
        printf("[응답] %s\n", recvBuf);
    }
}