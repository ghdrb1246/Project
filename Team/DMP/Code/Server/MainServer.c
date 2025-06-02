#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DBM.h"
#include "InputInfo.h"

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
    char cmd[16];
    UserSignupInfo *USI = USImalloc();
    MealInputInfo *MII = MIImalloc();
    WeightInputInfo *WII = WIImalloc();
    WorkOutInputInfo *WOII = WOIImalloc();

    sscanf(request, "%[^/]", cmd);

   /*  if (!USI) {
        printf("메모리 할당 실패!\n");
        return;
    } */

    // 회원가입 처리
    if (strcmp(cmd, "SIGNUP") == 0) {
        sscanf(
            request, 
            "SIGNUP/%[^/]/%[^/]/%[^/]/%d/%f/%f/%f",
            USI->id, USI->pw, USI->gender, &USI->age,
           &USI->height, &USI->initialWeight, &USI->goalWeight
        );

        printf("-> %s %s %s %d %f %f %f\n", USI->id, USI->pw, USI->gender, USI->age, USI->height, USI->initialWeight, USI->goalWeight);

        if (userExists(USI->id)) {
            sprintf(response, "[실패] 이미 존재하는 ID입니다->");
        } 
        else {
            signupUser(USI);
            sprintf(response, "[성공] 회원가입 완료");
        }
        USIfree(USI);
    } 

    // 로그인 처리
    else if (strcmp(cmd, "LOGIN") == 0) {
        sscanf(
            request, 
            "LOGIN/%[^/]/%s", 
            USI->id, USI->pw
        );
        
        if (loginUser(USI->id, USI->pw)) {
            sprintf(response, "[성공] 로그인 성공");
        } 
        else {
            sprintf(response, "[실패] 로그인 실패");
        }
        USIfree(USI);
    } 
    
    /* 입력 함수 -> */

    // 식단 입력
    else if (strcmp(cmd, "INPUT_MEAL") == 0) {
        sscanf(
            request, 
            "INPUT_MEAL/%[^/]/%[^/]/%[^/]/%f", 
            MII->userId, MII->dateTime, MII->foodName, &MII->gram
        );

        printf("식단 입력 치리 -> %s %s %s %.1f\n", MII->userId, MII->dateTime, MII->foodName, MII->gram);
        insertMeal(MII);

        MIIfree(MII);
    } 
    // 운동 입력
    else if (strcmp(cmd, "INPUT_WORKOUT") == 0) {
        sscanf(
            request,
            "INPUT_WORKOUT/%[^/]/%[^/]/%[^/]/%f",
            WOII->userId, WOII->dateTime, WOII->exerciseName, &WOII->hour
        );

        printf("운동 입력 치리 -> %s %s %s %.1f\n", WOII->userId ,WOII->dateTime, WOII->exerciseName, WOII->hour);
        insertWorkout(WOII);

        WOIIfree(WOII);
    } 
    // 체중 입력
    else if (strcmp(cmd, "INPUT_WEIGHT") == 0) {
        sscanf(
            request,
            "INPUT_WEIGHT/%[^/]/%[^/]/%f",
            WII->userId, WII->date, &WII->weight
        );

        printf("체중 입력 치리 -> %s %s %.1f\n", WII->userId, WII->date, WII->weight);
        insertWeight(WII);

        WIIfree(WII);
    } 
    
    /* 조회 함수 -> */

    // 날짜별 기록 조회
    else if (strcmp(cmd, "GET_RECORD") == 0) {
        printf("날짜별 기록 조회\n");
    } 
    // 피드백 추천
    else if (strcmp(cmd, "FEEDBACK") == 0) {
        printf("피드백 추천\n");
    } 
    
    /* 처리 함수 -> */

    // 감량 진행률 계산
    else if (strcmp(cmd, "CHECK_PROGRESS") == 0) {
        printf("감량 진행률 계산\n");
    } 
    // 로그아웃 처리
    else if (strcmp(cmd, "LOGOUT") == 0) {
        printf("로그아웃 처리\n");
    } 
    // 회원 탈퇴 처리
    else if (strcmp(cmd, "DELETE_ID") == 0) {
        printf("회원 탈퇴 처리 해당 사용자 DB 삭제\n");
    }
    
    else {
        sprintf(response, "[오류] 알 수 없는 명령어");
    }
}