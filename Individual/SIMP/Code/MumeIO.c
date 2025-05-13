#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "MumeIO.h"
#include "DB_Management.h"
#include "UserStructure.h"
#include "Utils.h"

static const char *CSTATUS[3] = { "TODO", "DOING", "DONE" };
static int deleteStatusNumer = 0;
/* ================== 메뉴 입출력 모듈 ================== */

/* ----------------- Output 관련 함수 ----------------- */
void runMenu() {
    if (DBO("Schedule")) return;

    MenuState current = MM;
    while (current != EXIT) {
        switch (current) {
            case MM:    current = mainMenu();                  break;
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

MenuState mainMenu() {
    int meunNumber = 0;
   
    // 상태 변경 확인
    checkScheduleStatus();
    
    while(1) {
        P_MENU_TITLE("일정 관리 프로그램");
        // printf("================= [ 일정 관리 프로그램 ] =================\n");
        P_MENU_SB_S("선택", "--------");

        printf("1. 일정 등록\n");
        printf("2. 일정 조회\n");
        printf("3. 종료하기\n");
        
        P_MENU_IN;
        
        printf("메뉴 선택 : ");
        scanf("%d", &meunNumber);
        clearInputBuffer(); // 버퍼 제거
        
        P_MENU_IN;
        P_MENU_END;
        
        switch (meunNumber) {
            case 1: return SRM;
            case 2: return CVM;
            case 3: return EXIT;
            default : printf("%d 은 메뉴 번호가 아닙니다.\n", meunNumber); break;
        }
    }
}

MenuState scheduleRegistrationMenu() {
    char str[2] = "", sitme[6] = "", eitme[6] = "";
    int sw = 1;
    Schedule *s = smalloc();

    // P_MENU_END;

    while (sw) {
        P_MENU_TITLE("    일정  등록    ");
        // printf("===================== [ 일정 등록 ] =====================\n");
        
        /* while (1) {
            printf("제목 : "); 
            // scanf("%s", s->title);
            fgets(s->title, TITLE_SIZE, stdin);
            s->title[strcspn(s->title, "\n")] = 0;
    
            if (strlen(s->title) >= TITLE_SIZE - 1) {
                printf("제목이 너무 깁니다. 다시 입력해주세요.\n");
            } 
            else if (strlen(s->title) == 0) {
                printf("제목이 비어있습니다. 다시 입력하세요.\n");
            } 
            else break;
        } */
        
        inputTitle(s->title);
        
        /* 
        printf("예정 날짜(YYYY-MM-DD) : "); 
        scanf("%s", s->scheduled_date_time);
        
        printf("-> 예정 시간(HH:MM) : ");
        scanf("%s", sitme);
        strcat(s->scheduled_date_time, " ");
        strcat(s->scheduled_date_time, sitme); */
        
        inputSDT(s->scheduled_date_time);

       /*  while (1) {
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
 */
        
        inputEDT(s->end_date_time);


        /* printf("태그 : "); 
        scanf("%s", s->tag); */
        
        inputTag(s->tag);

        /* while (1) {
            printf("우선순위(0:없음, 1:낮음, 2:중간, 3:높음) : "); 
            scanf("%d", &s->priority);

            if (s->priority >= 0 && s->priority <= 3) break;
            else printf("Input Error : %d\n", s->priority);
        } */
        
        s->priority = inputPriority();

        P_MENU_IN;
        // printf("---------------------------------------------------------\n");

        while (1) {
            printf("일정을 등록하시겠습니까?[Y/N] : ");
            scanf("%s", str);

            P_MENU_IN;
            // printf("---------------------------------------------------------\n");

            if (strcmp(str, "Y") == 0 || strcmp(str, "y") == 0) {
                saveDB(s);
                printf("일정을 저장되었습니다.\n\n");
                sw = 0;
                break;
            }
            else if (strcmp(str, "N") == 0 || strcmp(str, "n") == 0) { 
                printf("취소.\n");
                sw = 0;
                break;
            }
            else printf("%s은 잘못 된 입력입니다.\n", str);
        }
    }

    // printf("%s %s %s %s %d\n", s->title, s->scheduled_date_time, s->end_date_time, s->tag, s->priority);
    
    P_MENU_IN;
    // printf("---------------------------------------------------------\n");
    P_MENU_END;
    // printf("=========================================================\n");
    
    sfree(s);
    return MM;
}

MenuState calendarViewMenu() {
    int meunNumber = 0;
    
    // P_MENU_END;

    while (1) {
        P_MENU_TITLE("    일정  조회    ");
        // printf("===================== [ 일정 조회 ] =====================\n");
        P_MENU_SB_S("선택", "--------");

        printf("1. 상태별 조회\n");
        printf("2. 태그 조회\n");
        printf("3. 뒤로가기\n");

        P_MENU_IN;
        // printf("---------------------------------------------------------\n");

        printf("메뉴 선택 : ");
        scanf("%d", &meunNumber);
        
        P_MENU_IN;
        // printf("---------------------------------------------------------\n");

        switch (meunNumber) {
            case 1: return SVBSM;
            case 2: return TVM;
            case 3: P_MENU_END; return MM;
            default : printf("%d 은 메뉴 번호가 아닙니다.\n", meunNumber); break;
        }
    }
}

MenuState scheduleViewByStatusMenu() {
    int statusNumer = 0, meunNumber = 0;
    
    // 상태 변경 확인
    checkScheduleStatus();

    P_MENU_END;

    while (1) {
        P_MENU_SB_S("선택", "--------");
        // printf("[ 선택 ----------------------------------------------- ]\n");

        printf("1:%s\n", CSTATUS[0]);
        printf("2:%s\n", CSTATUS[1]);
        printf("3:%s\n", CSTATUS[2]);
        printf("4:뒤로 가기\n");

        P_MENU_IN;
        // printf("---------------------------------------------------------\n");
        
        printf("메뉴 선택 : ");
        scanf("%d", &statusNumer);
        
        P_MENU_IN;
        // printf("--------------------------------------------------------\n");

        if ((statusNumer >= 1 && statusNumer <= 3) && statusNumer != 4) break;
        else if (statusNumer == 4) return CVM;
        else printf("Input Error\n");
    }
    
    P_MENU_END;

    P_MENU_TITLE(" 일정 상태별 조회 ");
    // printf("================= [ 일정 상태별 조회 ] =================\n");

    P_MENU_SB_S(CSTATUS[statusNumer - 1], (statusNumer == 2) ? "-------" : "--------");
    // printf("[ %s ----------------------------------------------- ]\n", CSTATUS[statusNumer - 1]);

    if (viewAllByStatus(CSTATUS[statusNumer - 1]) != 1) {
        printf("%s 상태의 일정이 없습니다.\n", CSTATUS[statusNumer - 1]);
        P_MENU_IN;
        P_MENU_END;
        return SVBSM;
    }
    P_MENU_IN;
    // printf("--------------------------------------------------------\n");

    switch(statusNumer) {
        case 1: 
            // _TODO
            while (1) {
                P_MENU_SB_S("선택", "--------");
                // printf("[ 선택 ----------------------------------------------- ]\n");

                printf("1. 일정 수정\n");
                printf("2. 일정 삭제\n");
                printf("3. 뒤로가기\n");

                P_MENU_IN;
                // printf("--------------------------------------------------------\n");

                printf("메뉴 선택 : ");
                scanf("%d", &meunNumber);
                
                P_MENU_IN;
                
                switch (meunNumber) {
                    case 1: return SMM;
                    case 2: deleteStatusNumer = 1; return DSM;
                    case 3: return SVBSM;
                    default : printf("Input Error\n"); break;
                }
            }
        case 2: 
            // _DOING
            while (1) {
                P_MENU_SB_S("선택", "--------");
                // printf("[ 선택 ----------------------------------------------- ]\n");

                printf("1. 일정 완료\n");
                printf("2. 일정 연기\n");
                printf("3. 일정 삭제\n");
                printf("4. 뒤로가기\n");
                
                P_MENU_IN;
                // printf("--------------------------------------------------------\n");
                
                printf("메뉴 선택 : ");
                scanf("%d", &meunNumber);
                
                P_MENU_IN;
                // printf("--------------------------------------------------------\n");

                switch (meunNumber) {
                    case 1: return SCM;
                    case 2: return SPM;
                    case 3: deleteStatusNumer = 2; return DSM;
                    case 4: return SVBSM;
                    default : printf("Input Error\n"); break;
                }
            }
        case 3: 
            // _DONE
            while (1) {
                P_MENU_SB_S("선택", "--------");
                // printf("[ 선택 ----------------------------------------------- ]\n");
                
                printf("1. 일정 삭제\n");
                printf("2. 뒤로가기\n");
                
                P_MENU_IN;
                // printf("--------------------------------------------------------\n");
                
                printf("메뉴 선택 : ");
                scanf("%d", &meunNumber);
                
                P_MENU_IN;

                switch (meunNumber) {
                    case 1: deleteStatusNumer = 3; return DSM;
                    case 2: return SVBSM;
                    default : printf("Input Error\n"); break;
                }
            }
        default : printf("%d 은 메뉴 번호가 아닙니다.\n", meunNumber); break;
    }

    // P_MENU_END;
    // printf("========================================================\n");
    
    return CVM;
}

MenuState scheduleModificationMenu() {
    Schedule *new_s = smalloc(), *old_s;
    char str[2] = "", sitme[6] = "", eitme[6] = "";
    int meunNumber = 0, sw = 1, id = 0;

    P_MENU_SB_S("일정 수정", "---");
    // printf("[ 일정 수정 --------------------------------------------- ]\n");

    printf("수정할 일정 번호 선택 : ");
    scanf("%d", &meunNumber);

    id = statusIndexToId(CSTATUS[0], meunNumber);
    meunNumber = 0;
    
    if (id != -1) {
        old_s = idToStatusView(id);
    }
    else {
        // printf("db Error\n");
        return SVBSM;
    }

    P_MENU_IN;
    // printf("---------------------------------------------------------\n");
    
    printf("수정 대상 : %s, %s, %s, %s, %d\n", old_s->title, old_s->scheduled_date_time, old_s->end_date_time, old_s->tag, old_s->priority);
    
    P_MENU_IN;
    // printf("---------------------------------------------------------\n");
    
    while (sw) {
        P_MENU_SB_S("선택", "--------");

        printf("1. 제목\n");
        printf("2. 예정 날짜/시간\n");
        printf("3. 종료 날짜/시간\n");
        printf("4. 태그\n");
        printf("5. 우선순위\n");
        printf("6. 저장\n");
        
        P_MENU_SB;
        // printf("[ ----------------------------------------------------- ]\n");
        
        printf("메뉴 선택 : ");
        scanf("%d", &meunNumber);
        clearInputBuffer(); // 버퍼 제거

        P_MENU_IN;
        // printf("---------------------------------------------------------\n");

        switch (meunNumber) {
            case 1:

                /* printf("제목 : "); 
                // scanf("%s", new_s->title);
                fgets(new_s->title, TITLE_SIZE, stdin);
                new_s->title[strlen(new_s->title) - 1] = '\0';
                */
                inputTitle(new_s->title);
                strcpy(old_s->title, new_s->title);
            break;
    
            case 2:
                /* printf("예정 날짜(YYYY-MM-DD) : "); 
                scanf("%s", new_s->scheduled_date_time);
    
                printf("-> 예정 시간(HH:MM) : ");
                scanf("%s", sitme);
                strcat(new_s->scheduled_date_time, " ");
                strcat(new_s->scheduled_date_time, sitme); */
                inputSDT(new_s->scheduled_date_time);
                strcpy(old_s->scheduled_date_time, new_s->scheduled_date_time);
            break;
    
            case 3:
                /* printf("종료 날짜(YYYY-MM-DD or x) : ");
                scanf("%s", new_s->end_date_time);
                printf("-> 종료 시간(HH:MM) : ");
                scanf("%s", eitme);
                strcat(new_s->end_date_time, " ");
                strcat(new_s->end_date_time, eitme); */
                inputEDT(new_s->end_date_time);
                strcpy(old_s->end_date_time, new_s->end_date_time);
            break;
    
            case 4:
                /* printf("태그 : "); 
                scanf("%s", new_s->tag); */
                inputTag(new_s->tag);
                strcpy(old_s->tag, new_s->tag);
            break;
    
            case 5:
                /* while (1) {
                    printf("우선순위(0:없음, 1:낮음, 2:중간, 3:높음) : "); 
                    scanf("%d", &new_s->priority);
            
                    if (new_s->priority >= 0 && new_s->priority <= 3) break;
                    else printf("Input Error : %d\n", new_s->priority);
                } */
                new_s->priority = inputPriority();
                old_s->priority = new_s->priority;
            break;
    
            case 6:
                while (1) {
                    P_MENU_IN;
                    // printf("---------------------------------------------------------\n");
    
                    printf("선택한 일정을 수정하시겠습니까?[Y/N] : ");
                    scanf("%s", str);

                    P_MENU_IN;
                    // printf("---------------------------------------------------------\n");
            
                    if (strcmp(str, "Y") == 0 || strcmp(str, "y") == 0) {
                        updateDB(old_s, id);
                        // printf("%s %s %s %s %d\n", old_s->title, old_s->scheduled_date_time, old_s->end_date_time, old_s->tag, old_s->priority);
                        printf("일정을 저장되었습니다.\n"); 
                        sw = 0;                       
                        break;
                    }

                    else if (strcmp(str, "N") == 0 || strcmp(str, "n") == 0) { 
                        printf("취소.\n");
                        sw = 0;
                        break;
                    }

                    else printf("%s은 잘못 된 입력입니다.\n", str);
                }
            break;
            
            default : printf("%d 은 메뉴 번호가 아닙니다.\n", meunNumber); break;
        }
    }

    P_MENU_IN;
    P_MENU_END;
    // printf("=========================================================\n");
    
    sfree(new_s);
    sfree(old_s);
    return SVBSM;
}

MenuState deleteScheduleMenu() {
    Schedule *s;
    char str[2] = "";
    int meunNumber = 0, id = 0;

    P_MENU_SB_S("일정 삭제", "---");
    // printf("[ 일정 삭제 --------------------------------------------- ]\n");

    printf("삭제할 일정 선택 : ");
    scanf("%d", &meunNumber);
    
    P_MENU_IN;
    // printf("---------------------------------------------------------\n");

    id = statusIndexToId(CSTATUS[deleteStatusNumer - 1], meunNumber);
    
    if (id != -1) {
        s = idToStatusView(id);
    }
    else {
        // printf("db Error\n");
        return SVBSM;
    }

    printf("삭제 대상 : %s, %s, %s, %s, %d\n", s->title, s->scheduled_date_time, s->end_date_time, s->tag, s->priority);
    
    P_MENU_SB;
    // printf("[ ----------------------------------------------------- ]\n");
    
    while (1) {
        printf("선택한 일정을 삭제하시겠습니까?[Y/N] : ");
        scanf("%s", str);
        
        P_MENU_IN;

        if (strcmp(str, "Y") == 0 || strcmp(str, "y") == 0) {
            deleteDB(id);
            printf("일정을 삭제되었습니다.\n");
            break;
        }
        else if (strcmp(str, "N") == 0 || strcmp(str, "n") == 0) { 
            printf("취소.\n");
            break;
        }
        else printf("%s은 잘못 된 입력입니다.\n", str);
    }

    P_MENU_IN;
    P_MENU_END;
    // printf("=========================================================\n");

    sfree(s);
    return SVBSM;
}

MenuState scheduleCompleteMenu() {
    Schedule *s;
    char str[2] = "";
    int meunNumber = 0, id = 0;

    // 상태 변경 확인
    checkScheduleStatus();
    
    P_MENU_SB_S("일정 완료", "---");
    // printf("[ 일정 완료 --------------------------------------------- ]\n");

    printf("완료할 일정 선택 : ");
    scanf("%d", &meunNumber);

    P_MENU_IN;
    // printf("---------------------------------------------------------\n");
    
    id = statusIndexToId(CSTATUS[1], meunNumber);
    
    if (id != -1) {
        s = idToStatusView(id);
    }
    else {
        // printf("db Error\n");
        return SVBSM;
    }

    printf("완료 대상 : %s, %s, %s, %s, %d\n", s->title, s->scheduled_date_time, s->end_date_time, s->tag, s->priority);
    
    P_MENU_SB;
    // printf("[ ----------------------------------------------------- ]\n");
    
    while (1) {
        printf("선택한 일정을 완료하시셨습니까?[Y/N] : ");
        scanf("%s", str);
        
        P_MENU_IN;

        if (strcmp(str, "Y") == 0 || strcmp(str, "y") == 0) {
            updateStatus("DONE", id);
            printf("완료 처리 되었습니다.\n");
            break;
        }
        else if (strcmp(str, "N") == 0 || strcmp(str, "n") == 0) { 
            printf("취소.\n");
            break;
        }
        else printf("%s은 잘못 된 입력입니다.\n", str);
    }
    
    P_MENU_IN;
    P_MENU_END;
    // printf("=========================================================\n");

    sfree(s);
    return SVBSM;
}

MenuState schedulePostponedMenu() {
    char str[2] = "", sitme[6] = "", eitme[6] = "";
    int meunNumber = 0, sw = 1, id = 0;
    Schedule *new_s = smalloc(), *old_s;
    
    P_MENU_SB_S("일정 연기", "---");
    // printf("[ 일정 연기 ---------------------------------------------- ]\n");

    printf("연기할 일정 선택 : ");
    scanf("%d", &meunNumber);
    
    P_MENU_IN;
    // printf("---------------------------------------------------------\n");

    id = statusIndexToId(CSTATUS[1], meunNumber);
    meunNumber = 0;

    if (id != -1) {
        old_s = idToStatusView(id);
    }
    else {
        // printf("db Error\n");
        return SVBSM;
    }

    printf("연기 대상 : %s, %s, %s, %s, %d\n", old_s->title, old_s->scheduled_date_time, old_s->end_date_time, old_s->tag, old_s->priority);
    
    P_MENU_IN;
    // printf("---------------------------------------------------------\n");

    while (sw) {
        P_MENU_SB_S("선택", "--------");
        printf("1. 예정 날짜/시간\n");
        printf("2. 종료 날짜/시간\n");
        printf("3. 저장\n");

        P_MENU_IN;
        scanf("%d", &meunNumber);
        clearInputBuffer(); // 퍼버 제거
        P_MENU_SB;

        switch (meunNumber) {
            case 1:
                /* printf("예정 날짜/시간(YYYY-MM-DD HH:MM) : \n");  
                printf("%s -> ", old_s->scheduled_date_time); 
                scanf("%s %s", new_s->scheduled_date_time, sitme);
                strcat(new_s->scheduled_date_time, " ");
                strcat(new_s->scheduled_date_time, sitme); */
                inputSDT(old_s->scheduled_date_time);
                strcpy(old_s->scheduled_date_time, new_s->scheduled_date_time);
            break;
            
            case 2:
                /* printf("종료 날짜/시간(YYYY-MM-DD HH:MM) : \n"); 
                printf("%s -> ", old_s->end_date_time); 
                scanf("%s %s", new_s->end_date_time, eitme);
                strcat(new_s->end_date_time, " ");
                strcat(new_s->end_date_time, eitme); */
                inputEDT(old_s->end_date_time);
                strcpy(old_s->end_date_time, new_s->end_date_time);
            break;

            case 3:
                while (1) {
                    P_MENU_SB;
                    // printf("[ ----------------------------------------------------- ]\n");

                    printf("선택한 일정을 연기 하시겠습니까?[Y/N] : ");
                    scanf("%s", str);
                
                    if (strcmp(str, "Y") == 0 || strcmp(str, "y") == 0) {
                        updateDB(old_s, id);
                        printf("일정을 연기 되었습니다.\n");
                        sw = 0;
                        break;
                    }
                    else if (strcmp(str, "N") == 0 || strcmp(str, "n") == 0) { 
                        printf("취소.\n");
                        sw = 0;
                        break;
                    }
                    else printf("%s은 잘못 된 입력입니다.\n", str);
                }
            break;
            
            default : printf("%d 은 메뉴 번호가 아닙니다.\n", meunNumber); break;
        }
    }
    
    P_MENU_IN;
    P_MENU_END;
    // printf("=========================================================\n");

    sfree(new_s);
    sfree(old_s);
    return SVBSM;
}

MenuState tagViewMenu() {
    int meunNumber = 0;
    
    P_MENU_TITLE("  일정 태그 조회  ");
    // printf("================= [ 일정 조회 : 태그 ] ================\n");

    P_MENU_SB_S("태그", "--------");
    viewAllByTag();

    P_MENU_SB_S("선택", "--------");
    // printf("[ 선택 ----------------------------------------------- ]\n");

    printf("1. 태크별 일정 조회\n");
    printf("2. 뒤로가기\n");
    
    P_MENU_IN;
    
    while (1) {
        printf("메뉴 선택 : ");
        scanf("%d", &meunNumber);
        P_MENU_IN;

        switch (meunNumber) {
            case 1: return SBTM;
            case 2: return CVM;
            default : printf("%d 은 메뉴 번호가 아닙니다.\n", meunNumber); break;
        }
    }
}

MenuState scheduleByTagMenu() {
    char str[2] = "";
    int user_no = 0;
    TagCount *tc;

    P_MENU_SB_S("태그별 일정", "-");
    // printf("[ 태그별 일정 -------------------------------------------- ]\n");

    printf("확인할 태그 번호 선택 : ");
    scanf("%d", &user_no);

    tc = indexToTagCount(user_no);

    P_MENU_IN;
    // printf("--------------------------------------------------------\n");
    P_MENU_TC(tc->tag, tc->count);
    // printf("[ %s | %d -------------------------------------------- ]\n", tc->tag, tc->count);
    P_MENU_SB;
    // printf("[ ----------------------------------------------------- ]\n");
    
    viewTagByschedule(tc->tag);
    
    P_MENU_IN;
    
    while (1) {
        P_MENU_SB_S("선택", "--------");

        printf("뒤로 가기[Y/N] : ");
        scanf("%s", str);
        P_MENU_IN;

        if (strcmp(str, "Y") == 0 || strcmp(str, "y") == 0) {
            printf("\n뒤로 갑니다.\n\n");
            break;
        }
        else if (strcmp(str, "N") == 0 || strcmp(str, "n") == 0) { 
            printf("\n취소.\n\n");
        }
        else printf("%s은 잘못 된 입력입니다.\n", str);
    }

    P_MENU_IN;
    P_MENU_END;
    // printf("=========================================================\n");

    tcfree(tc);
    return TVM;
}
/* --------------------------------------------------------- */

/* --------------------- Input 관련 함수 --------------------- */

int isValidDateTime(char *datetime) {
    int y, m, d, h, min;
    int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    
    // "YYYY-MM-DD HH:MM" → 총 16자 (문자열 + 널 포함 17바이트)
    if (strlen(datetime) != 16 || datetime[4] != '-' || datetime[7] != '-' || datetime[10] != ' ' || datetime[13] != ':') {
        return 0;
    }

    // 날짜와 시간 추출
    else if (sscanf(datetime, "%4d-%2d-%2d %2d:%2d", &y, &m, &d, &h, &min) != 5) {
        return 0;
    }

    // 날짜 범위 검사
    if ((y < 1900 || y > 2100) || (m < 1 || m > 12) || (d < 1 || d > 31)) return 0;

    // 월별 최대 일자 확인 (윤년 고려)
    if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0) {
        daysInMonth[1] = 29;  // 윤년
    }
    if (d > daysInMonth[m - 1]) return 0;

    // 시간 범위 검사
    if ((h < 0 || h > 23) || (min < 0 || min > 59)) return 0;


return 1; // 유효한 날짜+시간
}

void inputTitle(char *title) {
    while (1) {
        printf("제목 : "); 
        // scanf("%s", s->title);
        fgets(title, TITLE_SIZE, stdin);
        // title[strlen(title) - 1] = '\0';
        title[strcspn(title, "\n")] = 0;

        if (strlen(title) >= TITLE_SIZE - 1) {
            printf("제목이 너무 깁니다.\n최대 %d 까지 가능 합니다.\n다시 입력해주세요.\n", TITLE_SIZE - 1);
        } 
        else if (strlen(title) == 0) {
            printf("제목이 비어있습니다. 다시 입력하세요.\n");
        } 
        else break;
    }
}

void inputSDT(char *sdt) {
    char sitme[6] = "";

    while (1) {
        printf("예정 날짜(YYYY-MM-DD) : "); 
        scanf("%s", sdt);
        
        printf("-> 예정 시간(HH:MM) : ");
        scanf("%s", sitme);
        strcat(sdt, " ");
        strcat(sdt, sitme);
        
        /* printf("예정 날짜(YYYY-MM-DD HH:MM) : ");
        fgets(sdt, DT_SIZE, stdin);
        // sdt[strlen(sdt) - 1] = '\0';
        sdt[strcspn(sdt, "\n")] = 0; */

        if (isValidDateTime(sdt)) break;
        else printf("유효하지 않은 날짜 형식입니다.\n");
    }
}

void inputEDT(char *edt) {
    char eitme[6] = "";

     while (1) {
        printf("종료 날짜(YYYY-MM-DD) or x: "); 
        scanf("%s", edt);
        
        /* printf("종료 날짜(YYYY-MM-DD HH:MM) or x : ");
        fgets(edt, DT_SIZE, stdin);
        // edt[strlen(edt) - 1] = '\0';
        edt[strcspn(edt, "\n")] = 0; */
        
        if (strcmp(edt, "x") == 0 || strcmp(edt, "X") == 0) {
            strcpy(edt, "NULL");
            break;
        }
        else {
            printf("-> 종료 시간(HH:MM) : ");
            scanf("%s", eitme);
            strcat(edt, " ");
            strcat(edt, eitme);
            
            if (isValidDateTime(edt)) break;
            else printf("유효하지 않은 날짜 형식입니다.\n");
        }
    }
/* 
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

    else printf("Input Error\n"); */
}

void inputTag(char *tag) {
    while (1) {
        printf("태그 : "); 
        scanf("%s", tag);

        if (strlen(tag) >= TAG_SIZE - 1) {
            printf("태그가 너무 깁니다.\n최대 %d 까지 가능 합니다.\n다시 입력해주세요.\n", TAG_SIZE - 1);
        } 
        else if (strlen(tag) == 0) {
            printf("태그가 비어있습니다.\n다시 입력하세요.\n");
        } 
        else break;
    }
}

int inputPriority() {
    int priority = 0;

    while (1) {
        printf("우선순위(0:없음, 1:낮음, 2:중간, 3:높음) : "); 
        scanf("%d", &priority);

        if (priority >= 0 && priority <= 3) break;
        else printf("%d 은 입력 범위가 아닙니다.\n", priority);
    }

    return priority;
}

// 확인할 "%s" 번호 선택 함수

/* --------------------------------------------------------- */