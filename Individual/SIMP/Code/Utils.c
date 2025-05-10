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
    time_t start = (strcmp(s->scheduled_date_time, "NULL") != 0 || s->scheduled_date_time != NULL) ? parseTime(s->scheduled_date_time) : 0;
    time_t end = (strcmp(s->end_date_time, "NULL") != 0 || s->end_date_time != NULL) ? parseTime(s->end_date_time) : 0;
    
    // TODO → DOING
    if (strcmp(s->status, "TODO") == 0 && (now >= start || (end > 0 && now >= end))) {
        printf("id : %d, s: %s -> DOING\n", id, s->status);
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