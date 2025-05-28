//출처 http://remocon33.tistory.com/465

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// macOS/Unix 호환 server.c
#if defined(_WIN32) || defined(_WIN64)
	#error "이 코드는 Unix/Linux 환경에서만 작동합니다."
	#include <Windows.h>
	#include <process.h>
#else
    #include <unistd.h>
    #include <pthread.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <signal.h>
#endif

#define BUF_SIZE 100
#define MAX_CLNT 256

void* HandleClient(void* arg);
void SendMsg(char* msg, int len);
void ErrorHandling(const char* msg);

int clientCount = 0;
int clientSocks[MAX_CLNT];
pthread_mutex_t mutex;

int main() {
    int serverSock, clientSock;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrSize;
    pthread_t t_id;

    char portStr[100];
    int port;

    printf("Input port number: ");
    fgets(portStr, sizeof(portStr), stdin);
    port = atoi(portStr);

    signal(SIGPIPE, SIG_IGN);
    pthread_mutex_init(&mutex, NULL);

    serverSock = socket(PF_INET, SOCK_STREAM, 0);
    if (serverSock == -1)
        ErrorHandling("socket() error");

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(port);

    if (bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
        ErrorHandling("bind() error");

    if (listen(serverSock, 5) == -1)
        ErrorHandling("listen() error");

    while (1) {
        clientAddrSize = sizeof(clientAddr);
        clientSock = accept(serverSock, (struct sockaddr*)&clientAddr, &clientAddrSize);

        pthread_mutex_lock(&mutex);
        clientSocks[clientCount++] = clientSock;
        pthread_mutex_unlock(&mutex);

        pthread_create(&t_id, NULL, HandleClient, (void*)&clientSock);
        pthread_detach(t_id);

        printf("Connected client IP: %s\n", inet_ntoa(clientAddr.sin_addr));
    }

    close(serverSock);
    return 0;
}

void* HandleClient(void* arg) {
    int sock = *((int*)arg);
    int strLen = 0;
    char msg[BUF_SIZE];

    while ((strLen = read(sock, msg, sizeof(msg))) != 0)
        SendMsg(msg, strLen);

    pthread_mutex_lock(&mutex);
    for (int i = 0; i < clientCount; i++) {
        if (sock == clientSocks[i]) {
            for (int j = i; j < clientCount - 1; j++)
                clientSocks[j] = clientSocks[j + 1];
            break;
        }
    }
    clientCount--;
    pthread_mutex_unlock(&mutex);

    close(sock);
    return NULL;
}

void SendMsg(char* msg, int len) {
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < clientCount; i++)
        write(clientSocks[i], msg, len);
    pthread_mutex_unlock(&mutex);
}

void ErrorHandling(const char* msg) {
    perror(msg);
    exit(1);
}
