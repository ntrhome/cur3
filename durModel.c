#include <stdlib.h> //for srand, rand, malloc, free
#include <time.h>   //for srand
#include "dur.h"




void dur() {
    sMatch *m1 = newMatch();
    while (m1 != NULL && m1->stage != es_cmdQuit) {
        durModel(m1);
//        durView(b1);    //отделяем для автономности - возможность потока (в потоках - либо учим симафоры, либо - проще - бьём stage на подуровни ..View и ..Control для синхронизации
//        durControl(b1); //отделяем для автономности - возможность потока
    }
    durView_board(m1);
    free(m1);
}