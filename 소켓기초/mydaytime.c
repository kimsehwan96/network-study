// mydaytime.c
// daytime 서비스를 요청하는 tcp 클라이언트.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 127

int main(int argc, char *argv[]) {
    int s, nbyte;
    //소켓 주소 구조체 선언 
    struct sockaddr_in servaddr;
    char buf [MAXLINE + 1];

    if (argc != 2) { //인자개수가 2가 아니면 (명령어 인자 이케 들어와야함)
        printf("Usage : %s ip_address\n", argv[0]);
        exit(0);
    }
    
    if((s = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket");
        exit(1);
    }

    bzero((char *)&servaddr, sizeof(servaddr)); // 서버의 소켓 구조체 servaddr을 '\0'으로 초기화
    //servaddr의 주소 지정
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    servaddr.sin_port = htons(13);

    if(connect(s, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
        perror("connect");
        exit(1);
    }
    if((nbyte=read(s, buf, MAXLINE)) < 0) {
        perror("read fail");
        exit(1);
    }
    buf[nbyte] = 0;
    printf("%s", buf);
    close(s);
    return 0;
}