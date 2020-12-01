#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4
void error_handling(char *message);

int main(int argc, char *argv[]){
    int sock;
    char opmsg[BUF_SIZE];
    int result, opnd_cnt, i;
    struct sockaddr_in serv_adr;
    if(argc !=3){
        printf("usage : %s <ip> <port> \n", argv[0]);
        exit(1);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);//tcp
    if(sock==-1){
        error_handling("Socket() error !");
    }
    memset(&serv_adr, 0 , sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]); //ip address를 실행 인자로 받음
    serv_adr.sin_port = htons(atoi(argv[2])); //port number를 실행 인자로 받음

    if(connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1){
        error_handling("connect() error");//connect
    }
    else {
        puts("connected.......");
    }
    fputs("operand count : ", stdout);
    scanf("%d", &opnd_cnt); //연산할 값들의 개수를 받는다.
    opmsg[0] = (char)opnd_cnt;
    for (i=0; i<opnd_cnt; i++){
        printf("Operand %d: ", i + 1);
        scanf("%d", (int*)&opmsg[i*OPSZ+1]); //char형 포인터 변수 opmsg를 int 형 포인터 변수로 강제 형변환.
    }
    fgetc(stdin);
    fputs("operator : ", stdout);
    scanf("%c", &opmsg[opnd_cnt*OPSZ+1]);
    write(sock, opmsg, opnd_cnt*OPSZ+2);
    read(sock, &result, RLT_SIZE);

    printf("Operation result : %d \n", result);
    close(sock);
    return 0;
}

void error_handling(char *message){
    fputs(message, stderr);
    fputs("\n", stderr);
    exit(1);
}