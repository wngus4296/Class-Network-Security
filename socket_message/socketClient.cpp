#include "unp.h"
#include <cstring>

int main(int argc, char* argv[])
{
    int sockfd, n;    // sockfd ���� ��ũ���� (��ȣǥ), n�� �ð� ������ ũ�� (����Ʈ ����)
    char recvline[MAXLINE + 1]; // �����κ��� ���� �ð� ������ ������ ����

    char buff[MAXLINE];   // server�� ���� ������ �����ϴ� buffer
    char message[20]; // client���� ���� �޽���

    struct sockaddr_in servaddr; // ������ �ּҸ� ������ ���ͳ� ���� �ּ� ����ü
    // ������ IP �ּҰ� ���ڷ� �־��������� üũ
    if (argc != 2)
        printf("usage: %s <IPaddress>", argv[0]);
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);//Windows Sockets API Version 2.2
#endif
    // TCP ������ ������ socket descriptor�� �޾ƿͼ� sockfd�� ����
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        printf("socket error");
    // ���� �ּҿ� ���� ����ü servaddr�� �ּҸ� ������
    bzero(&servaddr, sizeof(servaddr)); // 0���� ä��
    servaddr.sin_family = AF_INET; // Internet Protocol Suite�� ���
    servaddr.sin_port = htons(1007); /* message change server port number */
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
        printf("inet_pton error for %s", argv[1]); // (argv[1] ���ڿ�)�� ����� dotted 
        // decimal ������ ���� �ּҸ� 32��Ʈ �������� ��ȯ
        // servaddr�� ����� �ּҷ� ������ �õ���
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) < 0)
        printf("connect error");

    strcpy_s(message, "hello server\0");
    snprintf(buff, sizeof(buff), "%.24s\r\n", message);
    // ������ ���ῡ ���� socket descriptor�� sockfd�� �޽��� ���

    if (send(sockfd, buff, strlen(buff), 0) != strlen(buff)) // server���� message ����
        printf("send error");

    // ����� ������ ���� ������ ���� ����Ʈ �� (��, ���� ����)�� ������
    while ((n = recv(sockfd, recvline, MAXLINE, 0)) > 0) { // �̸� recvline�� ������
        recvline[n] = 0; // buffer overflow�� ���� ���� ������ ���ڴ� �� ĳ���ͷ� �ٲ�
        if (fputs(recvline, stdout) == EOF) // stdout�� recvline�� �о����
            printf("fputs error"); // �ڵ� ���� �� fputs�� ����� EOF�� ���� �� ����
    }
    if (n < 0) // �������̾��ٸ� n == 0�̾�� �ϸ�, n < 0�̸� while ������ ������ �־���
        printf("read error");
#ifdef _WIN32
    WSACleanup(); // Winsock ��� �� ����
#endif
    exit(0);
}