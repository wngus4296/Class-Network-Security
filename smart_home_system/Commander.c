// 현재 설정 확인 및 새로운 설정 적용하는 client

#include "unp.h"

#define MESSAGE_BUF 1024
#pragma warning(disable : 4996)

int main(int argc, char* argv[]) {

	WSADATA wsadate;
	SOCKADDR_IN serv_addr;
	SOCKET client;
	// int str_len = 1;
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

	if (connect(client, (SOCKADDR*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)	printf("connect error");

	while (1)
	{
		fputs("\ncommander input : ", stdout);
		fgets(message, MESSAGE_BUF, stdin);

		send(client, message, strlen(message) + 1, 0);

		int e = recv(client, recvMessage, 100, 0); // 버퍼 크기 신경쓰기
		recvMessage[e] = 0;
		fputs(recvMessage, stdout);
		printf("\n");
	}

	closesocket(client);
	WSACleanup();

	return 0;
}