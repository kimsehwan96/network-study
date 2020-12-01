# 포인터 형변환

```c
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#define ops 4

int main(void){
    char msg[1024];
    //int t = 65; //'A'
    fputs("Start ! \n", stdout);
    memset(msg, 0, 1024);
    msg[0] = 'a';
    for(int i =0; i<3; i++){
        printf("Input an integer %d :", i + 1);
        scanf("%d", (int *)&msg[i*ops +1]); //1바이트 정수(캐릭터형) 배열에, 정수를 담기 위해서 4바이트를 써야된다
        //이때 포인터 형변환을 하고, 배열의 인덱스를 잘 가공해주면
        //예를들어 &msg[1],&msg[2],&msg[3],&msg[4] 이것들은 1바이트 데이터를 담을 주소니까
        //여기에 강제 형변환을 해서 &msg[1] 즉, 시작주소에 정수를 넣어주면 &msg[4]까지 나눠서 담긴다.
        fputs("\n", stdout);
    }
    for(int i=0; i<3; i++){
        printf("%d\n", *(int *)&msg[i*ops+1]);
    }//잘 동작한다.
    
    return 0;
    
}
```
- ㅎㅎ.