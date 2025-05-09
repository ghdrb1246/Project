#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "MumeIO.h"
#include "DB_Management.h"
#include "UserStructure.h"

const char *CSTATUS[3] = { "TODO", "DOING", "DONE" };

// 메뉴 입출력 모듈

void runMenu() {
    if (DBO("DB/DBSC.db")) return;

    MenuState current = MM;
    while (current != EXIT) {
        switch (current) {
            case MM:    current = main_meun();                 break;
            case SRM:   current = scheduleRegistrationMenu();  break;
            case CVM:   current = calendarViewMenu();          break;
            case SVBSM: current = scheduleViewByStatusMenu();  break;
            case SMM:   current = scheduleModificationMenu();  break;
            case DSM:   current = deleteScheduleMenu();        break;
            case SCM:   current = scheduleCompleteMenu();      break;
            case SPM:   current = schedulePostponedMenu();     break;
            case TVM:   current = tagViewMenu();               break;
            case SBTM:  current = scheduleByTagMenu();         break;
            default:    current = EXIT;                        break;
        }
    }

    DBC();
}

MenuState main_meun() {
    int meunNumber = 0;
   
    // 상태 변경 확인
    // checkScheduleStatus();
    
    while(1) {
        printf("================= [ 일정 관리 프로그램 ] =================\n");
        printf("1. 일정 등록\n");
        printf("2. 일정 조회\n");
        printf("3. 종료하기\n");
        printf("---------------------------------------------------------\n");
        printf("메뉴 선택 : ");
        scanf("%d", &meunNumber);
        printf("=========================================================\n");

        switch (meunNumber) {
            case 1: return SRM;

            case 2: return CVM;

            case 3: return EXIT;

            default : 
                printf("Input Error\n");
            break;
        }
    }
}

MenuState scheduleRegistrationMenu() {
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
    return MM;
}

MenuState calendarViewMenu() {
    int meunNumber = 0;
    
    while (1) {
        printf("===================== [ 일정 조회 ] =====================\n");
        printf("1. 상태별 조회\n");
        printf("2. 태그 조회\n");
        printf("3. 뒤로가기\n");
        printf("---------------------------------------------------------\n");
        printf("메뉴 선택 : ");
        scanf("%d", &meunNumber);
    
        switch (meunNumber) {
            case 1: return SVBSM;
            case 2: return TVM;
            case 3: return MM;
            default : printf("Input Error\n"); break;
        }
    }
}

MenuState scheduleViewByStatusMenu() {
    int statusNumer = 0, meunNumber = 0;
    
    // 상태 변경 확인
    // checkScheduleStatus();

    printf("================= [ 일정 상태별 조회 ] ----------------------\n");
    while (1) {
        printf("1:%s\n", CSTATUS[0]);
        printf("2:%s\n", CSTATUS[1]);
        printf("3:%s\n", CSTATUS[2]);
        printf("4:뒤로가기\n");
        printf("---------------------------------------------------------\n");
        printf("메뉴 선택 : ");
        scanf("%d", &statusNumer);
        
        if ((statusNumer >= 1 && statusNumer <= 3) && statusNumer != 4) break;
        else if (statusNumer == 4) return CVM;
        else printf("Input Error\n");
    }

    printf("--------------------------------------------------------\n");

    printf("[ %s -------------------------------------------- ]\n", CSTATUS[statusNumer - 1]);

    viewAllByStatus(CSTATUS[statusNumer - 1]);

    printf("--------------------------------------------------------\n");

    switch(statusNumer) {
        case 1: 
            // _TODO
            while (1) {
                printf("[ 선택 ----------------------------------------------- ]\n");
                printf("1. 일정 수정\n");
                printf("2. 일정 삭제\n");
                printf("3. 뒤로가기\n");
                printf("--------------------------------------------------------\n");
                printf("메뉴 선택 : ");
                scanf("%d", &meunNumber);
                
                switch (meunNumber) {
                    case 1: return SMM;
                    case 2: return DSM;
                    case 3: return SVBSM;
                    default : printf("Input Error\n"); break;
                }
            }
        case 2: 
            // _DOING
            while (1) {
                printf("[ 선택 ----------------------------------------------- ]\n");
                printf("1. 일정 완료\n");
                printf("2. 일정 연기\n");
                printf("3. 일정 삭제\n");
                printf("4. 뒤로가기\n");
                printf("--------------------------------------------------------\n");
                printf("메뉴 선택 : ");
                scanf("%d", &meunNumber);
            
                switch (meunNumber) {
                    case 1: return SCM;
                    case 2: return SPM;
                    case 3: return DSM;
                    case 4: return SVBSM;
                    default : printf("Input Error\n"); break;
                }
            }
        case 3: 
            // _DONE
            while (1) {
                printf("[ 선택 ----------------------------------------------- ]\n");
                printf("1. 일정 삭제\n");
                printf("2. 뒤로가기\n");
                printf("--------------------------------------------------------\n");
                printf("메뉴 선택 : ");
                scanf("%d", &meunNumber);
            
                switch (meunNumber) {
                    case 1: return DSM;
                    case 2: return SVBSM;
                    default : printf("Input Error\n"); break;
                }
            }
        default : printf("Return Error : %d\n", statusNumer); break;
    }

    printf("========================================================\n");
    
    return CVM;
}

