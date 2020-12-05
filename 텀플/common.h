//here will be useful common functions
/*
유저 인증 및 macro 상수를 정의하는 파일 임.
유저의 id, password를 저장하고 있으며,
유저를 구분하기 위한 token 값을 상수로 define 하여 사용함.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define INIT_MSG "===================\n Hi this is p2p test plz login\n ===============\n"
#define ID_REQ "input id : "
#define PW_REQ "input password : "
#define USER1_ID "user"
#define USER1_PW "passwd1"
#define USER2_ID "user2"
#define USER2_PW "passwd2"

#define USER1_LOGIN 1
#define USER2_LOGIN 2
#define LOGIN_FAIL 0
#define BUFSIZE 512
#define FILESIZE 65536

// 유저의 인증로직을 위한 함수.
unsigned int authenticate(int fd, char *id, char *pw)
{
    send(fd, ID_REQ, strlen(ID_REQ) + 1, 0); //strlen((char *)some_string +1) 인 이유는 Null 포함
    read(fd, id, sizeof(id));
    send(fd, PW_REQ, strlen(PW_REQ) + 1, 0);
    read(fd, pw, sizeof(pw));

    printf("===========================\n");
    printf("User Information\n");
    printf("id : %s  pw : %s \n", id, pw);
    printf("===========================\n");

    if (strcmp(id, USER1_ID) == 0)
    {
        if (strcmp(pw, USER1_PW) == 0)
        {
            printf("%s Login success \n", id);
            return USER1_LOGIN;
        }
        else
        {
            return LOGIN_FAIL;
        }
    }
    else if (strcmp(id, USER2_ID) == 0)
    {
        if (strcmp(pw, USER2_PW) == 0)
        {
            printf("%s Login success \n", id);
            return USER2_LOGIN;
        }
        else
        {
            return LOGIN_FAIL;
        }
    }
    else
        return LOGIN_FAIL;
}