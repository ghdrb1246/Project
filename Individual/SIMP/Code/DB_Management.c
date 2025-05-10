#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h> // mkdir

#include "sqlite/sqlite3.h"
#include "DB_Management.h"
#include "UserStructure.h"
#include "Utils.h"

#ifdef _WIN32
    // Windows 환경 
    #include <direct.h>
    #define mkdir(dir, mode) _mkdir(dir)

#else 
    // macOS 환경
    #include <sys/types.h>
    #include <unistd.h>
#endif

// DB 관리 모듈

static sqlite3 *db;

int DBO(const char *filename) {
    const char *foldername = "DB";  // 폴더 이름
    char path[256];                 // 파일 경로 생성 문자열
    int mode = 0755;                // 생성하려는 디렉터리에 대한 접근 권한 설정 값
    
    // DB 폴더 확인 및 생성
    #ifdef _WIN32
        // Windows 환경일 때
        struct _stat st = { 0 };
        if (_stat(foldername, &st) != 0) {
            if (mkdir(foldername, mode) != 0) {
                perror("DB 폴더 생성 실패");
                return 1;
            }
        }
    #else
        // macOS 환경일 때
        struct stat st = { 0 };
        if (stat(foldername, &st) != 0) {
            if (mkdir(foldername, mode) != 0) {
                perror("DB 폴더 생성 실패");
                return 1;
            }
        }
    #endif

    // 파일 경로 생성
    snprintf(path, sizeof(path), "%s/%s.db", foldername, filename);

    // SQLite DB 열기 또는 생성
    if (sqlite3_open(path, &db) != SQLITE_OK) {
        fprintf(stderr, "DB 열기 실패: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // 테이블이 존재하면 1, 존재하지 않으면 0이 반환
    if (tableExists() != 1) tableDB();

    return 0; // 성공
}

void DBC() {
    sqlite3_close(db);
}

int tableExists() {
    sqlite3_stmt *stmt;
    char *sql = "SELECT COUNT(*) FROM sqlite_master WHERE type = 'table' AND name = 'schedules';";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    int real_id = -1;
    
    if (rc == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            real_id = sqlite3_column_int(stmt, 0);
        }
        else fprintf(stderr, "'%s' 해당 파일에 테이블이 존재하지 않습니다\n", sqlite3_errmsg(db));

        sqlite3_finalize(stmt);
    }
    else printf("SQL 실행 실패 : %s\n", sqlite3_errmsg(db));

    return real_id;
}

void tableDB() {
    char *sql = "CREATE TABLE schedules (id INTEGER PRIMARY KEY AUTOINCREMENT, title TEXT NOT NULL, scheduled_date_time TEXT, end_date_time TEXT, tag TEXT, priority INTEGER, status TEXT DEFAULT 'TODO');";
    char *err_msg = 0;

    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) { 
        fprintf(stderr, "테이블 생성 오류: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

void saveDB(Schedule *s) {
    // ((s.end_date_time != "x") ? s.end_date_time : "NULL")?
    char *sql = sqlite3_mprintf("INSERT INTO schedules (title, scheduled_date_time, end_date_time, tag, priority) VALUES ('%s', '%s', '%s', '%s', %d);", s->title, s->scheduled_date_time, s->end_date_time, s->tag, s->priority);
    char *err_msg = "0";
    
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "데이터 저장 오류: %s\n", err_msg);
        sqlite3_free(err_msg);
    }

    sqlite3_free(sql);
}

void updateDB(Schedule *s, int id) {
    char *sql = sqlite3_mprintf("UPDATE schedules SET title = '%s', scheduled_date_time = '%s', end_date_time = '%s', tag = '%s', priority = %d WHERE id = %d;", s->title, s->scheduled_date_time, s->end_date_time, s->tag, s->priority, id);
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

void checkScheduleStatus() {
    Schedule *s = smalloc();
    sqlite3_stmt *stmt;
    char *sql = "SELECT * FROM schedules WHERE status = 'TODO';";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0), id_c = 0;

    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {    
            int id = sqlite3_column_int(stmt, 0);
            const unsigned char *title = sqlite3_column_text(stmt, 1);
            const unsigned char *sdt = sqlite3_column_text(stmt, 2);
            const unsigned char *edt = sqlite3_column_text(stmt, 3);
            const unsigned char *tag = sqlite3_column_text(stmt, 4);
            int priority = sqlite3_column_int(stmt, 5);
            const unsigned char *status = sqlite3_column_text(stmt, 6);
            
            // NULL 체크 필요
            strcpy(s->title, (((char*)title) != NULL) ? (char*)title : "NULL");
            strcpy(s->scheduled_date_time, (((char*)sdt) != NULL) ? (char*)sdt : "NULL");
            strcpy(s->end_date_time, (((char*)edt) != NULL) ? (char*)edt : "NULL");
            strcpy(s->tag, (((char*)tag) != NULL) ? (char*)tag : "NULL");
            s->priority = priority;
            strcpy(s->status, (((char*)status) != NULL) ? (char*)status : "NULL");

            id_c = updateScheduleStatus(s, id);
            if (id_c > 0) {
                printf("%d\n", id_c);
                updateStatus("DOING", id_c);
            }
            // printf("ID: %d, title: %s, sdt: %s, edt: %s, tag: %s, priority %d\n", id, title, sdt, edt, tag, priority);
        }
        
        // SQL문이 NULL일때 동적 할당 해제 에러 방지
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            sqlite3_finalize(stmt);
            sqlite3_free(sql);
        }
    }

    else fprintf(stderr, "데이터 조회 오류: %s\n", sqlite3_errmsg(db));   
}

