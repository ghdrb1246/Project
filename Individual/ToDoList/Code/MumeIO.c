#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "MumeIO.h"
#include "DB_Management.h"
#include "UserStructure.h"

// 메뉴 입출력 모듈

char *estatus[3] = { "TODO", "DOING", "DONE" }; // 이동

void main_meun() {
    int meunNumber = 0, sw = 1;

    while (sw) {
        // 상태 변경 확인
        // checkScheduleStatus();

        printf("================= [ 일정 관리 프로그램 ] =================\n");
        printf("1. 일정 등록\n");
        printf("2. 일정 조회\n");
        printf("3. 종료하기\n");
        printf("---------------------------------------------------------\n");
        printf("메뉴 선택 : ");
        scanf("%d", &meunNumber);
        printf("=========================================================\n");
    
        switch (meunNumber) {
            case 1:
                scheduleRegistrationMenu();
                sw = 0;
            break;
    
            case 2: 
                calendarViewMenu();
                sw = 0;
            break;
    
            case 3:
                DBC();
                exit(0);
            break;
    
            default : 
                printf("Input Error\n");
            break;
        }
    }
}

void scheduleRegistrationMenu() {
    char str[2] = "", sitme[6] = "", eitme[6] = "";
    int sw = 1;
    Schedule *s = smalloc();

    while (sw) {
        printf("===================== [ 일정 등록 ] =====================\n");
        
        printf("제목 : "); 
        scanf("%s", s->title);
        
        printf("예정 날짜(YYYY-MM-DD) : "); 
        scanf("%s", s->scheduled_date_time);
        
        printf("-> 예정 시간(HH:MM) : ");
        scanf("%s", sitme);
        strcat(s->scheduled_date_time, " ");
        strcat(s->scheduled_date_time, sitme);
        
        while (1) {
            int in_x = -1;
            printf("종료 날짜(YYYY-MM-DD or x) : ");
            scanf("%s", s->end_date_time);
            
            in_x = (strcmp(s->end_date_time, "x") == 0 || strcmp(s->end_date_time, "X") == 0) ? 1 : 0;
            
            if (in_x == 1) {
                strcpy(s->end_date_time, "NULL");
                break;
            }

            else if (in_x == 0) {
                printf("-> 종료 시간(HH:MM) : ");
                scanf("%s", eitme);
                strcat(s->end_date_time, " ");
                strcat(s->end_date_time, eitme);
                break;
            }

            else printf("Input Error\n");
        }

        printf("태그 : "); 
        scanf("%s", s->tag);

        while (1) {
            printf("우선순위(0:없음, 1:낮음, 2:중간, 3:높음) : "); 
            scanf("%d", &s->priority);

            if (s->priority >= 0 && s->priority <= 3) break;
            else printf("Input Error : %d\n", s->priority);
        }

        printf("---------------------------------------------------------\n");

        while (1) {
            printf("저장[Y/N] : ");
            scanf("%s", str);

            printf("---------------------------------------------------------\n");

            if (strcmp(str, "Y") == 0 || strcmp(str, "y") == 0) {
                saveDB(s);
                printf("\n일정을 저장되었습니다.\n\n");
                sw = 0;
                break;
            }
            else if (strcmp(str, "N") == 0 || strcmp(str, "n") == 0) { 
                printf("\n취소.\n\n");
                sw = 0;
                break;
            }
            else printf("Input Error : %s\n", str);
        }
    }

    // printf("%s %s %s %s %d\n", s->title, s->scheduled_date_time, s->end_date_time, s->tag, s->priority);
    
    printf("=========================================================\n");
    
    sfree(s);
    main_meun();
}

