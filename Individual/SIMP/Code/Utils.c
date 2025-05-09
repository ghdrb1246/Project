#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Utils.h"
#include "UserStructure.h"

int updateScheduleStatus(Schedule *s, int id) {
    // char new_status[5] = "DOING";
    int new_id = 0;
    time_t now = time(NULL);
    time_t start = (strcmp(s->scheduled_date_time, "NULL") != 0 || s->scheduled_date_time != NULL) ? parseTime(s->scheduled_date_time) : 0;
    time_t end = (strcmp(s->end_date_time, "NULL") != 0 || s->end_date_time != NULL) ? parseTime(s->end_date_time) : 0;
    
    // printf("%ld %ld %ld\n", now, start, end);
    // printf("안 : %s | %s | %s | %s | %d | %s\n", s->title, s->scheduled_date_time, s->end_date_time, s->tag, s->priority, s->status);
    
    // TODO → DOING
    if (strcmp(s->status, "TODO") == 0 && (now >= start || (end > 0 && now >= end))) {
        // strcpy(s->status, "DOING");
        // s->status = "DOING";
        // s->status = strdup("DOING");
        printf("id : %d, s: %s -> DOING\n", id, s->status);
        new_id = id;
    }
    return new_id;
}

static time_t parseTime(const char *datetime_str) {
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));
    strptime(datetime_str, "%Y-%m-%d %H:%M", &tm);

    // printf("%d-%d-%d %d:%d\n", tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min);
    return mktime(&tm);
}