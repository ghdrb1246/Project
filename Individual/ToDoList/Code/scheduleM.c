#include "scheduleM.h"

Schedule smalloc() {
    Schedule s;
    s.title = malloc(100 * sizeof(char));
    s.scheduled_date_time = malloc(20 * sizeof(char));
    s.end_date_time = malloc(20 * sizeof(char));
    s.tag = malloc(10 * sizeof(char));
    s.status = malloc(5 * sizeof(char));

    return s;
}

void updateScheduleStatus(Schedule s) {
    time_t now = time(NULL);
    time_t scheduled = parseTime(s.scheduled_date_time);
    time_t end = parseTime(s.end_date_time);

    if (strcmp(s.status, "TODO") == 0 && now >= scheduled) {
        strcpy(s.status, "DOING");
    }
    else if (strlen(s.end_date_time) > 0 && strcmp(s.status, "DOING") == 0 && now >= end) {
        strcpy(s.status, "DONE");
    }
}

static time_t parseTime(const char *datetime_str) {
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));
    strptime(datetime_str, "%Y-%m-%d %H:%M", &tm);
    return mktime(&tm);
}