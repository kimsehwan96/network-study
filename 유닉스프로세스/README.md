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
