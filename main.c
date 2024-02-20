#include "durModel.h"
#include "durView.h"
#include "durControl.h"

void dur() {
    sMatch *m = durModel_newMatch();
    while (m != NULL) {
        durModel(m);
        durView(m);    //отделяем для автономности - возможность потока (в потоках - либо учим симафоры, либо - проще - бьём state на подуровни ..View и ..Control для синхронизации
        durControl(m); //отделяем для автономности - возможность потока
//        m->state = es_cmdQuit;
printf("DBG Match=%p\n", m);
        durModel_quitCheckout(&m);
printf("DBG Match=%p\n", m);
    }
    //durView_match(m);
}

int main(void) { //int argc, char *argv[], char *envp[]
    dur();
    return 0;
}