void viewAllByStatus(const char *status) {
    sqlite3_stmt *stmt;
    // char *sql = sqlite3_mprintf("SELECT ROW_NUMBER() OVER (ORDER BY scheduled_date_time) AS no, title, scheduled_date_time, end_date_time, tag, priority, status FROM schedules WHERE status = '%s';", status);
    char *sql = sqlite3_mprintf("SELECT ROW_NUMBER() OVER (ORDER BY priority DESC, scheduled_date_time ASC, id ASC) AS no, CASE WHEN priority = 1 THEN '! ' || title WHEN priority = 2 THEN '!! ' || title WHEN priority = 3 THEN '!!! ' || title ELSE title END AS priority_title, scheduled_date_time, end_date_time, tag, status FROM schedules WHERE status = '%s';", status);
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0), sw = 0;

    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            sw = 1;
            int no = sqlite3_column_int(stmt, 0);
            const unsigned char *title = sqlite3_column_text(stmt, 1);
            const unsigned char *sdt = sqlite3_column_text(stmt, 2);
            const unsigned char *edt = sqlite3_column_text(stmt, 3);
            const unsigned char *tag = sqlite3_column_text(stmt, 4);
            const unsigned char *status = sqlite3_column_text(stmt, 5);
            
            // edt "NULL" x -> NULL in
            printf("%2d | %s | %s | %s | %s | %s\n", no, title, sdt, edt, tag, status);
        }

        // SQL문이 NULL일때 동적 할당 해제 에러 방지
        if (sw) {
            sqlite3_finalize(stmt);
            sqlite3_free(sql);
            printf("%d\n", sw);
        }
        else printf("%s 상태의 일정이 없습니다.\n", status);
    }
    else fprintf(stderr, "데이터 조회 오류: %s\n", sqlite3_errmsg(db));
}

int statusIndexToId(const char *status, int user_no) {
    int real_id = -1;

    /* if (DBO("DB/DBSC.db") != 0) {
        // printf("DB 연결이 되어 있지 않습니다.\n");
        return real_id;
    } */

    char *sql = sqlite3_mprintf("SELECT id FROM (SELECT ROW_NUMBER() OVER (ORDER BY scheduled_date_time) AS no, id FROM schedules WHERE status = '%s') WHERE no = %d;", status, user_no);
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);


    if (rc == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            real_id = sqlite3_column_int(stmt, 0);
        }
        else printf("%d 해당 번호의 일정이 존재하지 않습니다.\n", real_id);

        sqlite3_finalize(stmt);
        sqlite3_free(sql);
    }
    else printf("SQL 실행 실패 : %s\n", sqlite3_errmsg(db));

    return real_id;
}

