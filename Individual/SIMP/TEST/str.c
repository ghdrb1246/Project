#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdlib.h>

typedef struct {
    char *title;
    char *scheduled_date_time;  // "2025-04-16 09:00"
    char *end_date_time;        // "2025-04-23 23:59" 선택 사항
    char *tag;                  // 일정 관련 태그(수업, 버스 시간, 시험.. 등)
    int priority;                 // 0(없음), 1(낮음), 2(중간), 3(높음)
    char *status;               // TODO, DOING, DONE
} Schedule ;

char *VALUES(Schedule);
char *itoa(int);

int main() {
    Schedule s;
    s.title = "과제";
    s.scheduled_date_time = "2025-04-28 09:00";
    s.end_date_time = NULL;
    s.tag = "수업";
    s.priority = 3;
    
    printf("%s\n", s.title);
    printf("%s\n", s.scheduled_date_time);
    printf("%s\n", s.end_date_time);
    printf("%s\n", s.tag);
    printf("%d\n", s.priority);

    printf("%s\n", VALUES(s));
    

    return 0;
}
// INSERT INTO schedules (title, scheduled_date_time, end_date_time, tag, priority)
char *VALUES(Schedule s) {
    char *sql = (char*)malloc(200 * sizeof(char));
    
    strcat(sql, "VALUES (");
    strcat(sql, s.title);
    strcat(sql, ", ");
    strcat(sql, s.scheduled_date_time);
    strcat(sql, ", ");
    strcat(sql, (s.end_date_time != NULL) ? s.end_date_time : "NULL");
    strcat(sql, ", ");
    strcat(sql, s.tag);
    strcat(sql, ", ");
    strcat(sql, itoa(s.priority));
    strcat(sql, ");");

    sql = realloc(sql, strlen(sql) * sizeof(char));
    return sql;
}

char *itoa(int inNum) {
    char *outStr = (char*)malloc(11 * sizeof(char));
    int tmpe = inNum, count = 0;

    while(tmpe != 0) {
        tmpe /= 10;
        count++;
    }
    outStr[count] = '\0';

    do { 
        count--;              
        outStr[count] = (char)(inNum % 10 + 48);
        inNum /= 10;
            
    } while(inNum != 0);
    // printf("%d %lu, %s\n", count, strlen(outStr), outStr);
    outStr = realloc(outStr, count * sizeof(char));

    return outStr;
}