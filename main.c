#include "durModel.h"
#include "durView.h"
#include "durControl.h"

void dur() {
    sMatch *m = durModel_newMatch();
    while (m != NULL) {
        durModel(m);
        durView(m);    //отделяем для автономности - возможность потока (в потоках - либо учим симафоры, либо - проще - бьём state на подуровни ..View и ..Control для синхронизации
        durControl(m); //отделяем для автономности - возможность потока
        durModel_newMatch_quitCheckout(&m);
    }
}

int main(void) { //int argc, char *argv[], char *envp[]
    dur();
    return 0;
}
