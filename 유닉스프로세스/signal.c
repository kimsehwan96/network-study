#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig) {
    if(sig==SIGALRM){
        puts("Time out !");
    }
    alarm(2);
}
//signal의 두번째 인자로 함수 포인터가 들어와야 하며, 반환형은 void, 인자는 Int 이다.
//timeout을 singal에 등록해주기만 하면, timeout()이 아닌, timeout은 함수의 주소를 의미(포인터) 이기 때문에 등록 가능.

void keycontrol(int sig){
    if(sig==SIGINT){
        puts("CTRL + C pressed");
    }
}
    
int main(int argc, char *argv[]) {
    int i;
    signal(SIGALRM, timeout);
    signal(SIGINT, keycontrol);
    alarm(2);
    
    for(i = 0; i < 3; i++){
        puts("wait..");
        sleep(100); //100sec의 sleep을 걸었지만, signal에 등록한 함수중 하나가, 2초 후 alarm을 발생하는 함수이다(timeout)
        // 시그널이 발생하면 블로킹 상태에 있던 프로세스가 깨어난다는 의미다 !
    }
    return 0;
    }

