#ifndef CLIENT_CONTROLLER_H
#define CLIENT_CONTROLLER_H

#include <stdio.h>
#include <string.h>
#include "MenuIO.h"
#include "ClientController.h"
#include "InputInfo.h"

// Windows 환경
#ifdef _WIN32
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")

// macOS 환경
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
#endif

/**
 * @brief 클라이언트 프로그램의 전역 로그인 상태를 저장하는 변수
 *
 * @note - 프로그램이 로그인 상태를 유지할 때, 접속한 사용자의 ID를 저장
 * @note - 로그인 성공 시 할당되며, 로그아웃/회원탈퇴 시 빈 문자열로 초기화
 * @note - 프로그램이 종료될 때까지 전역적으로 유지되며, 사용자 메뉴에서 사용됨
 * @note - 예: 로그인 후 "사용자 메뉴" 등에서 해당 ID를 참조하여 사용자별 데이터 처리
 */
static char loggedInUserId[ID_SIZE];

/**
 * @brief 클라이언트가 서버에 메시지를 전송하고, 단순 응답을 출력하는 함수
 *
 * @param sock 서버와 연결된 소켓 디스크립터 (서버와의 통신에 사용됨)
 * @param message 클라이언트가 서버로 보낼 요청 문자열 (예: "INPUT_MEAL/사용자ID/날짜/음식명/g")
 *
 * @note - send()로 서버에 message를 전송
 * @note - recv()로 서버의 응답을 수신 후 표준 출력으로 응답 메시지를 출력
 * @note - 받은 응답은 단순 출력만 하고 추가 처리/저장은 하지 않음
 */
void sendRequest(int sock, const char *message);

/**
 * @brief 클라이언트가 서버에 메시지를 전송하고, 응답을 받아 반환하는 함수
 *
 * @param sock 서버와 연결된 소켓 디스크립터
 * @param message 서버로 보낼 요청 문자열
 * @param response 서버로부터 받은 응답 문자열을 저장할 버퍼 (호출자에게 결과 전달)
 * @return 성공적으로 응답을 받으면 1, recv()가 실패하면 0을 반환
 *
 * @note - send()로 서버에 요청을 전송
 * @note - recv()로 서버의 응답을 수신
 * @note - 응답 문자열을 response 버퍼에 복사하여 호출자에게 전달
 */
int sendRequestWithResponse(int sock, const char *message, char *response);

/**
 * @brief 클라이언트에서 메인 메뉴를 출력하고, 사용자 선택에 따라 동작을 처리하는 함수
 *
 * @param sock 서버와 연결된 소켓 디스크립터
 * @return 다음 상태 (STATE_MAIN_MENU, STATE_USER_MENU, STATE_EXIT 등)
 *
 * @note - 메인 메뉴 항목을 사용자에게 출력
 * @note - 사용자의 선택에 따라:
 * @note      | 회원가입 (서버에 요청 전송)
 * @note      | 로그인 (서버에 요청 전송 및 응답으로 성공/실패 판별)
 * @note      | 프로그램 종료
 * @note - 선택 결과에 따라 다음 메뉴 상태를 반환
 */
MenuState handleMainMenu(int sock);

/**
 * @brief 클라이언트에서 사용자 메뉴를 출력하고, 사용자 선택에 따라 동작을 처리하는 함수
 *
 * @param sock 서버와 연결된 소켓 디스크립터
 * @return 다음 상태 (STATE_USER_MENU, STATE_MAIN_MENU 등)
 *
 * @note - 사용자 메뉴 항목(식단 입력, 운동 입력, 체중 입력 등)을 출력
 * @note - 사용자 입력 데이터를 받아서 서버에 요청 전송
 * @note - 서버 응답에 따라:
 * @note     | 루프를 반복하거나
 * @note     | 메뉴를 종료하거나
 * @note     | 사용자에게 다시 입력을 요구
 * @note - 최종적으로 다음 메뉴 상태를 반환 (로그아웃/회원탈퇴 시 STATE_MAIN_MENU 등)
 */
MenuState handleUserMenu(int sock);

#endif