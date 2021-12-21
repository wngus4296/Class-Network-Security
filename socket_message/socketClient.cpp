#include "unp.h"
#include <cstring>

int main(int argc, char* argv[])
{
    int sockfd, n;    // sockfd 소켓 디스크립터 (번호표), n은 시간 정보의 크기 (바이트 단위)
    char recvline[MAXLINE + 1]; // 서버로부터 받을 시간 정보를 저장할 버퍼

    char buff[MAXLINE];   // server에 보낼 내용을 저장하는 buffer
    char message[20]; // client에서 보낼 메시지

    struct sockaddr_in servaddr; // 서버의 주소를 저장한 인터넷 소켓 주소 구조체
    // 서버의 IP 주소가 인자로 주어졌는지를 체크
    if (argc != 2)
        printf("usage: %s <IPaddress>", argv[0]);
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);//Windows Sockets API Version 2.2
#endif
    // TCP 소켓을 생성해 socket descriptor를 받아와서 sockfd에 저장
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        printf("socket error");
    // 서버 주소에 대한 구조체 servaddr에 주소를 설정함
    bzero(&servaddr, sizeof(servaddr)); // 0으로 채움
    servaddr.sin_family = AF_INET; // Internet Protocol Suite를 사용
    servaddr.sin_port = htons(1007); /* message change server port number */
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
        printf("inet_pton error for %s", argv[1]); // (argv[1] 문자열)에 저장된 dotted 
        // decimal 형식의 서버 주소를 32비트 이진수로 변환
        // servaddr에 저장된 주소로 연결을 시도함
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) < 0)
        printf("connect error");

    strcpy_s(message, "hello server\0");
    snprintf(buff, sizeof(buff), "%.24s\r\n", message);
    // 수립된 연결에 대한 socket descriptor인 sockfd에 메시지 기록

    if (send(sockfd, buff, strlen(buff), 0) != strlen(buff)) // server에게 message 보냄
        printf("send error");

    // 연결된 소켓을 통해 서버가 보낸 바이트 열 (즉, 읽을 문장)이 있으면
    // 굳이 while문 안써도 됨! 한 번만 받을 것이기 때문
    while ((n = recv(sockfd, recvline, MAXLINE, 0)) > 0) { // 이를 recvline에 저장함
        recvline[n] = 0; // buffer overflow를 막기 위해 마지막 글자는 널 캐릭터로 바꿈
        if (fputs(recvline, stdout) == EOF) // stdout에 recvline을 밀어넣음
            printf("fputs error"); // 코드 구조 상 fputs의 결과가 EOF가 나올 수 없음
    }
    if (n < 0) // 정상적이었다면 n == 0이어야 하며, n < 0이면 while 루프에 문제가 있었음
        printf("read error");
#ifdef _WIN32
    WSACleanup(); // Winsock 사용 후 정리
#endif
    exit(0);
}
