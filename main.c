#include "main.h"
#include "dur.h"
#include "durView.h"
#include <stdlib.h> //for srand, rand, malloc, free

void dur() {
    sMatch *m = durNewMatch();
    while (m != NULL && m->stage != es_cmdQuit) {
//        durModel(m);
//        durView(m);    //отделяем для автономности - возможность потока (в потоках - либо учим симафоры, либо - проще - бьём stage на подуровни ..View и ..Control для синхронизации
//        durControl(m); //отделяем для автономности - возможность потока
        durView_match(m);
        m->stage = es_cmdQuit;
    }
    free(m);
}

int main(void) { //int argc, char *argv[], char *envp[]
    dur();
    return 0;
}
