#include "durControl.h"
#include <stdio.h>

static const char rankChar[ed_ranks] = { '6','7','8','9','X','J','Q','K','A' };
static const char suitChar[ed_suits] = { 's','c','d','h' };
extern const int cardOfRankSuit[ed_ranks][ed_suits];

static es input() { //return card [0..ed_cards) or command (es > 1000)
    fflush(stdout);
    fflush (stdin);
    char s[3]; //printf("i=%d, s=%s, s[0]=%d, s[1]=%d, s[2]=%d.\n", i, s, s[0],s[1],s[2]);
    if (scanf("%2s", s) == 1) {
        if (s[0] >= 'a' && s[0] <= 'z') s[0] -= ('a' - 'A'); //toupper(s[0])
        if (s[1] == 0) { //one symbol - command
            switch (s[0]) {
                case 'E': return es_cmdEnough;
                case 'T': return es_cmdTake;
                case 'Y': return es_cmdYes;
                case 'Q': return es_cmdQuit;
            }
        } else {         //two symbol - card
            int rank = 0;
            for (; rank < ed_ranks; ++rank) {
                if (s[0] == rankChar[rank]) break;
            }
            if (rank < ed_ranks) {
                if (s[1] >= 'A' && s[1] <= 'Z') s[1] += ('a' - 'A'); //tolower(s[0])
                int suit = 0;
                for (; suit < ed_suits; ++suit) {
                    if (s[1] == suitChar[suit]) break;
                }
                if (suit < ed_suits) {
                    return cardOfRankSuit[rank][suit]; //suit * ed_ranks + rank;
                }
            }
        }
    }
    return es_cmdWrong;
}

void durControl(sMatch *m) {
    if (m != NULL) {
        switch (m->state) {
        case es_attackControl:
            m->cmd = input();
            m->state = es_attackHandler;
            break;
//        case es_defendControl:
//            b->cmd = durControl_inputCardOrCommand();
//            b->state = es_defendResult;
//            break;
        }
    }
}
