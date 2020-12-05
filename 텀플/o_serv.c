/*
한국산업기술대학교 임베디드 시스템과 2015146007 김세환
임베디드 운영체제 과목 P2P Server Client Implementaion Source Code
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>  //for posix thread
#include <signal.h>   //for signal handler.
#include <fcntl.h>    //for file handling
#include <sys/stat.h> //for file status
#include <dirent.h>
#include <errno.h>

//custom
#include "common.h"
//macro
#define SERV_IP "127.0.0.1" // 서버의 로컬 호스트 주소를 define
#define SERV_PORT 4140      //서버의 포트 번호를 define
#define BACKLOG 10
#define INIT_STATE 0
#define AFTER_STATE 1
#define BUFSIZE 512


extern int errno;

int main()
{
    int sockfd, new_fd; //server 호스트의 소켓 파일디스크립터 및 새로운 연결을 정의할 new_fd
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    unsigned int sin_size;
    //서버 코드가 돌아가는 디렉터리의 하위 폴더인 data 밑에 저장함

    //for server concurrency, we will fork server process with each connection request.
    pid_t childpid;
    int rcv_byte;
    char *buf = (char *)malloc(BUFSIZE);
    char *tmp_file_name = (char *)malloc(BUFSIZE);
    char id[20];
    char pw[20];
    int val = 1;

    //socket TCP file descirptor
    //check sockfd condition
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("serverf-socket() error occured");
        exit(1);
    }
    else
    {
        printf("server socket() sockfd is ok \n");
    }

    my_addr.sin_family = AF_INET; //address family is AF_INET

    my_addr.sin_port = htons(SERV_PORT);

    my_addr.sin_addr.s_addr = INADDR_ANY; //any address can accept

    memset(&(my_addr.sin_zero), 0, 8);

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&val, sizeof(val)) < 0)
    {
        perror("setsockopt");
        close(sockfd);
        return -1;
    }

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == 1)
    {
        perror("bind error ");
        exit(1);
    }
    else
    {
        printf("server bind ok \n");
    }
    if (listen(sockfd, BACKLOG) == -1)
    {
        perror("Listen error");
        exit(1);
    }
    else
    {
        printf("listen ok \n\n");
    }

    memset(id, 0, sizeof(id));
    memset(pw, 0, sizeof(pw));

    sin_size = sizeof(struct sockaddr_in);

    for (;;)
    {
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);

        if (new_fd < 0)
        {
            perror("bind error ");
            exit(1);
        }
        printf("Connection accept from %s\n", inet_ntoa(their_addr.sin_addr));

        if ((childpid = fork()) == 0)
        {                  //child process
            close(sockfd); 
            for (;;)
            {
                //새로운 클라이언트가 들어왔을때의 로직.
            }
        }
    }
    close(new_fd);
    close(sockfd);
}
