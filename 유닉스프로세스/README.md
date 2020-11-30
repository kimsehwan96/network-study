# 유닉스 프로세스
- 네트워크 프로그램에서는 여러 작업을 동시에 처리해야 할 경우가 많다.
- 이를 위해 멀티태스킹 프로그래밍이 자주 사용된다.
    - 멀티 스레드
    - 멀티 프로세스
- 두가지 방법이 있다.
- 멀티 프로세스 프로그래밍을 먼저 해보고, 멀티 스레드는 추후해 해본다.

## 프로세스의 이해
- 프로세스란 실행중인 프로그램을 의미한다. 프로그램이란 소스코드를 말하며, 프로세스는 이러한 프로그램이 컴퓨터에서 실행되고 있는것을 말한다.
- 프로세스는 사용자 모드 또는 커널 모드 중 하나에서 실행된다.
- i++ 같은 일반 연산은 사용자 모드에서 실행된다. read()함수를 호출하면 read()는 커널에게 동작을 요청하고 커널의 권한으로 실행된다.

#### 프로세스의 상태

- 프로세스는 다음과 같은 상태를 가진다
    - 실행 상태
    - 블록 상태
    - 중단 상태
    - 좀비 상태

- 실행상태 : 프로세스가 CPU 서비스를 받을 수 있는 상태로서 사용자 모드 혹은 커널 모드에서 실행된다
- 블록상태 : 블록 상태란 입출력 처리나 어떤 조건을 기다리는 상태
- 중단상태 : 프로세스 실행이 잠시 중단된 상태로 예를들어 시그널을 받아 프로세스의 동작이 정지된 경우등을 말함
- 좀비상태 : 프로세스의 실행은 끝났으나 아직 종료 상태를 부모 프로세스에게 보내지 못하고 있는 상태.

#### 프로세스의 메모리 배치
- 프로세스가 실행되려면 일정한 메모리(메인메모리)를 배정받아 사용해야 함
- 메모리 영역을 `프로세스 이미지`라고 부른다,
- 프로세스 이미지에는 프로그램 실행에 필요한 어셈블러 코드와
- 각종 변수들이 저장된다.
- 원칙적으로 다른 프로세스가 다른 프로세스의 메모리에 접근 할 수는 없다. (ipc의 공유메모리는 제외겠지?)

```c
#include <stdio.h>
#include <stdlib.h>

extern char **environ; //extern 변수
int init_global_var = 3; //초기화된 전역 변수
int unint_global_var; //초기화 되지 않은 전역 변수

int main(int argc, char **argv){
    int auto_var; //자동 변수
    static int static_var; //static 변수
    register int reg_var; // register 변수
    char *auto_ptr; // 자동 변수
    auto_ptr = malloc(10); // 메모리를 10바이트 할당
    return 0;
}
```

|메모리 영역|메모리 내용 및 변수들|
|--------|----------------|
|환경변수와 명령행 인자 영역| *environ 내용|
|스택(stack)|argc, argv, auto_var, reg_var, auto_ptr|
|힙(heap)|malloc()이 할당한 10바이트|
|데이터 영역 (초기화 안된 데이터)|unint_global_var, static_var|
|데이터 영역 (초기화된 데이터)|init_global_var|
|코드 영역| 어셈블된 프로그램 코드|

- 위와 같이 프로세스 각 메모리 영역에 분리되어 저장된다.

- 환경변수란?
    - 프로세스 실행과 관련된 정보들, 예를들어 프로세스의 현재 디렉토리명, 파일 액세스 권한 정보등이 포함된다
    - 프로그램 실행시 인자로 입력한 명령인자 값(argc, argv)들도 환경변수와 같은 영역에 저장된다.

#### 스택과 힙
- C프로그램은 `main()`함수에서 시작하여 필요한 함수들을 호출하는 형태로 실행된다.
- 스택 영역에는 현재 호출되어 실행중인 함수의 코드와 환경 정보들이 저장된다.
- 따라서 스택의 첫 부분에는 `main()` 함수의 내용이 저장될 것이다.
- `main()`함수에서 `printf()`를 호출하였다면, `main()`이 차지하는 스택 영역 위에 `printf()`를 처리하기 위한 메모리가 할당되며, 수행된다
- `printf()` 함수가 리턴됨면 관련된 스택 영역은 삭제(free)되고, 프로세스의 실행은 다시 `main()`영역으로 돌아온다.

