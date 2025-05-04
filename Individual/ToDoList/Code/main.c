#include <stdio.h>
#include "MumeIO.h"
#include "DB_Management.h"

/* typedef struct {
    char *title;                // 제목
    char *scheduled_date_time;  // "2025-04-16 09:00"
    char *end_date_time;        // "2025-04-23 23:59" 선택 사항
    char *tag;                  // 일정 관련 태그(수업, 버스 시간, 시험.. 등)
    int priority;               // 0(없음), 1(낮음), 2(중간), 3(높음)
    char *status;               // TODO, DOING, DONE
} Schedule;
 */

int main() {
    int test = 1;

    while (test) {
        test = DBO("DBSC.db");
    }
        
    main_meun();
    
    // viewAllByStatus("TODO");
    // viewDB();
    DBC();

    return 0;
}
