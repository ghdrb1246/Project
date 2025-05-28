#define P_MENU_TITLE(s)     printf("================= [ %s ] =================\n", s)
#define P_MENU_IN           printf("----------------------------------------------------------\n")
#define P_MENU_END          printf("==========================================================\n\n")
#define P_MENU_SB_S(s, l)   printf("[ %s %s------------------------------------------ ]\n", s, l)

enum MenuState {
   EXIT, MM, SIGNUP, LOGIN
};

/**
 * @brief runMenu
 * 
 * @param EXIT  종료
 * @param MM    MainMenu()
 * @param SIGNUP   SignupMenu()
 * @param LOGIN   LoginMenu()
 */
typedef enum MenuState MenuState;

void runMenu();
MenuState mainMenu();
MenuState signupMenu();
MenuState LoginMenu();