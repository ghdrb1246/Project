#ifndef MENU_IO_H
#define MENU_IO_H

#include "InputInfo.h"

#define P_MENU_TITLE(s)     printf("================= [ %s ] =================\n", s)
#define P_MENU_IN           printf("----------------------------------------------------------\n")
#define P_MENU_END          printf("==========================================================\n\n")
#define P_MENU_SB_S(s, l)   printf("[ %s %s------------------------------------------ ]\n", s, l)

/* OUT */

int mainMenu();
void signupMenu(UserSignupInfo *USI);
void loginMenu(char *id, char *pw);
int userMenu(char *id);
void mealMenu(MealInputInfo *MII);
void workOutMenu(WorkOutInputInfo *WOII);
void weightMenu(WeightInputInfo *WII);
void viewRecordsByDateMenu();
void checkWeightLossProgressMenu(const float initialWeight, const float goalWeight, const float currentWeight);
void feedBackMenu(const float weight, const float kcal);
void logOutMenu(const char *id);
void deleteIdMenu(const char *id);

/* In */

void inputLine(const char *prompt, char *buf, int size);
int inputMeunNum(const char *prompt);
void inputDateTime(char *dateTime);
int validDateTime(char *datetime);

#endif