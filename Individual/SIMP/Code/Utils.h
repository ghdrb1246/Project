#ifndef _UTILS_
#define _UTILS_

#include <time.h>
#include "UserStructure.h"

/**
 * @brief 현재 시간과 일정 시간을 비교
 * 
 * @param s 일정 구조체
 * @param id 확인할 일정 DB id
 * @return int 상태가 변경할 DB id 반환
 */
int updateScheduleStatus(Schedule *s, int id);

/**
 * @brief YYYY-MM-DD HH:MM 형식을 time_t 타입으로 변경
 * 
 * @param datetime_str 변경할 YYYY-MM-DD HH:MM 형식의 시간
 * @return time_t 
 */
static time_t parseTime(const char *datetime_str);

#endif