#include <stdio.h>
#include "durModel.h"
#include "durView.h"
#include "durControl.h"

static es durControl_inputCardOrCommand() { //return card [0..ed_cards) or command (es > 1000)
    fflush(stdout);
    fflush (stdin);
    char s[2+1];
    int i = scanf("%2s", s); //printf("i=%d, s=%s, s[0]=%d, s[1]=%d, s[2]=%d.\n", i, s, s[0],s[1],s[2]); //dbg
    if (i != 1) return es_cmd_wrong;
    if (s[0] > 96 && s[0] < 123) s[0] -= ('a' - 'A'); //toupper(s[0])
    //command
    if (s[1] == 0) {
        if(s[0] == 'E') return es_cmd_enough;
        if(s[0] == 'T') return es_cmd_take;
        if(s[0] == 'Y') return es_cmd_newGame;
        if(s[0] == 'Q') return es_cmd_quit;
        return es_cmd_wrong;
    }
    //card
    int rank = 0;
    for (; rank < ed_ranks; ++rank) {
        if (s[0] == ranks[rank]) break;
    }
    if (rank == ed_ranks) return es_cmd_wrong;
    if (s[1] > 64 && s[1] < 91)  s[1] += ('a' - 'A'); //tolower(s[0])
    int suit = 0;
    for (; suit < ed_suits; ++suit) {
        if (s[1] == suits[suit]) break;
    }
    if (suit == ed_suits) return es_cmd_wrong;
    return suit * ed_ranks + rank; //card
}

void durControl(sBoard *b) {
//    printf("durControl\n");
    switch (b->stage) {
        case es_attackControl:
            b->cmd = durControl_inputCardOrCommand();
            b->stage = es_attackResult;
            break;
        case es_defendControl:
            b->cmd = durControl_inputCardOrCommand();
            b->stage = es_defendResult;
            break;
    }
}