MenuState scheduleModificationMenu() {
    Schedule *new_s = smalloc(), *old_s;
    char str[2] = "", sitme[6] = "", eitme[6] = "";
    int meunNumber = 0, sw = 1, id = 0;

    printf("[ 일정 수정 --------------------------------------------- ]\n");

    printf("수정할 일정 번호 선택 : ");
    scanf("%d", &meunNumber);

    id = statusIndexToId(CSTATUS[0], meunNumber);
    meunNumber = 0;
    
    if (id != -1) {
        old_s = idToStatusView(id);
    }
    else {
        printf("db Error");
        return SVBSM;
    }

    printf("---------------------------------------------------------\n");
    
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
    return SVBSM;
}

MenuState deleteScheduleMenu() {
    Schedule *s;
    char str[2] = "";
    int meunNumber = 0, id = 0;

    printf("[ 일정 삭제 --------------------------------------------- ]\n");

    printf("삭제할 일정 선택 : ");
    scanf("%d", &meunNumber);

    id = statusIndexToId(CSTATUS[0], meunNumber);
    
    if (id != -1) {
        s = idToStatusView(id);
    }
    else {
        printf("db Error");
        return SVBSM;
    }

    printf("---------------------------------------------------------\n");

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
    return SVBSM;
}

MenuState scheduleCompleteMenu() {
    Schedule *s;
    char str[2] = "";
    int meunNumber = 0, id = 0;

    // 상태 변경 확인
    // checkScheduleStatus();
    
    printf("[ 일정 완료 --------------------------------------------- ]\n");

    printf("완료할 일정 선택 : ");
    scanf("%d", &meunNumber);

    id = statusIndexToId(CSTATUS[1], meunNumber);
    
    if (id != -1) {
        s = idToStatusView(id);
    }
    else {
        printf("db Error");
        return SVBSM;
    }

    printf("---------------------------------------------------------\n");

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
    return SVBSM;
}

MenuState schedulePostponedMenu() {
    char str[2] = "", sitme[6] = "", eitme[6] = "";
    int meunNumber = 0, sw = 1, id = 0;
    Schedule *new_s = smalloc(), *old_s;
    
    printf("[ 일정 연기 ---------------------------------------------- ]\n");

    printf("연기할 일정 선택 : ");
    scanf("%d", &meunNumber);

    id = statusIndexToId(CSTATUS[1], meunNumber);
    meunNumber = 0;

    if (id != -1) {
        old_s = idToStatusView(id);
    }
    else {
        printf("db Error");
        return SVBSM;
    }

    printf("---------------------------------------------------------\n");

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
    return SVBSM;
}

MenuState tagViewMenu() {
    int meunNumber = 0;
    
    printf("================= [ 일정 조회 : 태그 ] ================\n");
    viewAllByTag();
    printf("[ 선택 ----------------------------------------------- ]\n");
    printf("1. 태크별 일정 조회\n");
    printf("2. 뒤로가기\n");
    printf("--------------------------------------------------------\n");
    
    while (1) {
        printf("메뉴 선택 : ");
        scanf("%d", &meunNumber);

        switch (meunNumber) {
            case 1: return SBTM;
            case 2: return CVM;
            default : printf("Input Error\n"); break;
        }
    }

    printf("========================================================\n");
}

MenuState scheduleByTagMenu() {
    char str[2] = "";
    int user_no = 0;
    TagCount *tc;

    printf("[ 태그별 일정 -------------------------------------------- ]\n");

    printf("확인할 태그 번호 선택 : ");
    scanf("%d", &user_no);

    tc = indexToTagCount(user_no);

    printf("--------------------------------------------------------\n");
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
    return TVM;
}