void calendarViewMenu() {
    int meunNumber = 0, status = 0;
    
    printf("===================== [ 일정 조회 ] =====================\n");
    printf("1. 상태별 조회\n");
    printf("2. 태그 조회\n");
    printf("3. 뒤로가기\n");
    printf("---------------------------------------------------------\n");
    printf("메뉴 선택 : ");
    scanf("%d", &meunNumber);

    switch (meunNumber) {
        case 1: 
            printf("조회할 상태(1:%s / 2:%s / 3:%s) : ", estatus[0], estatus[1], estatus[2]);
            scanf("%d", &status);
            scheduleViewMenuByStatus(status);
        break;

        case 2: 
            tagViewMenu();
        break;

        case 3:
            printf("\n뒤로가기.\n");
            main_meun();
        break;

        default : 
            printf("Input Error\n");
        break;
    }

    printf("=========================================================\n");
}

void scheduleViewMenuByStatus(int statusNumer) {
    // 상태 변경 확인
    // checkScheduleStatus();

    printf("================= [ 일정 상태별 조회 ] ================\n");
    printf("[ %s -------------------------------------------- ]\n", estatus[statusNumer - 1]);

    viewAllByStatus(estatus[statusNumer - 1]);

    printf("--------------------------------------------------------\n");

    switch(statusNumer) {
        case 1: _TODO(); break;
        case 2: _DOING(); break;
        case 3: _DONE(); break;
        default : printf("Return Error : %d\n", statusNumer); break;
    }

    printf("========================================================\n");
}

static void _TODO() {
    int meunNumber = 0, sw = 1, user_no = 0, id = 0;
    
    while (sw) {
        printf("[ 선택 ----------------------------------------------- ]\n");
        printf("1. 일정 수정\n");
        printf("2. 일정 삭제\n");
        printf("3. 뒤로가기\n");
        printf("--------------------------------------------------------\n");
        printf("메뉴 선택 : ");
        scanf("%d", &meunNumber);
        
        switch (meunNumber) {
            case 1:
                printf("수정할 일정 번호 선택 : ");
                scanf("%d", &user_no);

                id = statusIndexToId(estatus[0], user_no);

                if (id != -1) scheduleModificationMenu(id);
                else printf("db Error");
                sw = 0;
            break;
    
            case 2: 
                printf("삭제할 일정 선택 : ");
                scanf("%d", &user_no);

                id = statusIndexToId(estatus[0], user_no);
                
                if (id != -1) deleteScheduleMenu(id);
                else printf("db Error");
                sw = 0;
            break;
    
            case 3:
                calendarViewMenu();
                sw = 0;
            break;
    
            default : 
                printf("Input Error\n");
            break;
        }
    }
}

static void _DOING() {
    int meunNumber = 0, sw = 1, user_no = 0, id = 0;
    
    while (sw) {
        printf("[ 선택 ----------------------------------------------- ]\n");
        printf("1. 일정 완료\n");
        printf("2. 일정 연기\n");
        printf("3. 일정 삭제\n");
        printf("4. 뒤로가기\n");
        printf("--------------------------------------------------------\n");
        printf("메뉴 선택 : ");
        scanf("%d", &meunNumber);
    
        switch (meunNumber) {
            case 1:
                printf("완료할 일정 선택 : ");
                scanf("%d", &user_no);

                id = statusIndexToId(estatus[1], user_no);
                
                if (id != -1) scheduleCompleteMenu(id);
                else printf("db Error");
                sw = 0;
            break;
    
            case 2:
                printf("연기할 일정 선택 : ");
                scanf("%d", &user_no);

                id = statusIndexToId(estatus[1], user_no);
                
                if (id != -1) schedulePostponedMenu(id);
                else printf("db Error");
                sw = 0;
            break;
            
            case 3:
                printf("식제할 일정 선택 : ");
                scanf("%d", &user_no);

                id = statusIndexToId(estatus[1], user_no);
                
                if (id != -1) deleteScheduleMenu(id);
                else printf("db Error");
                sw = 0;
            break;

            case 4:
                calendarViewMenu();
                sw = 0;
            break;
    
            default : 
                printf("Input Error\n");
            break;
        }
    }
}