Schedule *idToStatusView(int id) {
    sqlite3_stmt *stmt;
    Schedule *s = smalloc();
    // char *sql = sqlite3_mprintf("SELECT title, scheduled_date_time, end_date_time, tag, priority FROM schedules WHERE id = %d;", id);
    char *sql = sqlite3_mprintf("SELECT CASE WHEN priority = 1 THEN '! ' || title WHEN priority = 2 THEN '!! ' || title WHEN priority = 3 THEN '!!! ' || title ELSE title END AS priority_title, scheduled_date_time, end_date_time, tag FROM schedules WHERE id = %d;", id);
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char *title = sqlite3_column_text(stmt, 0);
            const unsigned char *sdt = sqlite3_column_text(stmt, 1);
            const unsigned char *edt = sqlite3_column_text(stmt, 2);
            const unsigned char *tag = sqlite3_column_text(stmt, 3);
        
            // NULL 체크 필요
            strcpy(s->title, (((char*)title) != NULL) ? (char*)title : "NULL");
            strcpy(s->scheduled_date_time, (((char*)sdt) != NULL) ? (char*)sdt : "NULL");
            strcpy(s->end_date_time, (((char*)edt) != NULL) ? (char*)edt : "NULL");
            strcpy(s->tag, (((char*)tag) != NULL) ? (char*)tag : "NULL");

            // printf("%s, %s, %s, %s, %d\n", s->title, s->scheduled_date_time, s->end_date_time, s->tag, s->priority);
        }
        // SQL문이 NULL일때 동적 할당 해제 에러 방지
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            sqlite3_finalize(stmt);
            sqlite3_free(sql);
        }
    }
    else fprintf(stderr, "데이터 조회 오류: %s\n", sqlite3_errmsg(db)); 

    return s;
}

void updateStatus(const char *status, int id) {
    char *sql = sqlite3_mprintf("UPDATE schedules SET status = '%s' WHERE id = %d;", status, id);
    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "데이터 업데이트 오류: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_free(sql);
    }
    
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

        // SQL문이 NULL일때 동적 할당 해제 에러 방지
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            sqlite3_finalize(stmt);
            sqlite3_free(sql);
        }
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

TagCount *indexToTagCount(int user_no) {
    sqlite3_stmt *stmt;
    TagCount *tc = tcmalloc();

    const unsigned char *tag = NULL;
    char *sql = sqlite3_mprintf("SELECT temp_tag.tag, (SELECT COUNT(*) FROM schedules WHERE tag = temp_tag.tag) AS count FROM (SELECT tag FROM (SELECT ROW_NUMBER() OVER (ORDER BY tag) AS no, tag FROM (SELECT DISTINCT tag FROM schedules WHERE tag IS NOT NULL)) WHERE no = %d) AS temp_tag;", user_no);
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    int count = 0;
    
    if (rc == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            tag = sqlite3_column_text(stmt, 0);
            count = sqlite3_column_int(stmt, 1);

            strcpy(tc->tag, (((char*)tag) != NULL) ? (char*)tag : "NULL");
            tc->count = count;
            // printf("-> %s %d\n", tc->tag, tc->count);
        }
        else printf("%d 해당 번호의 태그가 존재하지 않습니다.\n", count);

        sqlite3_finalize(stmt);
        sqlite3_free(sql);
    }
    else printf("SQL 실행 실패 : %s\n", sqlite3_errmsg(db));

    return tc;
}

void viewTagByschedule(char *tag) {
    sqlite3_stmt *stmt;
    // char *sql = sqlite3_mprintf("SELECT * FROM schedules WHERE tag = '%s';", tag);
    char *sql = sqlite3_mprintf("SELECT ROW_NUMBER() OVER (ORDER BY priority DESC, scheduled_date_time ASC, id ASC) AS no, CASE WHEN priority = 1 THEN '! ' || title WHEN priority = 2 THEN '!! ' || title WHEN priority = 3 THEN '!!! ' || title ELSE title END AS priority_title, scheduled_date_time, end_date_time, tag, status FROM schedules WHERE tag = '%s';", tag);
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            const unsigned char *title = sqlite3_column_text(stmt, 1);
            const unsigned char *sdt = sqlite3_column_text(stmt, 2);
            const unsigned char *edt = sqlite3_column_text(stmt, 3);
            const unsigned char *tag = sqlite3_column_text(stmt, 4);

            printf("%2d | %s | %s | %s | %s\n", id, title, sdt, edt, tag);
        }

        // SQL문이 NULL일때 동적 할당 해제 에러 방지
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            sqlite3_finalize(stmt);
            sqlite3_free(sql);
        }
    }

    else fprintf(stderr, "데이터 조회 오류: %s\n", sqlite3_errmsg(db));
}