- 힙은 아래의 코드와 함께 보자

```c
int main(){
    char *ptr;
    ptr = func();
}
    char *func(){
        char arr[10];
        return arr;
    }

```

- 메인함수에서 `func()`를 호출하고, `func()`에서는 `char arr[10]`으로 10바이트의 배열을 할당한 후에 그 주소를 리턴하고 있다.
- 그런데 10바이트 크기의 배열 `arr[]`는 `func()` 함ㅁ수 내에서 임시롤 사용되는 자동 변수로서, 스택 영역에 저장되며, `func()` 함수가 리턴할 때
- 사용하고 있던 스택 영역은 지워진다. 즉 위의 코드는 에러가 발생한다.
- 위를 해결하기 위해선 `heap` 영역을 사용해야 함

```c
int main(){
    char **ptr;
    ptr = func();
    free(ptr);
}

char *func(){
    char *arr;
    arr = malloc(10);
    return arr;
}
```

#### fork()

- 유닉스에서 프로세스를 새로 만들기 위해서 `fork()`를 주로 사용한다.
- `fork()`를 호출하면, 호출한 프로세스의 이미지를 그대로 복사하여 새로운 프로세스를 하나 더 생성한다.
- 즉, 두개의 똑같은 프로세스가 생성되는 것이다.
- `fork()`를 호출한 프로세스를 부모프로세스, 새로 생성된 프로세스를 자식 프로세스라고 한다.
- 자식 프로세스의 `fork()` 리턴 값은 항상 0이고
- 부모 프로세스의 `fork()` 리턴값은 자식 프로세스의 pid 값이다.

```c
pid_t PID;
PID = fork();
if (PID == 0){
    child_work(); //자식 프로세스용 코드
}
else {
    parent_work(); //부모 프로세스용 코드
}
```

- 부모 프로세스와 자식 프로세스는 각각 별도의 메모리 영역(이미지)를 갖고 있음므로
- 모든 변수도 각각 따로 가지고 있다.
- 즉 변수를 공유하진 않는다.
- `fork()`를 호출하기 전 개설한 파일이나 소켓은 프로세스 이미지 외부에 존재하므로 공유 가능하다(디스크립터 번호로 접근 가능하다는것을 의미한다!)

```c
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
```

### exec 함수
- 프로세스가 하던 작업을 종료하고 다른 작업을 하도록 전환할 때 exec 계열의 함수를 사용한다.
- exec 함수를 호출하면 현재 실행 중이던 프로세스의 이미지를 새로운 프로세스의 이미지로 교체한다.

```c
execl("/bin/ls", "ls", "test.txt", NULL);
```

- `execl()` 함수의 첫 번째 인자는 실행시킬 함수의 경로를
- 두 번째 인자는 실행 시킬 함수 명을
- 세번째 인자는 함수 실행시 전달할 인자를
- 네번째 인자는 환경변수를 전달하는데 사용한다.
- exec함수 실행에 필요한 인자들을 list 형태로 주기위해 execl 을 사용 함.

```c
#include <unistd.h>
int execl(const char *path, const char *arg, ...);
int execv(const char *path, char *const argv[]);
int execle(const char *path, const char *arg, ..., char * const envp[]);
```

- 등등이 있다.

### 좀비 프로세스의 생성 이유
- fork로 호출로 생성된 자식 프로세스가 종료되는 상황 두가지를 예로 들면
    - 인자를 전달하면서 exit을 호출하는 경우
    - main 함수에서 return문을 실행하면서 값을 반환하는 경우
- exit 함수로 전달되는 인자 값과 main 함수의 return 문으로 반환되는 값 모두 운영체제로 전달 된다.
- 운영체제는 이 값이 자식프로세스를 생성한 부모 프로세스에게 전달 될 때 까지 자식 프로세스를 소멸시키지 않는다.
- 이 상황에 놓여있는 프로세스를 좀비 프로세스라고 한다.


```c
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
```
- 좀비 프로세스 생성 예제, 출력

