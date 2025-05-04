#include <stdio.h>
#include <sqlite3.h>

int main() {
    sqlite3 *db;
    char *err_msg = 0;
    char *sql = NULL;
    // 데이터베이스 열기
    int rc = sqlite3_open("Schedule.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "데이터베이스 열기 오류: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    
    // 테이블 생성
    sql = "CREATE TABLE schedules (id INTEGER PRIMARY KEY AUTOINCREMENT, title TEXT NOT NULL, scheduled_date_time TEXT, end_date_time TEXT, tag TEXT, priority INTEGER, status TEXT DEFAULT 'TODO');";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) { 
        fprintf(stderr, "테이블 생성 오류: %s\n", err_msg);
        sqlite3_free(err_msg);
    }

    // 데이터 삽입
    sql = "INSERT INTO schedules (title, scheduled_date_time, end_date_time, tag, priority) VALUES ('과제 제출', '2025-04-21 23:59', NULL, '수업', 3);";
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "데이터 삽입 오류: %s\n", err_msg);
        sqlite3_free(err_msg);
    }

    // 데이터 조회
    sql = "SELECT * FROM schedules;";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            const unsigned char *title = sqlite3_column_text(stmt, 1);
            const unsigned char *sdt = sqlite3_column_text(stmt, 2);
            const unsigned char *edt = sqlite3_column_text(stmt, 3);
            const unsigned char *tag = sqlite3_column_text(stmt, 4);
            int priority = sqlite3_column_int(stmt, 5);

            printf("ID: %d, title: %s, sdt: %s, edt: %s, tag: %s, priority %d\n", id, title, sdt, edt, tag, priority);
        }
        sqlite3_finalize(stmt);
    }

    else {
        fprintf(stderr, "데이터 조회 오류: %s\n", sqlite3_errmsg(db));
    }

    // 데이터베이스 닫기
    sqlite3_close(db);

    return 0;
}

/**
 * @brief DB 열기
 * 
 * @param F 파일.db
 * @param db 참조할 DB
 * @return int 0: 열기 성공, 1: 열기 실패
 */
int DBO(char *F, sqlite3 *db) {
    int rc = sqlite3_open(F, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "데이터베이스 열기 오류: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    else return 0;
}