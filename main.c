//#include "durModel.h"
#include "dur.h"
#include <stdio.h>

typedef double dec;
typedef struct {
    int a;
    dec b;
} S;

S f() {
    S s = {5, .5};
    return s;
}


int main(void) { //int argc, char *argv[], char *envp[]
    //dur();
    S w = f();

    return 0;
}

void f1() {
    char ch;
    while ((ch = getchar()) != '#') putchar(ch);
}