```console
(base)  ~/Desktop/ingkle/netc   master ●  ps au           
USER        PID  %CPU %MEM      VSZ    RSS   TT  STAT STARTED      TIME COMMAND
gimsehwan 45868   0.7  0.0  4731720   5648 s003  Ss    5:59PM   0:00.21 /bin/zsh -l
gimsehwan 45855   0.0  0.0  4268276    756 s001  S+    5:59PM   0:00.00 ./zombie
gimsehwan 43996   0.0  0.0  4634520   6372 s001  Ss    5:53PM   0:00.64 /bin/zsh -l
gimsehwan 32905   0.0  0.0  4440580   2848 s002  S+    4:57PM   0:00.04 ssh pi@192.168.0.100
gimsehwan 26170   0.0  0.0  4757352   5308 s002  S     4:27PM   0:00.51 -zsh
root      26169   0.0  0.0  4474868   4232 s002  Ss    4:27PM   0:00.05 login -pf gimsehwan
root      45957   0.0  0.0  4399348   1488 s003  R+    5:59PM   0:00.00 ps au
gimsehwan 45856   0.0  0.0        0      0 s001  Z+    5:59PM   0:00.00 (zombie)
```

- `Z+`로 좀비프로세스 임을 확인 할 수 있다.

## 좀비 프로세스의 소멸

- 자식 프로세스의 소멸을 위해서는 부모 프로세스가 자식 프로세스의 전달 값을 요청해야 한다.
- 요청에는 두가지 방법이 있다.
    - 1. 함수 호출

```c
#include <sys/wait.h>

pid_t wait(int * statloc);
//성공시 종료된 자식 프로세스의 ID, 실패시 -1 반환
```

- 위 함수가 호출되었을 때, 이미 종료된 자식 프로세스가 있다면
- 자식 프로세스가 종료되면서 전달한 값 (exit 함수의 인자 값, main 함수의 return에 의한 반환값)이 매개변수로 전달된 주소의 변수에 저장됨
- 이 변수에 저장되는 값에는 자식 프로세스가 종료되면서 전달한 값 이와에도 다른 정보가 함께 있다.

```c
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
            exit(7); // 이 자식 프로세스는 exir(7)을 통해 exit함수 호출을 해 종료된다.
        } else {
            printf("child PID %d\n", pid); //return으로 종료되는 
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
```

```console
gcc wait.c -o wait && "/Users/gimsehwan/Desktop/ingkle/netc/유닉스프로세스/"wait
child PID 55606
child PID 55606
child PID 55607
child send one : 3 
child send two : 7 
```

- 두 자식 프로세스가 전달한 값 3과 7이 부모프로세스로 전달되었다.
- wait 함수는 호출된 시점에서 종료된 자식 프로세스가 없다면, 임의의 자식프로세스가 종료될 때 까지 블로킹 상태에 놓인다.
    - 주의해서 사용해야 한다.

### 두번째 방법 : waitpid 함수
- wait 함수의 블로킹이 문제가 된다면, waitpid 함수의 호출을 고려해보자.

```c
#include <sys/wait.h>

pid_t waitpid(pid_t pid, int * statloc, int options);
//성공시 자식프로세스의 ID 또는 0, 실패시 -1
// pid <- 종료를 확인하고자 하는 자식 프로세스의 id 전달, -1을 전달하면 임의의 자식프루세스가 종료되기를 기다림
// statloc -> wait 함수의 매개변수, statloc과 동일한 의미
// options 헤더파일 sys/wait.h에 선언된 상수 WNOHANG을 인자로 전달하면, 종료된 자식프로세스가 없더라도
// 블로킹 상태에 있지 않고, 0을 반환하면서 함수를 빠져나온다.
```

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    int status;
    pid_t pid = fork();
    
    if(pid==0){
        sleep(15); //자식 프로세스의 종료를 지연시키기 위해 15초 기다림
        return 24;
    }
    else {
        while(!waitpid(-1, &status, WNOHANG)) { //while문 안에서 waitpid 함수 호출
            sleep(3);
            puts("sleep 3sec \n");
        }
        if(WIFEXITED(status)) {
            printf("child send %d \n", WEXITSTATUS(status));
        }
    }
    return 0;
}
```

```console
sleep 3sec 

