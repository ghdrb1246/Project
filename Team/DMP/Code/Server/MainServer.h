#ifndef _MS_
#define _MS_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DBM.h"
#include "InputInfo.h"
#include "CsvDB.h"

// Windows 환경
#ifdef _WIN32
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
    typedef int socklen_t;
    #define CLOSESOCKET closesocket

// macOS 환경
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #define SOCKET int
    #define CLOSESOCKET close
#endif

/**
 * @brief 클라이언트의 요청을 처리하는 메인 루프 함수
 *
 * @param clientSock 클라이언트 소켓 (각 클라이언트 연결마다 고유한 소켓 FD)
 *
 * @note - 클라이언트가 연결을 유지하는 동안, recv()로 요청을 수신 
 * @note - 요청을 processRequest()로 넘겨서 처리
 * @note - 처리 결과를 send()로 다시 클라이언트에게 전송
 * @note - 클라이언트가 접속을 종료하면 루프를 빠져나가고 소켓을 닫음
 */
void handleClient(SOCKET clientSock);

/**
 * @brief 클라이언트의 요청을 분석하고, 알맞은 응답을 준비하는 함수
 *
 * @param clientSock 클라이언트 소켓 (특수 케이스에서 서버가 직접 send()를 추가로 할 때 사용 가능)
 * @param request 클라이언트가 보낸 요청 문자열 (예: "INPUT_MEAL/사용자ID/날짜/음식명/g")
 * @param response 서버가 클라이언트에게 돌려줄 응답 메시지를 준비할 버퍼
 *
 * @note - 요청 문자열에서 명령어/데이터를 파싱 (예: "INPUT_MEAL", "SIGNUP" 등)
 * @note - 각 명령어별로 if-else로 분기 처리
 * @note - 처리 결과를 response에 snprintf() 등으로 작성
 * @note - 일부 경우 (예: GET_RECORD)는 response 대신 직접 send()를 호출 (예외 처리)
 */
void processRequest(SOCKET clientSock, char *request, char *response);

#endif