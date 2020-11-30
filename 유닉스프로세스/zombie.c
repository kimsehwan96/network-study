#include <stdio.h>
#include <unistd.h>

int main (int argc, char *argv[]){
    pid_t pid = fork(); //자식 프로세스 생성

    if (pid==0){//자식프로세스면
        puts("자식프로세스");
    }
    else {
        printf("자식프로세스의 ID : %d \n", pid);
        sleep(30); //30초 대기
    }

    if (pid==0){
        puts("End child process");
    }
    else {
        puts("End parent process");
    }
    return 0;
}