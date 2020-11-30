#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

void timeout(int sig) {
    if(sig==SIGALRM){
        puts("Time out !");
    }
    alarm(2);
}

void keycontrol(int sig){
    if(sig==SIGINT){
        puts("CTRL + C pressed");
    }
}
    
int main(int argc, char *argv[]) {
    int i;
    struct sigaction *sigato = (struct sigaction *)malloc(sizeof(struct sigaction)); //sigaction 함수의 핸들러 함수를 등록하기 위해 sigaction 구조체의 포인터 변수 선언
    struct sigaction *sigakc = (struct sigaction *)malloc(sizeof(struct sigaction)); //sigaction 함수의 핸들러 함수를 등록하기 위해 sigaction 구조체의 포인터 변수 선언
    sigato->sa_handler = timeout; //각 구조체 변수의 멤버 변수인 sa_handler 에 각 핸들러 함수 등록
    sigakc->sa_handler = keycontrol;
    memset(&(sigato->sa_mask),0, sizeof(sigato->sa_mask)); //sigemptyset 이라는 함수도 있지만, memset을 이용해 sa_mask의 모든 바이트을 0으로 초기화
    memset(&(sigakc->sa_mask),0, sizeof(sigato->sa_mask));
    sigato->sa_flags = 0; //sa_flags는 우선 0으로 셋
    sigakc->sa_flags = 0;
    sigaction(SIGALRM, sigato, 0); //sigaction 이라는 함수를 이용해 sigato 구조체 포인터 변수(이 안에 핸들러 등록되어있음)를 등록해 핸들러 선언
    sigaction(SIGINT, sigakc, 0);
    alarm(2);
    
    for(i = 0; i < 3; i++){
        puts("wait..");
        sleep(100);
    }
    return 0;
    }

