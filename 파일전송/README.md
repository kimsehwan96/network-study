# 파일 전송

### 파일 읽기 먼저
- 파일을 열고 난 후 데이터를 쓰거나 읽어올 때에는
- read와 write 함수를 사용합니다.

```c
#include <unistd.h>

ssize_t read(int fd, void *buff, size_t nbytes);
//반환값 : 읽어온 바이트 수, 실패시 -1, 파일의 끝에서 시도하면 0

ssize_t write(int fd, const void *buff, size_t nbytes);
//반환값 : 기록한 바이트 수, 실패시 -1

```

- 생각하고 기억해야 할 점
    - read 함수를 호출하였을 때 반환 값은 읽기 요청한 바이트수보다 작은 값일 수 있다.
    - 예를들어 읽어올 데이터가 100바이트밖에 없는데 200바이트 읽기를 요청하면 100바이트를 읽고 100을 반환
    - 그니까 요청한 바이트 수보다 작은값을 반환하면 파일의 끝을 만난거다 !


```c
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#define MAXLINE 512

int main(){
    int fd;
    fd = open("file.lst", O_RDONLY);
    char str[MAXLINE];
    int read_bytes = 0;
    while((read_bytes = read(fd, str, MAXLINE)) > 0){
            if (read_bytes < MAXLINE){
                str[read_bytes] = '\0';
            }
            printf("%s", str);
    }
    return 0;
}
```

- 버퍼에 파일 입력을 넣어 출력하는 코드.
- MAXLINE이 512바이트이므로, 512바이트씩 끊어서 읽게 된다.