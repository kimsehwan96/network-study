#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    int status;
    pid_t pid = fork();
    
    if (pid==0){
        return 3; //이 자식 프로세스는 메인 함수 내에서 return으로 종료된다.
    } else {
        printf("child PID %d\n", pid);
        pid = fork();
        if (pid==0) {
            exit(7); // 이 자식 플세스는 exir(7)을 통해 exit함수 호출을 해 종료된다.
        } else {
            printf("child PID %d\n", pid);
            wait(&status);
            if(WIFEXITED(status)) {
                printf("child send one : %d \n", WEXITSTATUS((status)));
            }
            wait(&status);
            if(WIFEXITED(status)){
                printf("child send two : %d \n", WEXITSTATUS((status)));
            }
            sleep(30); //sleep 30 sec
        }
    }
    return 0;
}
