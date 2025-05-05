#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "MumeIO.h"
#include "DB_Management.h"

int statusIndexToId(char *status, int user_no);

int main() {
    int test = 1;

    Schedule s = smalloc();

    while (test) {
        test = DBO("DBSC.db");
    }

    // printf("%d\n", statusIndexToId("TODO", 1));
    
    main_meun();
    // printf("%d\n", statusIndexToId("TODO", 1));
    
    // viewAllByStatus("DONE");
    // viewDB();
    // scheduleModificationMenu(1);

    // s = idToStatusView(1);

    // printf("%s %s %s %s %d\n", s.title, s.scheduled_date_time, s.end_date_time, s.tag, s.priority);

    DBC();

    return 0;
}
