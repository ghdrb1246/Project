#ifdef _WIN32
    #define CLOSESOCKET closesocket
#else
    #define SOCKET int
#endif

#define PORT 12345
#define BUF_SIZE 1024
#define SERVER_IP "127.0.0.1"

char sendBuf[BUF_SIZE], recvBuf[BUF_SIZE];

/**
 * @brief 요청 보내기
 * 
 * @param sock sock 
 * @param message 메시지
 */
void sendRequest(SOCKET sock, const char *message);