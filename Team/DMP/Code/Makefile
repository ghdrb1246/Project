CC = gcc
CFLAGS = -I. -I./Server -I./Client
LDFLAGS = -lsqlite3
CODE_SRC = UserInfo.c

# 클라이언트 빌드
client: $(CODE_SRC) Client/MainClient.c Client/MenuIO.c Client/ClientController.c
	$(CC) $(CFLAGS) -o client_app Client/MainClient.c Client/MenuIO.c Client/ClientController.c $(CODE_SRC)

# 서버 빌드
server: $(CODE_SRC) Server/MainServer.c Server/DBM.c
	$(CC) $(CFLAGS) -o server_app Server/MainServer.c Server/DBM.c $(CODE_SRC) $(LDFLAGS)

# clean
clean:
	rm -f client server