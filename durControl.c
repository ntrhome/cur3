#include "durControl.h"

static const char rankChar[ed_ranks] = { '6','7','8','9','X','J','Q','K','A' };
static const char suitChar[ed_suits] = { 's','c','d','h' };

static es input() { //return card [0..ed_cards) or command (es > 1000)
    fflush(stdout);
    fflush (stdin);
    char s[3];
    if (scanf("%2s", s) != 1) return es_cmdWrong; //printf("i=%d, s=%s, s[0]=%d, s[1]=%d, s[2]=%d.\n", i, s, s[0],s[1],s[2]); //dbg
    if (s[0] >= 'a' && s[0] <= 'z') s[0] -= ('a' - 'A'); //toupper(s[0])
    if (s[1] == 0) { //typed one symbol - command
        if (s[0] == 'E') return es_cmdEnough;
        if (s[0] == 'T') return es_cmdTake;
        if (s[0] == 'Y') return es_cmdNewGame;
        if (s[0] == 'Q') return es_cmdQuit;
        return es_cmdWrong;
    }
    //typed two symbol - card
    int rank = 0;
    for (; rank < ed_ranks; ++rank) {
        if (s[0] == rankChar[rank]) break;
    }
    if (rank == ed_ranks) return es_cmdWrong;
    if (s[1] >= 'A' && s[1] <='Z') s[1] += ('a' - 'A'); //tolower(s[0])
    int suit = 0;
    for (; suit < ed_suits; ++suit) {
        if (s[1] == suitChar[suit]) break;
    }
    if (suit == ed_suits) return es_cmdWrong;
    return suit * ed_ranks + rank; //card
}

void durControl(sMatch *m) {
    switch (m->stage) {
        case es_attackControl:
            m->cmd = input();
            m->stage = es_attackResult;
            break;
//        case es_defendControl:
//            b->cmd = durControl_inputCardOrCommand();
//            b->stage = es_defendResult;
//            break;
    }
}
