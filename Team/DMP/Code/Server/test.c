#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite/sqlite3.h"

int main() {
    sqlite3 *db;
    sqlite3_stmt *stmt;

    sqlite3_open("Server/DB/food.db", &db);

    // 테이블 생성
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS foods (name TEXT PRIMARY KEY, kcal REAL);", 0, 0, 0);

    FILE *fp = fopen("Server/CSV/food.csv", "r");
    if (!fp) {
        printf("CSV 파일 열기 실패!\n");
        return 1;
    }

    char line[256];
    fgets(line, sizeof(line), fp);  // 헤더 라인 skip

    sqlite3_prepare_v2(db, "INSERT INTO foods (name, kcal) VALUES (?, ?);", -1, &stmt, NULL);

    while (fgets(line, sizeof(line), fp)) {
        char *name = strtok(line, ",\n");
        char *kcalStr = strtok(NULL, ",\n");
        float kcal = atof(kcalStr);

        sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 2, kcal);

        sqlite3_step(stmt);
        sqlite3_reset(stmt);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    fclose(fp);
    printf("CSV → DB 저장 완료!\n");
    return 0;
}
