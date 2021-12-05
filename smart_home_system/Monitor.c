#include "unp.h"
#include <windows.h>

int main(int argc, char* argv[]) {
	int sockfd, n;
	char state[MAXLINE + 1];

	char buff[MAXLINE];
	char message[] = "POLLING\0";

    struct sockaddr_in servaddr; 
    if (argc != 2)
        printf("usage: %s <IPaddress>", argv[0]);
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        printf("socket error");
    FD_ZERO(&servaddr);
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(1007); 
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
        printf("inet_pton error for %s", argv[1]);

    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) < 0)
        printf("connect error");

    while(1) {
        snprintf(buff, sizeof(buff), "%s\n", message);

        if (send(sockfd, buff, strlen(buff), 0) != strlen(buff)) 
            printf("send error");
        
        while ((n = recv(sockfd, state, MAXLINE, 0)) > 0) { 
            state[n] = 0; 
            if (fputs(state, stdout) == EOF) 
                printf("fputs error");
        }
        if (n < 0) 
            printf("read error");

        Sleep(2000); 
    }

#ifdef _WIN32
    WSACleanup(); 
#endif
    exit(0);
}