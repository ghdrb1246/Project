#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DBM.h"
#include "UserInfo.h"

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

void handleClient(SOCKET clientSock);
void processRequest(char *request, char *response);

int main() {
    #ifdef _WIN32
        WSADATA wsa;
        WSAStartup(MAKEWORD(2, 2), &wsa);
    #endif

    SOCKET serverSock, clientSock;
    struct sockaddr_in server, client;
    
    socklen_t clientSize = sizeof(client);

    serverSock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    bind(serverSock, (struct sockaddr*)&server, sizeof(server));
    listen(serverSock, 5);

    printf("서버가 포트 %d에서 대기 중입니다...\n", PORT);
    
    DBO("Users");

    while (1) {
        clientSock = accept(serverSock, (struct sockaddr*)&client, &clientSize);
        printf("클라이언트 접속됨\n");
        handleClient(clientSock);
        CLOSESOCKET(clientSock);
    }

    // sqlite3_close(db);
    DBC();
    
    CLOSESOCKET(serverSock);

    #ifdef _WIN32
        WSACleanup();
    #endif
        return 0;
}

void handleClient(SOCKET clientSock) {
    char buf[BUF_SIZE];
    char response[BUF_SIZE];
    int len;

    while ((len = recv(clientSock, buf, BUF_SIZE - 1, 0)) > 0) {
        buf[len] = '\0';
        printf("[요청] %s\n", buf);
        processRequest(buf, response);
        send(clientSock, response, strlen(response), 0);
    }
}

void processRequest(char *request, char *response) {
    char cmd[16], arg1[32], arg2[32];
    UserSignupInfo *USI = USImalloc();

   /*  if (!USI) {
        printf("메모리 할당 실패!\n");
        return;
    } */

    sscanf(request, "%s %s %s", cmd, arg1, arg2);

    // 회원가입 처리
    if (strcmp(cmd, "SIGNUP") == 0) {
        sscanf(
            request, 
            "SIGNUP %s %s %s %d %f %f %f",
            USI->id, USI->pw, USI->gender, &USI->age,
           &USI->height, &USI->initialWeight, &USI->goalWeight
        );

        if (userExists(USI->id)) {
            sprintf(response, "[실패] 이미 존재하는 ID입니다->");
        } 
        else {
            signupUser(USI->id, USI->pw);
            sprintf(response, "[성공] 회원가입 완료");
        }
    } 
    // 로그인 처리
    else if (strcmp(cmd, "LOGIN") == 0) {
        if (loginUser(arg1, arg2)) {
            sprintf(response, "[성공] 로그인 성공");
        } 
        else {
            sprintf(response, "[실패] 로그인 실패");
        }
    } 
    else if (strcmp(cmd, "MENU") == 0) {
        sprintf(response, "[메뉴 선택] 사용자가 선택한 번호는 %s번입니다.", arg1);
    } 
    
    else if (strcmp(cmd, "INPUT_MEAL") == 0) {
        // 식단 입력
    } 
    else if (strcmp(cmd, "INPUT_WORKOUT") == 0) {
        // 운동 입력
    } 
    else if (strcmp(cmd, "INPUT_WEIGHT") == 0) {
        // 체중 입력
    } 
    else if (strcmp(cmd, "GET_RECORD") == 0) {
        // 날짜별 기록 조회
    } 
    else if (strcmp(cmd, "CHECK_PROGRESS") == 0) {
        // 감량 진행률 계산
    } 
    else if (strcmp(cmd, "FEEDBACK") == 0) {
        // 피드백 추천
    } 
    else if (strcmp(cmd, "LOGOUT") == 0) {
        // 로그아웃 처리
    } 
    else if (strcmp(cmd, "DELETE_ID") == 0) {
        // 회원 탈퇴 처리
    }
    else {
        sprintf(response, "[오류] 알 수 없는 명령어");
    }

    USIfree(USI);
}