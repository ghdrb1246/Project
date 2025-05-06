#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sqlite/sqlite3.h"
#include "DB_Management.h"

// DB 관리 모듈

static sqlite3 *db;

Schedule smalloc() {
    Schedule s;
    
    s.title = malloc(100 * sizeof(char));
    s.scheduled_date_time = malloc(20 * sizeof(char));
    s.end_date_time = malloc(20 * sizeof(char));
    s.tag = malloc(10 * sizeof(char));
    s.status = malloc(5 * sizeof(char));

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

    sqlite3_free(sql);
}

void updateDB(Schedule s, int id) {
    char *sql = sqlite3_mprintf("UPDATE schedules SET title = '%s', scheduled_date_time = '%s', end_date_time = '%s', tag = '%s', priority = %d WHERE id = %d;", s.title, s.scheduled_date_time, s.end_date_time, s.tag, s.priority, id);
    char *err_msg = "0";
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "데이터 업데이트 오류: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    sqlite3_free(sql);
}

void deleteDB(int id) {
    char *sql = sqlite3_mprintf("DELETE FROM schedules WHERE id = %d;", id);
    char *err_msg = "0";
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "데이터 삽입 오류: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    sqlite3_free(sql);
}

void viewDB() {
    sqlite3_stmt *stmt;
    char *sql = "SELECT * FROM schedules WHERE status;";
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
    sqlite3_stmt *stmt;
    char *sql = sqlite3_mprintf("SELECT ROW_NUMBER() OVER (ORDER BY scheduled_date_time) AS no, title, scheduled_date_time, end_date_time, tag, priority, status FROM schedules WHERE status = '%s';", status);
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
        sqlite3_free(sql);
    }

    else fprintf(stderr, "데이터 조회 오류: %s\n", sqlite3_errmsg(db));
}

int statusIndexToId(char *status, int user_no) {
    int real_id = -1;

    if (DBO("DB/DBSC.db") != 0) {
        printf("DB 연결이 되어 있지 않습니다.\n");
        return real_id;
    }

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

    sqlite3_finalize(stmt);
    sqlite3_free(sql);

    return real_id;
}

Schedule idToStatusView(int id) {
    sqlite3_stmt *stmt;
    Schedule s = smalloc();
    char *sql = sqlite3_mprintf("SELECT title, scheduled_date_time, end_date_time, tag, priority FROM schedules WHERE id = %d;", id);

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char *title = sqlite3_column_text(stmt, 0);
            const unsigned char *sdt = sqlite3_column_text(stmt, 1);
            const unsigned char *edt = sqlite3_column_text(stmt, 2);
            const unsigned char *tag = sqlite3_column_text(stmt, 3);
            int priority = sqlite3_column_int(stmt, 4);

            // NULL 체크 필요
            strcpy(s.title, (((char*)title) != NULL) ? (char*)title : "NULL");
            strcpy(s.scheduled_date_time, (((char*)sdt) != NULL) ? (char*)sdt : "NULL");
            strcpy(s.end_date_time, (((char*)edt) != NULL) ? (char*)edt : "NULL");
            strcpy(s.tag, (((char*)tag) != NULL) ? (char*)tag : "NULL");
            s.priority = priority;

            // printf("%s, %s, %s, %s, %d\n", s.title, s.scheduled_date_time, s.end_date_time, s.tag, s.priority);
        }
        sqlite3_finalize(stmt);
        sqlite3_free(sql);
    }
    else fprintf(stderr, "데이터 조회 오류: %s\n", sqlite3_errmsg(db)); 

    return s;
}

void updateStatus(char *status, int id) {
    char *sql = sqlite3_mprintf("UPDATE schedules SET status = '%s' WHERE id = %d;", status, id);
    char *err_msg = "0";
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "데이터 업데이트 오류: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    
    sqlite3_free(sql);
}

