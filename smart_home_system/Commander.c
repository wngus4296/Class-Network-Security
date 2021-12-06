// 현재 설정 확인 및 새로운 설정 적용하는 client

#pragma comment (lib, "ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MESSAGE_BUF 1024
#pragma warning(disable : 4996)

int main(int argc, char* argv[]) {

	WSADATA wsadate;
	SOCKADDR_IN serv_addr;
	SOCKET client;
	char message[MESSAGE_BUF] = { 0 , };
	char recvMessage[MESSAGE_BUF] = { 0, };

	if (argc != 3) printf("usage: %s <IPaddress>", argv[0]);

	if (WSAStartup(MAKEWORD(2, 2), &wsadate) != 0)  printf("socket error");
	client = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (client == INVALID_SOCKET) printf("socket error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if (connect(client, (SOCKADDR*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
		printf("connect error");
		return 0;
	}

	while (1)
	{
		fputs("\ncommander input : ", stdout);
		fgets(message, MESSAGE_BUF, stdin);

		if (send(client, message, strlen(message), 0) != strlen(message))
			printf("send error\n");

		int e = recv(client, recvMessage, 100, 0);
		recvMessage[e] = 0;
		fputs(recvMessage, stdout);
		printf("\n");
	}

	closesocket(client);
	WSACleanup();

	return 0;
}