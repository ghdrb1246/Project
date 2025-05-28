//출처 http://remocon33.tistory.com/465

// macOS/Unix 호환 client.c
#if defined(_WIN32) || defined(_WIN64)
#error "이 코드는 Unix/Linux 환경에서만 작동합니다."
#include <Windows.h>
#include <process.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 100
#define NAME_SIZE 20

void* SendMsg(void* arg);
void* RecvMsg(void* arg);
void ErrorHandling(const char* msg);

char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];

int main() {
    int sock;
    struct sockaddr_in serverAddr;
    pthread_t sendThread, recvThread;

    char myIp[100];
    char portStr[100];
    char inputName[NAME_SIZE];
    int port;

    printf("Input server IP: ");
    fgets(myIp, sizeof(myIp), stdin);
    myIp[strcspn(myIp, "\n")] = 0;

    printf("Input server port: ");
    fgets(portStr, sizeof(portStr), stdin);
    port = atoi(portStr);

    printf("Input your name: ");
    fgets(inputName, sizeof(inputName), stdin);
    inputName[strcspn(inputName, "\n")] = 0;

    snprintf(name, NAME_SIZE, "[%s]", inputName);

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        ErrorHandling("socket() error");

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(myIp);
    serverAddr.sin_port = htons(port);

    if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
        ErrorHandling("connect() error!");

    pthread_create(&sendThread, NULL, SendMsg, (void*)&sock);
    pthread_create(&recvThread, NULL, RecvMsg, (void*)&sock);

    pthread_join(sendThread, NULL);
    pthread_join(recvThread, NULL);

    close(sock);
    return 0;
}

void* SendMsg(void* arg) {
    int sock = *((int*)arg);
    char nameMsg[NAME_SIZE + BUF_SIZE];
    while (1) {
        fgets(msg, BUF_SIZE, stdin);
        if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n")) {
            close(sock);
            exit(0);
        }
        snprintf(nameMsg, sizeof(nameMsg), "%s %s", name, msg);
        write(sock, nameMsg, strlen(nameMsg));
    }
    return NULL;
}

void* RecvMsg(void* arg) {
    int sock = *((int*)arg);
    char nameMsg[NAME_SIZE + BUF_SIZE];
    int strLen;
    while ((strLen = read(sock, nameMsg, sizeof(nameMsg) - 1)) > 0) {
        nameMsg[strLen] = 0;
        fputs(nameMsg, stdout);
    }
    return NULL;
}

void ErrorHandling(const char* msg) {
    perror(msg);
    exit(1);
}

// 윈도우 환경 코드
/* 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <process.h>

#define BUF_SIZE 100
#define NAME_SIZE 20

unsigned WINAPI SendMsg(void* arg);//������ �����Լ�
unsigned WINAPI RecvMsg(void* arg);//������ �����Լ�
void ErrorHandling(char* msg);

char name[NAME_SIZE]="[DEFAULT]";
char msg[BUF_SIZE];

int main(){
	WSADATA wsaData;
	SOCKET sock;
	SOCKADDR_IN serverAddr;
	HANDLE sendThread,recvThread;

	char myIp[100];
	char port[100];
	char inputName[100];
	
	// if(argc!=4){
		// printf("Usage : %s <IP> <port> <name>\n",argv[0]);
		// exit(1);
	// }
	
	printf("Input server IP : ");
	gets(myIp);

	printf("Input server port : ");
	gets(port);

	printf("Input your name : ");
	gets(inputName);

	if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0)// ������ ������ ����Ѵٰ� �ü���� �˸�
		ErrorHandling("WSAStartup() error!");

	sprintf(name,"[%s]",inputName);
	sock=socket(PF_INET,SOCK_STREAM,0);//������ �ϳ� �����Ѵ�.

	memset(&serverAddr,0,sizeof(serverAddr));
	serverAddr.sin_family=AF_INET;
	serverAddr.sin_addr.s_addr=inet_addr(myIp);
	serverAddr.sin_port=htons(atoi(port));

	if(connect(sock,(SOCKADDR*)&serverAddr,sizeof(serverAddr))==SOCKET_ERROR)//������ �����Ѵ�.
		ErrorHandling("connect() error");

	//���ӿ� �����ϸ� �� �� �Ʒ��� ����ȴ�.

	sendThread=(HANDLE)_beginthreadex(NULL,0,SendMsg,(void*)&sock,0,NULL);//�޽��� ���ۿ� �����尡 ����ȴ�.
	recvThread=(HANDLE)_beginthreadex(NULL,0,RecvMsg,(void*)&sock,0,NULL);//�޽��� ���ſ� �����尡 ����ȴ�.

	WaitForSingleObject(sendThread,INFINITE);//���ۿ� �����尡 �����ɶ����� ��ٸ���./
	WaitForSingleObject(recvThread,INFINITE);//���ſ� �����尡 �����ɶ����� ��ٸ���.
	//Ŭ���̾�Ʈ�� ���Ḧ �õ��Ѵٸ� ���� �Ʒ��� ����ȴ�.
	closesocket(sock);//������ �����Ѵ�.
	WSACleanup();//������ ���� ��������� �ü���� �˸���.
	return 0;
}

unsigned WINAPI SendMsg(void* arg){//���ۿ� �������Լ�
	SOCKET sock=*((SOCKET*)arg);//������ ������ �����Ѵ�.
	char nameMsg[NAME_SIZE+BUF_SIZE];
	while(1){//�ݺ�
		fgets(msg,BUF_SIZE,stdin);//�Է��� �޴´�.
		if(!strcmp(msg,"q\n")){//q�� �Է��ϸ� �����Ѵ�.
			send(sock,"q",1,0);//nameMsg�� �������� �����Ѵ�.
		}
		sprintf(nameMsg,"%s %s",name,msg);//nameMsg�� �޽����� �����Ѵ�.
		send(sock,nameMsg,strlen(nameMsg),0);//nameMsg�� �������� �����Ѵ�.
	}
	return 0;
}

unsigned WINAPI RecvMsg(void* arg){
	SOCKET sock=*((SOCKET*)arg);//������ ������ �����Ѵ�.
	char nameMsg[NAME_SIZE+BUF_SIZE];
	int strLen;
	while(1){//�ݺ�
		strLen=recv(sock,nameMsg,NAME_SIZE+BUF_SIZE-1,0);//�����κ��� �޽����� �����Ѵ�.
		if(strLen==-1)
			return -1;
		nameMsg[strLen]=0;//���ڿ��� ���� �˸��� ���� ����
		if(!strcmp(nameMsg,"q")){
			printf("left the chat\n");
			closesocket(sock);
			exit(0);
		}
		fputs(nameMsg,stdout);//�ڽ��� �ֿܼ� ���� �޽����� ����Ѵ�.
	}
	return 0;
}

void ErrorHandling(char* msg){
	fputs(msg,stderr);
	fputc('\n',stderr);
	exit(1);
}

 */