void viewAllByTag() {
    sqlite3_stmt *stmt;
    char *sql = sqlite3_mprintf("SELECT ROW_NUMBER() OVER (ORDER BY tag) AS no, tag FROM (SELECT DISTINCT tag FROM schedules);");
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int on = sqlite3_column_int(stmt, 0);
            const unsigned char *tag = sqlite3_column_text(stmt, 1);

            printf("%2d | %s\n", on, tag);
        }

        sqlite3_finalize(stmt);
        sqlite3_free(sql);
    }

    else fprintf(stderr, "데이터 조회 오류: %s\n", sqlite3_errmsg(db));
}

/*
// tagIndexToId(), tagCount()을 indexToTagCount()으로 통합
char *tagIndexToId(int user_no) {
    sqlite3_stmt *stmt;
    
    const unsigned char *tag = NULL;
    char *tag_r = malloc(10 * sizeof(char));
    char *sql = sqlite3_mprintf("SELECT tag FROM (SELECT ROW_NUMBER() OVER (ORDER BY tag) AS no, tag FROM schedules) WHERE no = %d;", user_no);
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            tag = sqlite3_column_text(stmt, 0);

            strcpy(tag_r, (((char*)tag) != NULL) ? (char*)tag : "NULL");
        }
        else printf("%s 해당 번호의 태그가 존재하지 않습니다.\n", tag);
    }
    else printf("SQL 실행 실패 : %s\n", sqlite3_errmsg(db));

    sqlite3_finalize(stmt);
    sqlite3_free(sql);
    return tag_r;
}

int tagCount(char *tag) {
    sqlite3_stmt *stmt;
    
    char *sql = sqlite3_mprintf("SELECT COUNT(*) AS count FROM schedules WHERE tag = '%s';", tag);
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    int count = 0;

    if (rc == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            count = sqlite3_column_int(stmt, 0);
        }
        else printf("%d 해당 번호의 태그가 존재하지 않습니다.\n", count);
    }
    else printf("SQL 실행 실패 : %s\n", sqlite3_errmsg(db));

    sqlite3_finalize(stmt);
    sqlite3_free(sql);

    return count;
}
*/

TagCount indexToTagCount(int user_no) {
    sqlite3_stmt *stmt;
    TagCount tc;
    tc.tag = malloc(10 * sizeof(char));
    
    const unsigned char *tag = NULL;
    char *sql = sqlite3_mprintf("SELECT temp_tag.tag, (SELECT COUNT(*) FROM schedules WHERE tag = temp_tag.tag) AS count FROM (SELECT tag FROM (SELECT ROW_NUMBER() OVER (ORDER BY tag) AS no, tag FROM (SELECT DISTINCT tag FROM schedules WHERE tag IS NOT NULL)) WHERE no = %d) AS temp_tag;", user_no);
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    int count = 0;
    
    if (rc == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            tag = sqlite3_column_text(stmt, 0);
            count = sqlite3_column_int(stmt, 1);

            strcpy(tc.tag, (((char*)tag) != NULL) ? (char*)tag : "NULL");
            tc.count = count;
            printf("-> %s %d\n", tc.tag, tc.count);
        }
        else printf("%d 해당 번호의 태그가 존재하지 않습니다.\n", count);
    }
    else printf("SQL 실행 실패 : %s\n", sqlite3_errmsg(db));

    sqlite3_finalize(stmt);
    sqlite3_free(sql);

    return tc;
}

void viewTagByschedule(char *tag) {
    sqlite3_stmt *stmt;
    char *sql = sqlite3_mprintf("SELECT * FROM schedules WHERE tag = '%s';", tag);
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            const unsigned char *title = sqlite3_column_text(stmt, 1);
            const unsigned char *sdt = sqlite3_column_text(stmt, 2);
            const unsigned char *edt = sqlite3_column_text(stmt, 3);
            const unsigned char *tag = sqlite3_column_text(stmt, 4);
            int priority = sqlite3_column_int(stmt, 5);

            printf("%2d | %s | %s | %s | %s | %d\n", id, title, sdt, edt, tag, priority);
        }
        sqlite3_finalize(stmt);
        sqlite3_free(sql);
    }

    else fprintf(stderr, "데이터 조회 오류: %s\n", sqlite3_errmsg(db));
}