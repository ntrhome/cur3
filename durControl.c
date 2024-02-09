#include <stdio.h>
//#include "durModel.h"
#include "durView.h"
#include "durControl.h"

static const char rankChar[] = { '6','7','8','9','X','J','Q','K','A' };
static const char suitChar[] = { 's','c','d','h' };

static es durControl_input() { //return card [0..ed_cards) or command (es > 1000)
    fflush(stdout);
    fflush (stdin);
    char s[3];
    int i = scanf("%2s", s); //printf("i=%d, s=%s, s[0]=%d, s[1]=%d, s[2]=%d.\n", i, s, s[0],s[1],s[2]); //dbg
    if (i != 1) return es_cmdWrong;
    if (s[0] >= 'a' && s[0] <= 'z') s[0] -= ('a' - 'A'); //toupper(s[0])
    //command
    if (s[1] == 0) { //typed one symbol - command
        if(s[0] == 'E') return es_cmdEnough;
        if(s[0] == 'T') return es_cmdTake;
        if(s[0] == 'Y') return es_cmdNewGame;
        if(s[0] == 'Q') return es_cmdQuit;
        return es_cmdWrong;
    }
    //typed two symbol - card
    int i = 0;
    for (; i < ed_ranks; ++i) {
        if (s[0] == rankChar[i]) break;
    }
    if (i == ed_ranks) return es_cmdWrong;
    if (s[1] >= 'A' && s[1] <='Z') s[1] += ('a' - 'A'); //tolower(s[0])
    i = 0;
    for (; i < ed_suits; ++i) {
        if (s[1] == suitChar[i]) break;
    }
    if (suit == ed_suits) return es_cmd_wrong;
    return suit * ed_ranks + rank; //card
}

static es durControl_attack() {
    int cmd = durControl_input();

    return 0;
}


void durControl(sBoard *b) {
//    printf("durControl\n");
    switch (b->stage) {
        case es_attackControl:
            b->cmd = durControl_attack();
            b->stage = es_attackResult;
            break;
//        case es_defendControl:
//            b->cmd = durControl_inputCardOrCommand();
//            b->stage = es_defendResult;
//            break;
    }
}
