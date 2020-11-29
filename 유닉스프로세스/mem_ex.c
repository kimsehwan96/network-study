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