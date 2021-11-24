#define _CRT_SECURE_NO_WARNINGS
#include "unp.h"
#include <time.h>

int main(int argc, char* argv[]) {
    int listenfd, connfd; // client를 기다리는 listenfd와, 연결 수립 후 사용하는 connfd
    char buff[MAXLINE];   // client에 보낼 내용을 저장하는 buffer
    struct sockaddr_in servaddr; // server가 기다리려는 client의 주소 범위를 저장함
    time_t ticks;
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
    // TCP 소켓을 생성해 socket descriptor를 받아와서 sockfd에 저장
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        printf("socket error");
    // 서버 주소에 대한 구조체 servaddr에 주소를 설정함
    bzero(&servaddr, sizeof(servaddr));           // 0으로 채움
    servaddr.sin_family = AF_INET;                // Internet Protocol Suite를 사용
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // 0으로 채우는 것과 동일함
    servaddr.sin_port = htons(13);                /* daytime server port number */
    // servaddr에 저장된 주소를 소켓에 bind함
    if (bind(listenfd, (SA*)&servaddr, sizeof(servaddr)) < 0)
        printf("bind error");
    // 공간을 줄이기 위해 socket sockaddr는 SA로 치환함. connect는 SA* 타입을 받아야 하는데   
    // &servaddr는 struct sockaddr_in* 타입으로 다른 상황이므로, 타입 캐스팅이 필요함
        // bind한 소켓을 통해 client의 접속을 대기 (listen)함
    int backlog = LISTENQ; // listen 함수가 리턴될 때까지 받을 수 있는 클라이언트의 최대 수
    if (listen(listenfd, backlog) < 0)
        printf("listen error");
    for (;;) {
    again:
        if ((connfd = accept(listenfd, (SA*)NULL, NULL)) < 0)
            if (errno == EPROTO || errno == ECONNABORTED) /* SW 문제로 연결 중단 */
                goto again;
            else
                printf("accept error");
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        // 수립된 연결에 대한 socket descriptor인 connfd에 저장된 시간 문자열을 기록

        if (send(connfd, buff, strlen(buff), 0) != strlen(buff))
            printf("send error");
        if (closesocket(connfd) == SOCKET_ERROR)
            printf("close error");

    }
#ifdef _WIN32
    WSACleanup();
#endif
    exit(0);
}