static void _DONE() {
    int meunNumber = 0, sw = 1, user_no = 0, id = 0;
    
    while (sw) {
        printf("[ 선택 ----------------------------------------------- ]\n");
        printf("1. 일정 삭제\n");
        printf("2. 뒤로가기\n");
        printf("--------------------------------------------------------\n");
        printf("메뉴 선택 : ");
        scanf("%d", &meunNumber);
    
        switch (meunNumber) {
            case 1:
                printf("삭제할 일정 선택 : ");
                scanf("%d", &user_no);

                id = statusIndexToId(estatus[2], user_no);
                
                if (id != -1) deleteScheduleMenu(id);
                else printf("db Error");
                sw = 0;
            break;
    
            case 2:
                calendarViewMenu();
                sw = 0;
            break;
    
            default : 
                printf("Input Error\n");
            break;
        }
    }
}

void scheduleModificationMenu(int id) {
    Schedule *new_s = smalloc(), *old_s = idToStatusView(id);
    char str[2] = "", sitme[6] = "", eitme[6] = "";
    int meunNumber = 0, sw = 1;

    printf("===================== [ 일정 수정 ] =====================\n");
    printf("수정 대상 : %s, %s, %s, %s, %d\n", old_s->title, old_s->scheduled_date_time, old_s->end_date_time, old_s->tag, old_s->priority);
    printf("[ ----------------------------------------------------- ]\n");
    
    while (sw) {
        printf("1.제목, 2.예정 날짜/시간, 3.종료 날짜/시간 4.태그, 5.우선순위, 6.저장\n");
        printf("---------------------------------------------------------\n");
        printf("항목 선택 : ");
        scanf("%d", &meunNumber);

        switch (meunNumber) {
            case 1:
                printf("제목 : "); 
                scanf("%s", new_s->title);
    
                strcpy(old_s->title, new_s->title);
            break;
    
            case 2:
                printf("예정 날짜(YYYY-MM-DD) : "); 
                scanf("%s", new_s->scheduled_date_time);
    
                printf("-> 예정 시간(HH:MM) : ");
                scanf("%s", sitme);
                strcat(new_s->scheduled_date_time, " ");
                strcat(new_s->scheduled_date_time, sitme);
    
                strcpy(old_s->scheduled_date_time, new_s->scheduled_date_time);
            break;
    
            case 3:
                printf("종료 날짜(YYYY-MM-DD or x) : ");
                scanf("%s", new_s->end_date_time);
                printf("-> 종료 시간(HH:MM) : ");
                scanf("%s", eitme);
                strcat(new_s->end_date_time, " ");
                strcat(new_s->end_date_time, eitme);
    
                strcpy(old_s->end_date_time, new_s->end_date_time);
            break;
    
            case 4:
                printf("태그 : "); 
                scanf("%s", new_s->tag);
    
                strcpy(old_s->tag, new_s->tag);
            break;
    
            case 5:
                while (1) {
                    printf("우선순위(0:없음, 1:낮음, 2:중간, 3:높음) : "); 
                    scanf("%d", &new_s->priority);
            
                    if (new_s->priority >= 0 && new_s->priority <= 3) break;
                    else printf("Input Error : %d\n", new_s->priority);
                }
    
                old_s->priority = new_s->priority;
            break;
    
            case 6:
                while (1) {
                    printf("---------------------------------------------------------\n");
    
                    printf("저장[Y/N] : ");
                    scanf("%s", str);

                    printf("---------------------------------------------------------\n");
            
                    if (strcmp(str, "Y") == 0 || strcmp(str, "y") == 0) {
                        updateDB(old_s, id);
                        // printf("%s %s %s %s %d\n", old_s->title, old_s->scheduled_date_time, old_s->end_date_time, old_s->tag, old_s->priority);
                        printf("\n일정을 저장되었습니다.\n\n"); 
                        sw = 0;                       
                        break;
                    }

                    else if (strcmp(str, "N") == 0 || strcmp(str, "n") == 0) { 
                        printf("\n취소.\n\n");
                        sw = 0;
                        break;
                    }

                    else printf("%s : Input Error\n", str);
                }
            break;
    
            default :
                printf("%d : Input Error\n", meunNumber);
            break;
        }
    }
    printf("=========================================================\n");
    
    sfree(new_s);
    sfree(old_s);
    calendarViewMenu();
}

