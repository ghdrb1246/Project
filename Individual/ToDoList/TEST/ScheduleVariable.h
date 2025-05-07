#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Schedule {
    char *title;
    char *scheduled_date_time;
    char *end_date_time;
    char *tag;
    int priority;
    char *status;
};

struct TagCount {
    char *tag;
    int count;
};

/**
 * @brief 일정 구조체
 * 
 * @param title 제목
 * @param scheduled_date_time 예정 날짜/시간
 * @param end_date_time 종료 날짜/시간
 * @param tag 일정 관련 태그
 * @param priority 우선순위 (0:없음, 1:낮음, 2:중간, 3:높음)
 * @param status 상태(TODO, DOING, DONE)
 * 
 */
typedef struct Schedule Schedule;

/**
 * @brief 태그와 태그의 건수 구조체
 * 
 * @param tag 태그
 * @param count 해당 태그의 건수
 */
typedef struct TagCount TagCount;

/**
 * @brief Schedule 구조체 메모리 할당
 * 
 * @return Schedule 
 */
Schedule smalloc();

/**
 * @brief 현재 시간과 일정 시간을 비교
 * 
 * @param s 일정
 */
Schedule updateScheduleStatus(Schedule s);

/**
 * @brief YYYY-MM-DD HH:MM 형식을 time_t 타입으로 변경
 * 
 * @param datetime_str 변경할 YYYY-MM-DD HH:MM 형식의 시간
 * @return time_t 
 */
static time_t parseTime(const char *datetime_str);