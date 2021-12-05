// 주기적으로 POLLING 메시지 전달하여 실시간으로 현재 상태 확인 가능

#include "unp.h"
#include <time.h>

#define MESSAGE_BUF 1024
#pragma warning(disable : 4996)

int main(int argc, char* argv[]) {

	WSADATA wsadate;
	SOCKADDR_IN serv_addr;
	SOCKET client;
	int str_len = 1;
	time_t ticks;
	char message[MESSAGE_BUF] = { 0 , };

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
		ticks = time(NULL);
		snprintf(message, sizeof(client), "%.24s\r\n", ctime(&ticks));
		str_len = send(client, message, strlen(message) + 1, 0);
		int e = recv(client, message, str_len, 0);
		message[e] = 0;
		fputs(message, stdout);

		Sleep(2000);
	}

	closesocket(client);
	WSACleanup();

	return 0;
}