void deleteScheduleMenu(int id) {
    Schedule *s = idToStatusView(id);
    char str[2] = "";

    printf("===================== [ 일정 삭제 ] =====================\n");
    printf("삭제 대상 : %s, %s, %s, %s, %d\n", s->title, s->scheduled_date_time, s->end_date_time, s->tag, s->priority);
    printf("[ ----------------------------------------------------- ]\n");
    while (1) {
        printf("선택한 일정을 삭제 하겠습니까?[Y/N] : ");
        scanf("%s", str);
    
        if (strcmp(str, "Y") == 0 || strcmp(str, "y") == 0) {
            deleteDB(id);
            printf("\n일정을 삭제되었습니다.\n\n");
            break;
        }
        else if (strcmp(str, "N") == 0 || strcmp(str, "n") == 0) { 
            printf("\n취소.\n\n");
            break;
        }
        else printf("Input Error\n");
    }
    printf("=========================================================\n");

    sfree(s);
    calendarViewMenu();
}

void scheduleCompleteMenu(int id) {
    Schedule *s = idToStatusView(id);
    char str[2] = "";
    
    // 상태 변경 확인
    // checkScheduleStatus();
    
    printf("===================== [ 일정 완료 ] =====================\n");
    printf("완료 대상 : %s, %s, %s, %s, %d\n", s->title, s->scheduled_date_time, s->end_date_time, s->tag, s->priority);
    printf("[ ----------------------------------------------------- ]\n");
    while (1) {
        printf("선택한 일정을 완료 하셨습니까?[Y/N] : ");
        scanf("%s", str);
    
        if (strcmp(str, "Y") == 0 || strcmp(str, "y") == 0) {
            updateStatus("DONE", id);
            printf("\n완료 처리 되었습니다.\n\n");
            break;
        }
        else if (strcmp(str, "N") == 0 || strcmp(str, "n") == 0) { 
            printf("\n취소.\n\n");
            break;
        }
        else printf("Input Error\n");
    }
    printf("=========================================================\n");

    sfree(s);
    calendarViewMenu();
}

void schedulePostponedMenu(int id) {
    char str[2] = "", sitme[6] = "", eitme[6] = "";
    int meunNumber = 0, sw = 1;
    Schedule *new_s = smalloc(), *old_s = idToStatusView(id);

    printf("===================== [ 일정 연기 ] =====================\n");
    printf("연기 대상 : %s, %s, %s, %s, %d\n", old_s->title, old_s->scheduled_date_time, old_s->end_date_time, old_s->tag, old_s->priority);
    printf("[ ----------------------------------------------------- ]\n");
    while (sw) {
        printf("연기 항목 선택(1.예정 날짜/시간, 2.종료 날짜/시간, 3.저장) : ");
        scanf("%d", &meunNumber);

        switch (meunNumber) {
            case 1:
                printf("예정 날짜/시간(YYYY-MM-DD HH:MM) : \n");  
                printf("%s -> ", old_s->scheduled_date_time); 
                scanf("%s %s", new_s->scheduled_date_time, sitme);
                strcat(new_s->scheduled_date_time, " ");
                strcat(new_s->scheduled_date_time, sitme);
                strcpy(old_s->scheduled_date_time, new_s->scheduled_date_time);
            break;
            
            case 2:
                printf("종료 날짜/시간(YYYY-MM-DD HH:MM) : \n"); 
                printf("%s -> ", old_s->end_date_time); 
                scanf("%s %s", new_s->end_date_time, eitme);
                strcat(new_s->end_date_time, " ");
                strcat(new_s->end_date_time, eitme);
                strcpy(old_s->end_date_time, new_s->end_date_time);
            break;

            case 3:
                while (1) {
                    printf("[ ----------------------------------------------------- ]\n");
                    printf("선택한 일정을 연기 하시겠습니까?[Y/N] : ");
                    scanf("%s", str);
                
                    if (strcmp(str, "Y") == 0 || strcmp(str, "y") == 0) {
                        updateDB(old_s, id);
                        printf("\n일정을 연기 되었습니다.\n\n");
                        sw = 0;
                        break;
                    }
                    else if (strcmp(str, "N") == 0 || strcmp(str, "n") == 0) { 
                        printf("\n취소.\n\n");
                        sw = 0;
                        break;
                    }
                    else printf("Input Error\n");
                }
            break;
            
            default :
                printf("Input Error\n");
            break;
        }
    }
    
    printf("=========================================================\n");

    sfree(new_s);
    sfree(old_s);
    calendarViewMenu();
}

