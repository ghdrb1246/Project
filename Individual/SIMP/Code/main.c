#include "MumeIO.h"
#include "DB_Management.h"
/* #include <stdio.h>
#include "UserStructure.h"
#include "Utils.h" */

int main() {
   int test = DBO("DB/DBSC.db");
   if (test) return 1;

    main_meun();

    // checkScheduleStatus();

    // DBC();

   /*  Schedule *s = smalloc();
    if (!s) printf("smalloc Eorr");
    
    s->title = "test1";
    s->scheduled_date_time = "2025-05-07 22:40";
    s->end_date_time = "NULL";
    s->tag = "testtag1";
    s->priority = 3;
    s->status = "TODO";

    printf("전 : %s | %s | %s | %s | %d | %s\n", s->title, s->scheduled_date_time, s->end_date_time, s->tag, s->priority, s->status);
    
    updateScheduleStatus(s);
    
    printf("후 : %s | %s | %s | %s | %d | %s\n", s->title, s->scheduled_date_time, s->end_date_time, s->tag, s->priority, s->status);
 */
    return 0;
}