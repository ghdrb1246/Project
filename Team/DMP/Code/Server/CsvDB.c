#include "CsvDB.h"

int needConvert(const char *csv, const char *db) {
    struct stat csvStat, dbStat;

    if (stat(csv, &csvStat) != 0) {
        printf("[ 서버 ] | CSV 파일 확인 실패\n");
        return 0;
    }

    if (stat(db, &dbStat) != 0) {
        printf("[ 서버 ] | DB 파일이 없으므로 변환 필요\n");
        return 1;
    }

    if (csvStat.st_mtime > dbStat.st_mtime) {
        printf("[ 서버 ] | CSV가 최신입니다. 변환 필요\n");
        return 1;
    }

    printf("[ 서버 ] | DB가 최신 상태입니다.\n");
    
    return 0;
}

// CSV → DB 변환 (기존 DB 삭제 후 새로 생성)
void exerciseConvertCSVtoDB() {
    printf("[ 서버 ] | 기존 DB를 삭제하고 새로 생성합니다.\n");

    // DB 파일 삭제
    remove(EXERCISES_DB_FILE);
    // 새 DB 연결
    sqlite3 *db;
    if (sqlite3_open(EXERCISES_DB_FILE, &db)) {
        printf("[ 서버 ] | DB 열기 실패: %s\n", sqlite3_errmsg(db));
        return;
    }

    // 테이블 생성
    const char *createTableSQL = "CREATE TABLE exercise (name TEXT PRIMARY KEY, met REAL);";
    sqlite3_exec(db, createTableSQL, 0, 0, 0);

    // CSV 읽기 & INSERT
    FILE *fp = fopen(EXERCISES_CSV_FILE, "r");
    sqlite3_stmt *stmt;
    
    if (!fp) {
        printf("[ 서버 ] | CSV 열기 실패\n");
        sqlite3_close(db);
        return;
    }
    
    char line[128];
    fgets(line, sizeof(line), fp);  // 헤더 skip
    
    sqlite3_prepare_v2(db, "INSERT INTO exercise (name, met) VALUES (?, ?);", -1, &stmt, NULL);

    while (fgets(line, sizeof(line), fp)) {
        char *name = strtok(line, ",\n");
        char *metStr = strtok(NULL, ",\n");
        float met = atof(metStr);

        sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 2, met);

        sqlite3_step(stmt);
        sqlite3_reset(stmt);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    fclose(fp);

    printf("[ 서버 ] | CSV → DB 최신화 완료\n");
}

float inputWorkoutAndCalc(char *exercise) {
    sqlite3 *db;
    if (sqlite3_open(EXERCISES_DB_FILE, &db)) {
        printf("[ 서버 ] | DB 열기 실패: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    
    sqlite3_stmt *stmt;
    float met = -1;

    // DB에서 운동 찾기
    const char *sql = "SELECT met FROM exercise WHERE name = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("[ 서버 ] | SQL 준비 실패: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    sqlite3_bind_text(stmt, 1, exercise, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        met = (float)sqlite3_column_double(stmt, 0);
        sqlite3_finalize(stmt);
    } 

    else {
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(db);
    
    return met;
}

float METM(float met, float minutes, float user_weight) {
    float kcal;
    float hours = minutes / 60.0f;

    // MET 계산법: kcal = MET × 체중(kg) × 시간(h)
    kcal = met * user_weight * hours;

    return kcal;
}

// CSV → DB 변환 (기존 DB 삭제 후 새로 생성)
void foodConvertCSVtoDB() {
    printf("[ 서버 ] | 기존 DB를 삭제하고 새로 생성합니다.\n");

    // DB 파일 삭제
    remove(FOOD_DB_FILE);
    // 새 DB 연결
    sqlite3 *db;
    if (sqlite3_open(FOOD_DB_FILE, &db)) {
        printf("[ 서버 ] | DB 열기 실패: %s\n", sqlite3_errmsg(db));
        return;
    }

    // 테이블 생성
    const char *createTableSQL = "CREATE TABLE IF NOT EXISTS foods (name TEXT PRIMARY KEY, kcal REAL);";
    sqlite3_exec(db, createTableSQL, 0, 0, 0);

    // CSV 읽기 & INSERT
    FILE *fp = fopen(FOOD_CSV_FILE, "r");
    sqlite3_stmt *stmt;
    
    if (!fp) {
        printf("[ 서버 ] | CSV 열기 실패\n");
        sqlite3_close(db);
        return;
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

    printf("[ 서버 ] | CSV → DB 최신화 완료\n");
}

float inputFoodAndCalc(char *food) {
    sqlite3 *db;
    if (sqlite3_open(FOOD_DB_FILE, &db)) {
        printf("[ 서버 ] | DB 열기 실패: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    
    sqlite3_stmt *stmt;
    float kcal = -1;

    // DB에서 음식 찾기
    const char *sql = "SELECT kcal FROM foods WHERE name = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("[ 서버 ] | SQL 준비 실패: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    sqlite3_bind_text(stmt, 1, food, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        kcal = (float)sqlite3_column_double(stmt, 0);
        sqlite3_finalize(stmt);
    } 

    else {
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(db);
    
    return kcal;
}