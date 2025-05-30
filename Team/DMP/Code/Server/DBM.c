#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h> // mkdir

#include "sqlite/sqlite3.h"
#include "DBM.h"

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

    // 테이블이 존재하지 않으면 0, 존재하면 1를 반환
    //  if (tableExists("users") != 1) usersTableDB();

    usersTableDB();

    return 0; // 성공
}

void DBC() {
    sqlite3_close(db);
}

int tableExists(const char *tableName) {
    sqlite3_stmt *stmt;
    char *sql = sqlite3_mprintf("SELECT COUNT(*) FROM sqlite_master WHERE type = 'table' AND name = '%s';", tableName);
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    int real_id = -1;
    
    if (rc == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            real_id = sqlite3_column_int(stmt, 0);
            
            sqlite3_finalize(stmt);
        }
        else fprintf(stderr, "'%s' 해당 파일에 테이블이 존재하지 않습니다\n", sqlite3_errmsg(db));

    }
    else printf("SQL 실행 실패 : %s\n", sqlite3_errmsg(db));

    sqlite3_free(sql);  // SQL 문자열 메모리 해제

    return real_id;
}

void usersTableDB() {
    // CREATE TABLE IF NOT EXISTS [TABLE]
    // 테이블이 없다면 테이블을 추가 -> 따라서 tableExists()로 해당 테이블 확인할 필요 없다
    char *sql = "CREATE TABLE IF NOT EXISTS users(userId TEXT PRIMARY KEY, userPw TEXT NOT NULL);";
    char *err_msg = 0;

    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) { 
        fprintf(stderr, "테이블 생성 오류: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

int userExists(const char *userId) {
    sqlite3_stmt *stmt;
    char *sql = sqlite3_mprintf("SELECT * FROM users WHERE userId = '%s';", userId);

    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, userId, -1, SQLITE_STATIC);
    
    int result = (sqlite3_step(stmt) == SQLITE_ROW);
    
    sqlite3_finalize(stmt);
    sqlite3_free(sql);
    return result;
}

int signupUser(const char *userId, const char *userPw) {
    sqlite3_stmt *stmt;
    char *sql = sqlite3_mprintf("INSERT INTO users (userId, userPw) VALUES ('%s', '%s');", userId, userPw);
    
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, userId, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, userPw, -1, SQLITE_STATIC);
    
    int result = sqlite3_step(stmt);
    
    sqlite3_finalize(stmt);
    sqlite3_free(sql);
    return result == SQLITE_DONE;
}

int loginUser(const char *userId, const char *userPw) {
    sqlite3_stmt *stmt;
    char *sql = sqlite3_mprintf("SELECT * FROM users WHERE userId = '%s' AND userPw = '%s';", userId, userPw);
    
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, userId, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, userPw, -1, SQLITE_STATIC);
    
    int result = (sqlite3_step(stmt) == SQLITE_ROW);
    
    sqlite3_finalize(stmt);
    sqlite3_free(sql);
    return result;
}