sleep 3sec 

sleep 3sec 

sleep 3sec 

sleep 3sec 

child send 24 
Program ended with exit code: 0
```

- 5회 3sec이 실행되었다. waitpid 함수가 블로킹 되지 않았음을 증명하는 결과이기도 하다.


### 시그널 핸들링
- 도대체 자식 프로세스가 언제 종료될 줄 알고 waitpid를 계속 호출하겠는가
- 부모 프로세스도 자식만큼이나 바쁘다.
- 시그널 핸들링을 이용해
    - 시그널이 발생했을 때 부모프로세스가 해당 상황에 대한 작업을 하는게 좋겠다.
    - 핸들링, 혹은 뭐 핸들러 함수라고도 많이 하는듯.

#### 시그널과 signal 함수
- 프로세스 : 야 운영체제야, 내가 만든 자식 프로세스가 종료되면 zombie_handler 함수 호출좀 해줘
- 운영체제 : 오키 그럴테니까 넌 zombie_handler 잘 만들어놔

```c
#include <signal>

void (*signal(int signo, void(*func)(int)))(int);
// 시그널 발생시 호출되도록 등록된 함수의 포인터 반환
```

- 함수 이름 signal
    - 매개변수
        - int signo, void(*func)(int)
    - 반환형
        - 매개변수형이 int이고, 반환형이 void인 함수 포인터
- 그니까 매개변수에도 함수 포인터가 있고, 리턴형도 함수포인터다.

- signo으로 정의된 몇몇 상수를 정리해보자
    - SIGALRM : alarm 함수 호출을 통해 등록된 시간이 된 상황
    - SIGINT : CTRL + C 가 입력된 상황
    - SIGCHLD : 자식 프로세스가 종료된 상황

```c
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
```

#### sigaction 함수를 이용한 시그널 핸들링

- signal은 유닉스 계열의 운영체제에서 동작 방식이 약간씩 차이가 날 수 있지만
- sigaction은 완전히 동일한 동작을 보장한다.

```c
#include <signal.h>

int sigaction(int signo, const struct sigaction * act, struct sigaction * oldact);
// 성공시 0, 실패시 -1
// signo <- 시그널의 정보를 인자로 전달
// act <- 첫 번째 인자로 전달된 상수에 해당하는 시그널 발생시, 호출될 함수(시그널 핸들러)의 정보 전달
// oldact <- 이전에 등록되었던 시그널 핸들러의 함수 포인터를 얻는데 사용되는 인자, 팔요 없다면 0을 전달
```

- 위 함수의 호출을 위해서는 sigaction 이라는 구조체 변수를 선언 및 초기화 해야 한다.

```c
struct sigaction {
    void (*sa_handler)(int); //시그널 핸들러 함수의 함수 포인터
    sigset_t sa_mask;
    int sa_flags;
}
```

- sa_handler에 시그널 핸들러의 함수 포인터를 저장하면 된다. sa_mask 및 sa_flags는 0으로 초기화.

- 이번엔 내가 이런 API 문서를 이용해 직접 구현해봤다.

```c
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
```

- 시그널 핸들러 등록 과정이 조금 뭣같지만, 예제 코드와 동일하게 동작한다!

```c
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

void read_childproc(int sig){
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    
    if(WIFEXITED(status)) {
        printf("removed proc id %d\n", id);
        printf("child send : %d \n", WEXITSTATUS(status));
    }
}

int main (void){
    pid_t pid;
    struct sigaction *act = (struct sigaction *)malloc(sizeof(struct sigaction));
    act->sa_handler = read_childproc;
    sigemptyset(&(act->sa_mask));
    act->sa_flags = 0;
    sigaction(SIGCHLD, act, 0);
    
    pid = fork();
    if(pid ==0){
        puts("hello I'm child process");
        sleep(10);
        return 12;
    }
    else {
        printf("child proc id : %d \n", pid);
        pid = fork();
        if(pid == 0){
            puts("hello I'm child process");
            sleep(10);
            return 24;
        }
        else {
            int i;
            printf("Child proc id : %d \n", pid);
            for (i=0; i<5; i++){
                puts("wait....");
                sleep(5);
            }
        }
    }
    return 0;
}

```