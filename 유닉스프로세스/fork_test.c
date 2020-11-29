#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include  <stdlib.h>
int global_var = 0;

int main(void){
    pid_t pid;
    int local_var = 0;
    if( (pid = fork()) < 0){
        printf("fork error");
        exit(0);
    } else if (pid == 0){
        //자식 프로세스
        global_var++;
        local_var++;
        printf("CHILD - my pid is %d and parent's pid %d \n", getpid(), getppid());
    }
    else {
        // 부모 프로세스
        sleep(2);
        global_var += 5;
        local_var +=5;
        printf("PARENT - my pid is %d and childs's pid %d \n", getpid(), pid);
    }
    printf("\t global var : %d\n", global_var);
    printf("\t local var : %d\n", local_var);
    return 0;
}