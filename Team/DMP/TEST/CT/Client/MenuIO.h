#ifndef MENU_IO_H
#define MENU_IO_H

#define P_MENU_TITLE(s)     printf("================= [ %s ] =================\n", s)
#define P_MENU_IN           printf("----------------------------------------------------------\n")
#define P_MENU_END          printf("==========================================================\n\n")
#define P_MENU_SB_S(s, l)   printf("[ %s %s------------------------------------------ ]\n", s, l)

/* OUT */

int mainMenu();
void signupMenu(char *id, char *pw);
void loginMenu(char *id, char *pw);
int userMenu(char *id);
void mealMenu(char *mealDateTime, char *foodName, float *gram);
void workOutMenu(char *workOutDateTime, char *workOutname, float *Time);
void weightMenu(char *weightDateTime, float *weight);
void viewRecordsByDateMenu();
void checkWeightLossProgressMenu(const float initialWeight, const float goalWeight, const float currentWeight);
void feedBackMenu(const float weight, const float kcal);
void logOutMenu(const char *id);
void deleteIdMenu(const char *id);

/* In */

void inputLine(const char *prompt, char *buf, int size);
int inputMenu(const char *prompt, const char *type);

#endif