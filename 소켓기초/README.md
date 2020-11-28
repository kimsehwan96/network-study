# 소켓프로그래밍 기초
- 소켓은 TCP나 UDP와 같은 트랜스포트 계층을 이용하는 API이다.
    - 언어에 종속성 없이, 해당 언어에서 API를 제공해주면 동일 동작(C, Python, java, C++, C#..)

- 소켓은 응용프로그램에서 TCP/IP를 이용하는 창구 역할을 하고 있다.
    - 응용프로그램과 소켓 사이의 인터페이스를 소켓 인터페이스라고 한다.

## 소켓 번호
- 유닉스에서 파일을 새로 열면 int 타입의 파일 디스크립터를 리턴한다.
- 파일을 조작할때 이 디스킙터를 사용하면 된다.
    - 유닉스는 정규 파일 뿐 아니라, 키보드, 마우스, 하드웨어 장치, 파이프, 소켓등을 모두 파일로 취급.
    - 모두 파일 디스크립터를 이용해 접근 가능!

- 파일을 처음 open 하면, 커널은 이 파일에 관한 각종 정보를 갖고있는 구조체를 할당한다.
- 이 구조체를 가르키는 포인터로 구성된 테이블을 파일 디스크립터 테이블이라고도 함.
- 이 테이블의 인덱스값이 바로 파일 디스크립터이다.

## 소켓 사용법
- 소켓 개설
    - 소켓을 개설하여 톨신에 이용하기위해서는 다음의 다섯가지 정보가 정해져야 함
        - 통신에 사용할 프로토콜
        - 자신의 IP 주소
        - 자신의 포트번호
        - 상대방의 IP주소
        - 상대방의 포트번호
- 소켓 프로그래밍의 첫번째는 소켓을 만드는 것

```c
#include<sys/socket.h>
int socket(
    int domain, //프로토콜 체계
    int type, //서비스 타입
    int protocol //소켓에서 사용할 프로토콜
);
```

- socket()이 성공정으로 수행되면 소켓 번호를 리턴하고, 에러 발생시 -1 리턴 및 전역변수 ernno에 에러코드가 들어간다.
- 소켓은 TCP/IP 뿐 아니라 다양한 네트워크 프로토콜을 위해 일반적으로 정의된 개념임
- TCP/IP, UNIX NETWORK, XEROX 등등..

- 우리는 domain을 AF_INET을 이용하면 된다.
- 두번쨰 인자인 type은 서비스 타입을 의미한다.
    - SOCK_STREAM (TCP 소켓 생성)
    - SOCK_DGRAM (UDP 소켓 생성)
    - SOCK_RAW (RAW 소켓 생성)
- 세번쨰 인자의 경우 TCP, UDP 사용시 필요 없는 인자이며, 0 을 입력
- RAW소켓의 경우 protocol 필드가 구체적인 상위 프로토콜을 지정하는데 필요하다.

##### 소켓 시스템콜 호출 및 생성된 소켓 번호 출력 코드

```c
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
```

<strong> 출력 </strong>
```console
[Running] cd "/Users/gimsehwan/Desktop/ingkle/netc/소켓기초/" && gcc open_socket.c -o open_socket && "/Users/gimsehwan/Desktop/ingkle/netc/소켓기초/"open_socket
file1's fd 3
stream socket descriptor 4
datagram socket descriptor 5
file2's fd 6
```

- 시스템이 이미 0, 1, 2번 파일 디스크립터를 각각 표준 입력, 표준 출력, 표준 에러출력으로 잡아놓았다
- 따라서 3번부터 배정된다.

### 소켓주소 구조체

```c
struct sockaddr_in {
    short sin_family; // 주소 체계 (AF_INET, AF_UNIX, AF_NS 등등) 
    u_short sin_port; // 16비트 포트번호
    struct in_addr sin_addr; // 32비트 IP주소
    char sin_zero[8]; // 전체 크기를 16바이트로 맞추기 위한 dummy
};
```

### 소켓 사용 절차
- 클라이언트와 서버가 각각 TCP 소켓을 이용해 연결하는 경우
- 서버 프로그램이 먼저 실행되고 있어야 하며
    - 서버는 socket()을 호출하여 소켓을 개설하고, 이 때 리턴된 소켓번호와 자신의 소켓 주소(자신의 포트와 ip어드레스)를 bind()를 호출하여 연결시켜놓는다.
    - 그니까 소켓 번호는 응용프로그램만 알고있는 통신 창구 번호고, 포트랑 어드레스는 네트워크 시스템이 알고있는거니까 둘을 bind해서 묶어주는거래
    - 서버는 listen()을 호출하여 수동 대기모드로 들어가며 클라이언트로부터 오는 연결 요청을 처리 할 수 있게 된다.
    - 서버는 연결 설정 결과를 얻기 위해 accept() 함수를 호출하고 기다린다.
    - 클라이언트와 연결이 성공적으로 이루어지면 accept()가 새로운 소켓을 하나 리턴하는데
    - 이 소켓을 통해 서버와 클라이언트가 데이터 송수신 가능
- 클라이언트는
    - socket()을 호출하여 소켓을 만든 후, 서버와 연결 설정을 하기 위해 connect()를 호출한다.
    - 클라이언트는 접속할 상대방의 서버의 소켓 주소(상대방 ip, 상대방 port번호) 구조체를 만들어 connect()함수의 인자로 주어야 함
    - 클라이언트에서는 서버와 달리 bind()를 호출하지 않는데 왜그러냐?
        - 클라이언트는 보통은 자신의 ip, port번호를 굳이 특정값으로 지정해둘 필요가 없어서!
        - 보통은 자동으로 지정되는 Port number를 사용한다 (클라이언트!)
        - 만약 수동으로 지정하게 될 경우 안정성이 떨어짐.
        - 클라이언트 프로그램이 하나의 컴퓨터에서 두개 이상이 실행되면 포트번호 중복 에러가 발생해서 그럼

### 바이트 순서 (byte order)
- 바이트 순서는 호스트 바이트 순서와 네트워크 바이트 순서 두가지가 있다
    - 호스트 바이트 순서는 컴퓨터가 내부 메모라에 숫자를 저장하는 순서를 말하는데, 이것은 CPU의 종류에 따라 다르다
    - 예를들어 2바이트로 구성된 십진수 `50146`의 경우 16진수로는 `0xC3E2`인데, x86 계열에서는 E2, C3의 순서로 즉 하위 바이트부터 메모리에 저장되고
    - MC68000 계열의 CPU는 C3 E2의 순으로 저장. 전자를 하위 바이트가 먼저 저장된다고 해서 little-endian이라고 함. 후자는 big-endian
    - 실제 실무로 겪으면서 endian 문제는 매우 많이 겪었다... 

- 네트워크 바이트 순서는 포트 번호나, IP주소와 같은 정보를 바이트 단위로 네트워크로 전송하는 순서를 말하는데 
- 어떤 CPU인지에 따라서 달라지니까, 이걸 해결해주기 위해서 몇몇 함수가 있다.
- 컴퓨터 내부에서 사용하던 숫자 sockaddr_in 구조체 내의 포트번호 같은거를 htons() 함수를 이용해 모두 네트워크 바이트 순서로 바꿔야 된다
    - host to network short 의 약자임
- 반대로 네트워크로 부터 수신한 숫자는 ntohs() 함수를 이용해 자신의 호스트 바이트 순서로 바꾸어야 한다.

<strong> 바이트 순서 확인 프로그램 </strong>

```c
// byte_order.c
// 호스트 바이트 순서 테스트 프로그램

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(void){
    struct servent *servent;
    servent = getservbyname("echo", "udp");
    if (servent == NULL) {
        printf("정보 알 수 없음");
        exit(1);
    }
    printf("UDP 에코 포트번호 (네트워크 순서) : %d \n", servent->s_port);
    printf("UDP 에코 포트번호 (호스트 순서) : %d \n", ntohs(servent->s_port));

    return 0;
}
```

<strong> 출력 </strong>

```console

[Running] cd "/Users/gimsehwan/Desktop/ingkle/netc/소켓기초/" && gcc byte_order.c -o byte_order && "/Users/gimsehwan/Desktop/ingkle/netc/소켓기초/"byte_order
UDP 에코 포트번호 (네트워크 순서) : 1792 
UDP 에코 포트번호 (호스트 순서) : 7 
```

- x86 CPU이기 때문에 little-endian이 적용된 모습이다. 정수 1792는 0x0007의 바이트 숫자가 0x0700로 바뀐 십진수 값임.
