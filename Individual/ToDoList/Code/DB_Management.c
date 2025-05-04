#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sqlite/sqlite3.h"
#include "DB_Management.h"

static sqlite3 *db;

Schedule smalloc() {
    Schedule s;
    s.title = (char*)malloc(100 * sizeof(char));
    s.scheduled_date_time = (char*)malloc(20 * sizeof(char));
    s.end_date_time = (char*)malloc(20 * sizeof(char));
    s.tag = (char*)malloc(10 * sizeof(char));
    s.status = (char*)malloc(5 * sizeof(char));

    return s;
}

int DBO(char *F) {
    int rc = sqlite3_open(F, &db);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "데이터베이스 열기 오류: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    else return 0;
}

void DBC() {
    sqlite3_close(db);
}

void tableDB() {
    char *sql = "CREATE TABLE schedules (id INTEGER PRIMARY KEY AUTOINCREMENT, title TEXT NOT NULL, scheduled_date_time TEXT, end_date_time TEXT, tag TEXT, priority INTEGER, status TEXT DEFAULT 'TODO');";
    char *err_msg = "0";

    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) { 
        fprintf(stderr, "테이블 생성 오류: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

void saveDB(Schedule s) {
    // ((s.end_date_time != "x") ? s.end_date_time : "NULL")?
    char *sql = sqlite3_mprintf("INSERT INTO schedules (title, scheduled_date_time, end_date_time, tag, priority) VALUES ('%s', '%s', '%s', '%s', %d);", s.title, s.scheduled_date_time, s.end_date_time, s.tag, s.priority);
    char *err_msg = "0";
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "데이터 저장 오류: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

void updateDB(Schedule s, int id) {
    char *sql = sqlite3_mprintf("UPDATE schedules SET title = '%s', scheduled_date_time = '%s', end_date_time = '%s', tag = '%s', priority = %d, status = '%s' WHERE id = %d;", s.title, s.scheduled_date_time, s.end_date_time, s.tag, s.priority, s.status, id);
    char *err_msg = "0";
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "데이터 업데이트 오류: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

void deleteDB(int id) {
    char *sql = sqlite3_mprintf("DELETE FROM employee WHERE id = %d;", id);
    char *err_msg = "0";
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "데이터 삽입 오류: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

void viewDB() {
    char *sql = "SELECT * FROM schedules WHERE status;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

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

    else fprintf(stderr, "데이터 조회 오류: %s\n", sqlite3_errmsg(db));   
}

void viewAllByStatus(char *status) {
    char *sql = sqlite3_mprintf("SELECT ROW_NUMBER() OVER (ORDER BY scheduled_date_time) AS no, title, scheduled_date_time, end_date_time, tag, priority, status FROM schedules WHERE status = '%s';", status);
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int no = sqlite3_column_int(stmt, 0);
            const unsigned char *title = sqlite3_column_text(stmt, 1);
            const unsigned char *sdt = sqlite3_column_text(stmt, 2);
            const unsigned char *edt = sqlite3_column_text(stmt, 3);
            const unsigned char *tag = sqlite3_column_text(stmt, 4);
            int priority = sqlite3_column_int(stmt, 5);
            const unsigned char *status = sqlite3_column_text(stmt, 6);
            
            // edt "NULL" x -> NULL in
            printf("%2d | %s | %s | %s | %s | %d | %s\n", no, title, sdt, edt, tag, priority, status);
        }
        sqlite3_finalize(stmt);
    }

    else fprintf(stderr, "데이터 조회 오류: %s\n", sqlite3_errmsg(db));
}

int statusIndexToId(char *status, int user_no) {
    int real_id = -1;
    char *sql = sqlite3_mprintf("SELECT id FROM (SELECT ROW_NUMBER() OVER (ORDER BY scheduled_date_time) AS no, id FROM schedules WHERE status = '%s') WHERE no = %d;", status, user_no);
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            real_id = sqlite3_column_int(stmt, 0);
        }
        else printf("%d 해당 번호의 일정이 존재하지 않습니다.\n", real_id);
    }
    else printf("SQL 실행 실패 : %s\n", sqlite3_errmsg(db));

    return real_id;
}

Schedule idToStatusView(int id) {
    char *sql = sqlite3_mprintf("SELECT title, scheduled_date_time, end_date_time, tag, priority FROM schedules WHERE id = %d;", id);
    sqlite3_stmt *stmt;
    Schedule s = smalloc();
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char *title = sqlite3_column_text(stmt, 0);
            const unsigned char *sdt = sqlite3_column_text(stmt, 1);
            const unsigned char *edt = sqlite3_column_text(stmt, 2);
            const unsigned char *tag = sqlite3_column_text(stmt, 3);
            int priority = sqlite3_column_int(stmt, 4);

            s.title = (char*)title;
            s.scheduled_date_time = (char*)sdt;
            s.end_date_time = (char*)edt;
            s.tag = (char*)tag;
            s.priority = priority;

            // printf("%s, %s, %s, %s, %d\n", title, sdt, edt, tag, priority);
        }
        sqlite3_finalize(stmt);
    }
    else fprintf(stderr, "데이터 조회 오류: %s\n", sqlite3_errmsg(db)); 

    return s;
}

void todoToDoing(int id) {
    char *sql = sqlite3_mprintf("UPDATE schedules SET status = 'DOING' WHERE id = %d;", id);
    char *err_msg = "0";
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "데이터 업데이트 오류: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

void doingToDone(int id) {
    char *sql = sqlite3_mprintf("UPDATE schedules SET status = 'DONE' WHERE id = %d;", id);
    char *err_msg = "0";
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "데이터 업데이트 오류: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

void viewAllByTag() {
    char *sql = sqlite3_mprintf("SELECT ROW_NUMBER() OVER (ORDER BY scheduled_date_time) AS no, tag FROM schedules;");
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int on = sqlite3_column_int(stmt, 0);
            const unsigned char *tag = sqlite3_column_text(stmt, 1);

            printf("%d | %s\n", on, tag);
        }
        sqlite3_finalize(stmt);
    }

    else fprintf(stderr, "데이터 조회 오류: %s\n", sqlite3_errmsg(db));
}

char *tagIndexToId(int user_no) {
    const unsigned char *real_tag = NULL;
    // char *ret_tag = NULL;
    char *sql = sqlite3_mprintf("SELECT tag FROM (SELECT ROW_NUMBER() OVER (ORDER BY scheduled_date_time) AS no, tag FROM schedules) WHERE no = %d;", user_no);
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            real_tag = sqlite3_column_text(stmt, 0);
        }
        else printf("%s 해당 번호의 일정이 존재하지 않습니다.\n", real_tag);
    }
    else printf("SQL 실행 실패 : %s\n", sqlite3_errmsg(db));

    // memcpy(ret_tag real_tag);
    return (char*)real_tag;
}

int tagCount(char *tag) {
    int real_count = 0;
    char *sql = sqlite3_mprintf("SELECT tag, COUNT(*) AS count FROM schedules WHERE tag = '%s';", tag);
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            real_count = sqlite3_column_int(stmt, 0);
        }
        else printf("%d 해당 번호의 태그가 존재하지 않습니다.\n", real_count);
    }
    else printf("SQL 실행 실패 : %s\n", sqlite3_errmsg(db));

    return real_count;
}

void viewTagByschedule(char *tag) {
    char *sql = sqlite3_mprintf("SELECT * FROM schedules WHERE tag = '%s';", tag);
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

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

    else fprintf(stderr, "데이터 조회 오류: %s\n", sqlite3_errmsg(db));
}