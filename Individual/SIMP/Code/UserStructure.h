#ifndef _USERSTRUCTURE_
#define _USERSTRUCTURE_

enum MenuState{
   MM, SRM, CVM, SVBSM, SMM, DSM, SCM, SPM, TVM, SBTM, EXIT
};

/**
 * @brief runMenu
 * 
 * @param MM main_meun
 * @param SRM scheduleRegistrationMenu
 * @param CVM calendarViewMenu
 * @param SVBSM scheduleViewByStatusMenu
 * @param SMM scheduleModificationMenu
 * @param DSM deleteScheduleMenu
 * @param SCM scheduleCompleteMenu
 * @param SPM schedulePostponedMenu
 * @param TVM tagViewMenu
 * @param SBTM scheduleByTagMenu
 */
typedef enum MenuState MenuState;

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

// 메모리 할당 및 해제 함수
/**
 * @brief Schedule 구조체 메모리 할당
 * 
 * @return Schedule *
 */
Schedule *smalloc();

/**
 * @brief Schedule 구조체 메모리 해제
 * 
 * @param s 동적 해제할 Schedule 구조체
 */
void sfree(Schedule *s);

/**
 * @brief TagCount 구조체 메모리 할당
 * 
 * @return TagCount * 
 */
TagCount *tcmalloc();

/**
 * @brief TagCount 구조체 메모리 해제
 * 
 * @param tc 동적 해제할 TagCount 구조체
 */
void tcfree(TagCount *tc);

#endif