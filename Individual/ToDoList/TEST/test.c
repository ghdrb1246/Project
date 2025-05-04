/* // 사용자가 선택한 no (보기 번호)
int user_no;
printf("수정할 번호를 입력하세요: ");
scanf("%d", &user_no);

// 쿼리 실행: 보기 번호 → 실제 id 검색
char sql[256];
sqlite3_stmt *stmt;
sprintf(sql,
    "SELECT id FROM (SELECT ROW_NUMBER() OVER (ORDER BY scheduled_time) AS no, id FROM tasks WHERE status != 'DONE') WHERE no = %d;",
    user_no
);

if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int real_id = sqlite3_column_int(stmt, 0);
        // ✅ 이제 real_id로 수정/삭제/완료 처리 가능
        printf("실제 DB ID: %d\n", real_id);
    } else {
        printf("잘못된 번호입니다.\n");
    }
    sqlite3_finalize(stmt);
}
 */

#include <stdio.h>

typedef struct {
    char *title;                // 제목
    char *scheduled_date_time;  // "2025-04-16 09:00"
    char *end_date_time;        // "2025-04-23 23:59" 선택 사항
    char *tag;                  // 일정 관련 태그(수업, 버스 시간, 시험.. 등)
    int priority;               // 0(없음), 1(낮음), 2(중간), 3(높음)
    char *status;               // TODO, DOING, DONE
} Schedule;

int main() {
    Schedule s, s1;

    s.title = "제목";
    s.scheduled_date_time = "2025-04-22 09:00";
    s.end_date_time = "NULL";
    s.tag = "구조체";
    s.priority = 3;
    s.status = "TODO";

    printf("-> %s %s %s %s %d %s\n", s.title, s.scheduled_date_time, s.end_date_time, s.tag, s.priority, s.status);

    s.title = "";
    printf("-> ");
    scanf("%s", s1.title);
    s.title = s1.title;
    printf("->> %s %s %s %s %d %s\n", s.title, s.scheduled_date_time, s.end_date_time, s.tag, s.priority, s.status);

    return 0;
}