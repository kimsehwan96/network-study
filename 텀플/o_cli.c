/*
한국산업기술대학교 임베디드 시스템과 2015146007 김세환
임베디드 운영체제 과목 P2P Server Client Implementaion Source Code
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h> //open, close 등 파일 디스크립터 사용을 위함
#include <errno.h>

#define SERV_IP "127.0.0.1"
#define SERV_PORT 4140
#define INIT_STATE 0
#define AFTER_STATE 1
#define BUFSIZE 512

extern int errno;

int main(void)
{
    int sockfd, fd;
    int rcv_byte, file_size;
    int token = 0;
    struct sockaddr_in dest_addr;
    char *buf = (char *)malloc(BUFSIZE);
    char file_name[512];
    char id[20];
    char pw[20];

    sockfd = socket(AF_INET, SOCK_STREAM, 0); //socket fd

    if (sockfd == -1)
    {
        perror("socket");
        exit(1);
    }
    else
    {
        printf("socket is ok \n");
    }

    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(SERV_PORT);
    dest_addr.sin_addr.s_addr = inet_addr(SERV_IP);
    memset(&(dest_addr.sin_zero), 0, 8);
    if (connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("connect");
        exit(1);
    }
    else
    {
        printf("connect ok\n");
    }
    //클라이언트 메인 로직
    close(sockfd);
    free(buf);
    return 0;
}