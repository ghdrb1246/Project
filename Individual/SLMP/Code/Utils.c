#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Utils.h"
#include "UserStructure.h"

// 날짜/시간 비교 함수

int updateScheduleStatus(Schedule *s, int id) {
    int new_id = -1;
    time_t now = time(NULL);
    time_t start = (strcmp(s->scheduled_date_time, "NULL") != 0 && s->scheduled_date_time != NULL) ? parseTime(s->scheduled_date_time) : 0;
    time_t end = (strcmp(s->end_date_time, "NULL") != 0 && s->end_date_time != NULL) ? parseTime(s->end_date_time) : 0;
    time_t t = (start >= end) ? start : end;

    // TODO → DOING
    if (strcmp(s->status, "TODO") == 0 && (now >= t)) {
        new_id = id;
    }
    
    return new_id;
}

static time_t parseTime(const char *datetime_str) {
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));
    strptime(datetime_str, "%Y-%m-%d %H:%M", &tm);

    return mktime(&tm);
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int validDateTime(char *datetime) {
    int y, m, d, h, min;
    int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    
    // "YYYY-MM-DD HH:MM" → 총 16자 (문자열 + 널 포함 17바이트)
    if (strlen(datetime) != 16 || datetime[4] != '-' || datetime[7] != '-' || datetime[10] != ' ' || datetime[13] != ':') {
        return 0;
    }

    // 날짜와 시간 추출
    else if (sscanf(datetime, "%4d-%2d-%2d %2d:%2d", &y, &m, &d, &h, &min) != 5) {
        return 0;
    }

    // 날짜 범위 검사
    if ((y < 1900 || y > 2100) || (m < 1 || m > 12) || (d < 1 || d > 31)) return 0;

    // 월별 최대 일자 확인 (윤년 고려)
    if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0) {
        daysInMonth[1] = 29;  // 윤년
    }
    if (d > daysInMonth[m - 1]) return 0;

    // 시간 범위 검사
    if ((h < 0 || h > 23) || (min < 0 || min > 59)) return 0;


return 1; // 유효한 날짜+시간
}

int compareDateTimeSize(Schedule *s) {
    int run = -1;
    time_t start = (strcmp(s->scheduled_date_time, "NULL") != 0 || s->scheduled_date_time != NULL) ? parseTime(s->scheduled_date_time) : 0;
    time_t end = (strcmp(s->end_date_time, "NULL") != 0 || s->end_date_time != NULL) ? parseTime(s->end_date_time) : 0;
    
    if (end > 0) {
        if (start <= end) return 1;
        else if (start >= end && end > 0) return 2;
    }
    else return 1;

    return run;
}