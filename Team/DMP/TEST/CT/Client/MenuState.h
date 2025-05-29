#ifndef MENU_STATE_H
#define MENU_STATE_H

enum MenuState {
    STATE_MAIN_MENU,
    STATE_SIGNUP,
    STATE_LOGIN,
    STATE_USER_MENU,
    STATE_EXIT
};

/**
 * @brief 메뉴 이동
 * 
 */
typedef enum MenuState MenuState;

#endif