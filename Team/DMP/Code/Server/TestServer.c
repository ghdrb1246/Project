#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "sqlite/sqlite3.h"

#define DB_FILE "Server/DB/Users.db"

void sendRecordsAsText(int clientSock, sqlite3 *db, const char *userId, const char *date) {
    char response[2048] = "";

    sqlite3_stmt *stmt;

    // 식단 기록
    strcat(response, "MEAL:");
    const char *sqlMeal = "SELECT time(datetime), food_name, gram, kcal FROM meal WHERE user_id=? AND date(datetime)=?;";
    if (sqlite3_prepare_v2(db, sqlMeal, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, userId, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, date, -1, SQLITE_STATIC);

        int first = 1;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            if (!first) strcat(response, "|");
            char entry[128];
            snprintf(entry, sizeof(entry), "%s %s %.1fg %.1fkcal",
                sqlite3_column_text(stmt, 0),
                sqlite3_column_text(stmt, 1),
                sqlite3_column_double(stmt, 2),
                sqlite3_column_double(stmt, 3));
            strcat(response, entry);
            first = 0;
        }
        sqlite3_finalize(stmt);
    }
    else {
        printf("Err\n");
    }

    strcat(response, "#");

    // 운동 기록
    strcat(response, "WORKOUT:");
    const char *sqlWorkout = "SELECT time(datetime), exercise_name, minutes, kcal FROM workout WHERE user_id=? AND date(datetime)=?;";
    if (sqlite3_prepare_v2(db, sqlWorkout, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, userId, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, date, -1, SQLITE_STATIC);

        int first = 1;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            if (!first) strcat(response, "|");
            char entry[128];
            snprintf(entry, sizeof(entry), "%s %s %.1fh %.1fkcal",
                sqlite3_column_text(stmt, 0),
                sqlite3_column_text(stmt, 1),
                sqlite3_column_double(stmt, 2),
                sqlite3_column_double(stmt, 3));
            strcat(response, entry);
            first = 0;
        }
        sqlite3_finalize(stmt);
    }
    else {
        printf("Err\n");
    }
    strcat(response, "#");

    // 체중 기록
    strcat(response, "WEIGHT:");
    const char *sqlWeight = "SELECT weight FROM weightRecord WHERE user_id=? AND date=?;";
    if (sqlite3_prepare_v2(db, sqlWeight, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, userId, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, date, -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            char entry[32];
            snprintf(entry, sizeof(entry), "%.1fkg", sqlite3_column_double(stmt, 0));
            strcat(response, entry);
        } else {
            strcat(response, "기록없음");
        }
        sqlite3_finalize(stmt);
    }
    else {
        printf("Err\n");
    }

    send(clientSock, response, strlen(response), 0);
}

int main() {
    sqlite3 *db;
    sqlite3_open(DB_FILE, &db);

    int servSock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servAddr = {0}, cliAddr = {0};
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(9000);
    servAddr.sin_addr.s_addr = INADDR_ANY;

    bind(servSock, (struct sockaddr*)&servAddr, sizeof(servAddr));
    listen(servSock, 5);

    printf("서버 시작: 9000 포트 대기 중...\n");

    socklen_t cliSize = sizeof(cliAddr);
    int clientSock = accept(servSock, (struct sockaddr*)&cliAddr, &cliSize);

    char recvBuf[128];
    int len = recv(clientSock, recvBuf, sizeof(recvBuf)-1, 0);
    recvBuf[len] = '\0';
    printf("요청: %s\n", recvBuf);

    char userId[50], date[11];
    sscanf(recvBuf, "%[^/]/%s", userId, date);

    sendRecordsAsText(clientSock, db, userId, date);

    close(clientSock);
    close(servSock);
    sqlite3_close(db);
    return 0;
}