void tagViewMenu() {
    int meunNumber = 0, sw = 1, user_no = 0;

    printf("================= [ 일정 조회 : 태그 ] ================\n");
    viewAllByTag();
    printf("[ 선택 ----------------------------------------------- ]\n");
    printf("1. 태크별 일정 조회\n");
    printf("2. 뒤로가기\n");
    printf("--------------------------------------------------------\n");
    
    while (sw) {
        printf("메뉴 선택 : ");
        scanf("%d", &meunNumber);

        switch (meunNumber) {
            case 1:
                printf("확인할 태그 번호 선택 : ");
                scanf("%d", &user_no);
                // scheduleMenuByTag(tagIndexToId(user_no));
                scheduleMenuByTag(user_no);
                sw = 0;
            break;

            case 2:
                calendarViewMenu();
                sw = 0;
            break;

            default :
                printf("Input Error\n");
            break;
        }
    }

    printf("========================================================\n");
}

/* void scheduleMenuByTag(char *tag) {
    char str[2] = "";

    printf("==================== [ 태그별 일정 ] ====================\n");
    printf("[ %s | %d -------------------------------------------- ]\n", tag, tagCount(tag));
    
    viewTagByschedule(tag);
    
    while (1) {
        printf("[ ----------------------------------------------------- ]\n");
        printf("뒤로가기[Y/N] : ");
        scanf("%s", str);
    
        if (strcmp(str, "Y") == 0 || strcmp(str, "y") == 0) {
            printf("\n뒤로 갑니다.\n\n");
            tagViewMenu();
            break;
        }
        else if (strcmp(str, "N") == 0 || strcmp(str, "n") == 0) { 
            printf("\n취소.\n\n");
            break;
        }
        else printf("Input Error\n");
    }
    printf("=========================================================\n");
} */

void scheduleMenuByTag(int user_no) {
    char str[2] = "";
    TagCount *tc = indexToTagCount(user_no);

    printf("==================== [ 태그별 일정 ] ====================\n");
    printf("[ %s | %d -------------------------------------------- ]\n", tc->tag, tc->count);
    
    viewTagByschedule(tc->tag);
    
    while (1) {
        printf("[ ----------------------------------------------------- ]\n");
        printf("뒤로가기[Y/N] : ");
        scanf("%s", str);
    
        if (strcmp(str, "Y") == 0 || strcmp(str, "y") == 0) {
            printf("\n뒤로 갑니다.\n\n");
            tagViewMenu();
            break;
        }
        else if (strcmp(str, "N") == 0 || strcmp(str, "n") == 0) { 
            printf("\n취소.\n\n");
            break;
        }
        else printf("Input Error\n");
    }
    printf("=========================================================\n");

    tcfree(tc);
}
