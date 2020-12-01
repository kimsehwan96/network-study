#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#define MAXLINE 512

int main(){
    int fd;
    fd = open("file.lst", O_RDONLY);
    char *str = (char *)malloc(MAXLINE);
    int read_bytes = 0;
    while((read_bytes = read(fd, str, MAXLINE)) > 0){
            if (read_bytes < MAXLINE) { //마지막 NULL문자 및 EOF삭제.
                str[read_bytes] = '\0';
            }
            printf("%s", str);
    }
    return 0;
}