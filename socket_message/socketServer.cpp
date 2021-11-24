#define _CRT_SECURE_NO_WARNINGS
#include "unp.h"
#include <cstring>
#include <windows.h> // Sleep �Լ� ����� ���� ���

int main(int argc, char* argv[]) {
    int n;    // n�� message ������ ũ�� (����Ʈ ����)
    char recvline[MAXLINE + 1]; // client�� ���� ���� message�� ������ ����

    int listenfd, connfd; // client�� ��ٸ��� listenfd��, ���� ���� �� ����ϴ� connfd
    char buff[MAXLINE];   // client�� ���� ������ �����ϴ� buffer
    char message[20]; // �������� ���� �޽���

    struct sockaddr_in servaddr; // server�� ��ٸ����� client�� �ּ� ������ ������
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
    // TCP ������ ������ socket descriptor�� �޾ƿͼ� listenfd�� ����
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        printf("socket error");
    // ���� �ּҿ� ���� ����ü servaddr�� �ּҸ� ������
    bzero(&servaddr, sizeof(servaddr));           // 0���� ä��
    servaddr.sin_family = AF_INET;                // Internet Protocol Suite�� ���
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // 0���� ä��� �Ͱ� ������
    servaddr.sin_port = htons(1007);                /* message change server port number */
    // servaddr�� ����� �ּҸ� ���Ͽ� bind��
    if (bind(listenfd, (SA*)&servaddr, sizeof(servaddr)) < 0)
        printf("bind error");
    // ������ ���̱� ���� socket sockaddr�� SA�� ġȯ��. connect�� SA* Ÿ���� �޾ƾ� �ϴµ�   
    // &servaddr�� struct sockaddr_in* Ÿ������ �ٸ� ��Ȳ�̹Ƿ�, Ÿ�� ĳ������ �ʿ���
        // bind�� ������ ���� client�� ������ ��� (listen)��
    int backlog = LISTENQ; // listen �Լ��� ���ϵ� ������ ���� �� �ִ� Ŭ���̾�Ʈ�� �ִ� ��
    if (listen(listenfd, backlog) < 0)
        printf("listen error");
    for (;;) {
    again:
        if ((connfd = accept(listenfd, (SA*)NULL, NULL)) < 0)
            if (errno == EPROTO || errno == ECONNABORTED) /* SW ������ ���� �ߴ� */
                goto again;
            else
                printf("accept error");

        while ((n = recv(connfd, recvline, MAXLINE, 0)) > 0) { // �̸� recvline�� ������
            recvline[n] = 0; // buffer overflow�� ���� ���� ������ ���ڴ� �� ĳ���ͷ� �ٲ�
            if (fputs(recvline, stdout) == EOF) // stdout�� recvline�� �о����
                printf("fputs error"); // �ڵ� ���� �� fputs�� ����� EOF�� ���� �� ����
            
            Sleep(3000); // client���� message�� �ް� ���� 3�� �� hello client����ϱ� ���� ��ٸ���.
            // Sleep�� ���� �ʿ� ������ ���� ���� Ȯ���� ���� �����.
            strcpy(message, "hello client\0");
            snprintf(buff, sizeof(buff), "%.24s\r\n", message);
            // ������ ���ῡ ���� socket descriptor�� connfd�� �޽��� ���

            if (send(connfd, buff, strlen(buff), 0) != strlen(buff))
                printf("send error");
            else
                break; // message�� ���������� ���´ٸ� ������ socket�� �ݴ´�.
        }
        if (n < 0) // �������̾��ٸ� n == 0�̾�� �ϸ�, n < 0�̸� while ������ ������ �־���
            printf("read error");

        if (closesocket(connfd) == SOCKET_ERROR)
            printf("close error");
    }
#ifdef _WIN32
    WSACleanup();
#endif
    exit(0);
}