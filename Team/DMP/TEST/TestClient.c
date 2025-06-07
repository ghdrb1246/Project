#define _GNU_SOURCE  // strtok_r 사용
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servAddr = {0};
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(9000);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr*)&servAddr, sizeof(servAddr));

    // 사용자 ID / 날짜 입력
    char userId[50], date[11];
    printf("사용자 ID: ");
    scanf("%s", userId);
    printf("조회할 날짜 (YYYY-MM-DD): ");
    scanf("%s", date);

    // 요청 전송
    char sendBuf[128];
    snprintf(sendBuf, sizeof(sendBuf), "%s/%s", userId, date);
    send(sock, sendBuf, strlen(sendBuf), 0);

    // 서버 응답 수신
    char recvBuf[2048];
    int len = recv(sock, recvBuf, sizeof(recvBuf)-1, 0);
    recvBuf[len] = '\0';

    // 출력 시작
    printf("\n===============================\n");

    char *saveptr1;
    char *section = strtok_r(recvBuf, "#", &saveptr1);

    while (section) {
        if (strncmp(section, "MEAL:", 5) == 0) {
            printf("식단 ----\n");
            char *data = section + 5;
            char *saveptr2;
            char *entry = strtok_r(data, "|", &saveptr2);
            int mealCount = 1;
            while (entry) {
                printf("%d. %s\n", mealCount++, entry);
                entry = strtok_r(NULL, "|", &saveptr2);
            }
            printf("-------------------------------\n");
        } 
        else if (strncmp(section, "WORKOUT:", 8) == 0) {
            printf("운동 ----\n");
            char *data = section + 8;
            char *saveptr2;
            char *entry = strtok_r(data, "|", &saveptr2);
            int workoutCount = 1;
            while (entry) {
                printf("%d. %s\n", workoutCount++, entry);
                entry = strtok_r(NULL, "|", &saveptr2);
            }
            printf("-------------------------------\n");
        } 
        else if (strncmp(section, "WEIGHT:", 7) == 0) {
            printf("체중 ----\n%s\n", section + 7);
            printf("-------------------------------\n");
        }
        section = strtok_r(NULL, "#", &saveptr1);
    }

    close(sock);
    return 0;
}
