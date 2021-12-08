#define _CRT_SECURE_NO_WARNINGS
#pragma comment (lib, "ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER 1024 

void error_message(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(-1);
}

int refresh_temp(int curr_temp) {
    return curr_temp + rand() % 3 - 1;
}

boolean check_issafe(int curr_temp, int upper, int lower) {
    if (curr_temp <= upper && curr_temp >= lower)
        return 1;
}

int newTemp(char* message, int isminus) {
    char* sArr[5] = { NULL, };
    int newIndex = 0;
    char* ptr = isminus == 0? strtok(message, " ") : strtok(message, "-");
    while (ptr != NULL)
    {
        sArr[newIndex] = ptr;
        newIndex++;       

        ptr = strtok(NULL, " ");
    }

    return isminus == 0 ? atoi(sArr[2]) : atoi(sArr[1]);
}

int main(int argc, char* argv[])
{
    WSADATA wsadata;
    SOCKET server_sock, client_sock;
    SOCKADDR_IN server_addr, client_addr;
    char memory_buf[BUFFER] = { 0, };
    char returnMessage_buf[BUFFER] = { 0, };
    int client_addr_size;
    int fdNum, count, str_len;

    const char* query = "QUERY";
    const char* c_upper = "CONFIGURE UPPER";
    const char* c_lower = "CONFIGURE LOWER";

    fd_set read, copy_read;
    TIMEVAL time;

    // 보일러의 초기 값
    int Current_temperature = 25;
    int Upper_bound = 30;
    int Lower_bound = -5;

    if (argc != 2) {
        error_message("input error");
        exit(1);
    }
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
        error_message("socket init error");
        exit(1);
    }
    server_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_sock == INVALID_SOCKET) {
        error_message("socket descriptor error");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    if (bind(server_sock, (SOCKADDR*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        error_message("socket bind error");
        exit(1);
    }
    if (listen(server_sock, 5) == SOCKET_ERROR) {
        error_message("socket listen error");
        exit(1);
    }

    FD_ZERO(&read);
    FD_SET(server_sock, &read);

    while (1)
    {
        copy_read = read;
        time.tv_sec = 5;
        time.tv_usec = 5000;
        fdNum = select(0, &copy_read, 0, 0, &time);

        if (fdNum == SOCKET_ERROR)
        {
            error_message("select error");
            break;
        }

        if (fdNum == 0)
        {
            puts("continue.....");
            continue;
        }

        for (count = 0; count < read.fd_count; count++)
        {
            if (FD_ISSET(read.fd_array[count], &copy_read))
            {
                if (read.fd_array[count] == server_sock)
                {
                    client_addr_size = sizeof(client_addr);
                    client_sock = accept(server_sock, (SOCKADDR*)&client_addr, &client_addr_size);
                    FD_SET(client_sock, &read);
                    printf("%d : socket is connected\n", client_sock);
                }
                else
                {
                    str_len = recv(read.fd_array[count], memory_buf, BUFFER - 1, 0);
                    memory_buf[str_len] = 0;
                    if (str_len == 0)
                    {
                        FD_CLR(read.fd_array[count], &read);
                        closesocket(copy_read.fd_array[count]);
                        printf("%d : socket is disconnected\n", copy_read.fd_array[count]);
                    }
                    else
                    {
                        if (strncmp(memory_buf, query, 5) == 0) {
                            sprintf(returnMessage_buf, "Current temperature = %d\nUpper bound = %d\nLower bound = %d\n",
                                Current_temperature, Upper_bound, Lower_bound);
                        }
                        else if (strncmp(memory_buf, c_upper, 15) == 0) {
                            int isminus = 0;
                            if (memory_buf[22] == "-") {
                                isminus = 1;
                            }
                            Upper_bound = newTemp(&memory_buf, isminus);
                            sprintf(returnMessage_buf, "Current temperature = %d\nChanged Upper bound = %d\nLower bound = %d\n",
                                Current_temperature, Upper_bound, Lower_bound);
                        }
                        else if (strncmp(memory_buf, c_lower, 15) == 0) {
                            int isminus = 0;
                            if (memory_buf[22] == "-") {
                                isminus = 1;
                            }
                            Lower_bound = newTemp(&memory_buf, isminus);
                            sprintf(returnMessage_buf, "Current temperature = %d\nUpper bound = %d\nChanged Lower bound = %d\n",
                                Current_temperature, Upper_bound, Lower_bound);
                        }
                        else { // polling message
                            Current_temperature = refresh_temp(Current_temperature);
                            if (check_issafe(Current_temperature, Upper_bound, Lower_bound) == 1)
                                sprintf(returnMessage_buf, "safe");
                            else
                                sprintf(returnMessage_buf, "warning");
                        }
                        send(read.fd_array[count], returnMessage_buf, strlen(returnMessage_buf), 0);
                    }
                }
            }
        }
    }
    closesocket(server_sock);
    WSACleanup();

    return 0;
}