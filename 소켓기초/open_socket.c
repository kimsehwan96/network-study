// open_socket.c
// socket() 시스템콜 호출 및 생성된 소켓번호 출력

#include <stdio.h> //표준 라이브러리
#include <stdlib.h> //표준 라이브러리
#include <sys/types.h> // 시스템 콜에 필요한 상수 선언
#include <sys/stat.h> // 파일의 상태에 대한 데이터 선언
#include <sys/socket.h> //소켓 시스템콜 선언
#include <fcntl.h> // open에 필요한 flag 선언
#include <unistd.h>

int main(void){
    int fd1, fd2, sd1, sd2; //파일 디스크립터 총 4개, 파일 2개, 소켓 2개 테스트
    //파일 열어보기
    fd1 = open("file1", O_RDONLY, 0);
    printf("file1's fd %d\n", fd1);
    //tcp 소켓 열기
    sd1 = socket(AF_INET, SOCK_STREAM, 0);
    printf("stream socket descriptor %d\n", sd1);

    //UDP 소켓 열기

    sd2 = socket(AF_INET, SOCK_DGRAM, 0);
    printf("datagram socket descriptor %d\n", sd2);

    //또다른 파일 열기
    fd2 = open("file2", O_RDONLY, 0);
    printf("file2's fd %d\n", fd2);

    close(fd1);
    close(fd2);
    close(sd1);
    close(sd2);

    return 0;

}