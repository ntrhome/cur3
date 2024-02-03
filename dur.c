#include <stdlib.h> //for srand, rand, malloc, free
#include <time.h>   //for srand
#include "dur.h"
#include "durView.h"

int d_suit[ed_cards] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3};
int d_rank[ed_cards] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 1, 2, 3, 4, 5, 6, 7, 8};

static bool newGame_deskShuffle_isBadShuffle(const sDesk *d) { //true - if there are more than 4 same suits in one hand
    int suit[ed_suits];
    suit[0]=0; suit[1]=0; suit[2]=0; suit[3]=0;
    int i = ed_talon;
    for (; i < ed_talon+ed_normal; ++i) if (++suit[d_suit[d->card[i]]] > 4) return true;
    suit[0]=0; suit[1]=0; suit[2]=0; suit[3]=0;
    for (; i < ed_cards; ++i) if (++suit[d_suit[d->card[i]]] > 4) return true;
    return false;
}
static void newGame_deskShuffle(sDesk *d) { //shuffle, place & order filling
    d->count = ed_cards;
    srand((unsigned)time(NULL));
    do { //card shuffle
        for (int i = 0; i < ed_cards; ++i) {
            int r = rand() % ed_cards;
            int depot = d->card[r]; d->card[r] = d->card[i]; d->card[i] = depot;
        }
    } while (newGame_deskShuffle_isBadShuffle(d));
    for (int i = 0; i < ed_cards; ++i) {
        d->order[d->card[i]] = i;
        d->place[i] = ep_desk;
    }
    d->trump = d->card[0] / ed_ranks; //suits of trump
}
static void newGame_playersReset(sBoard *b) {
    b->left.count  = 0;
    b->right.count = 0;
    if(b->winner == NULL) { //roles :drawing (first or drawn game)
        srand((unsigned)time(NULL));
        b->attacker = (rand() % ed_players) ? &b->right : &b->left;
    } else {
        b->attacker = b->winner;
    }
    b->defender = (b->attacker == &b->left) ? &b->right : &b->left;
    b->dealer = &b->right; //first dealing - it doesn't matter who's first
}
void newGame(sBoard *b) {
    newGame_deskShuffle(&b->desk);
    newGame_playersReset(b);
    b->countAttack = 0;
    b->countDefend = 0;
    b->stage = es_newFight;
}

sBoard *newBoard() { //burst: +neGame+neFight
    sBoard *b = malloc(sizeof(sBoard));
    if (b != NULL) {
        for (int i = 0; i < ed_cards; ++i) b->desk.card[i] = i; //cards filling
        b->left.place = ep_left;
        b->left.score = 0;
        b->right.place = ep_right;
        b->right.score = 0;
        b->winner = NULL;
        b->stage = es_newGame;
    }
    return b;
}

static void newFight_deskDealing(sBoard *b) {
    sPlayer *p = b->dealer;
    while (p->count < ed_normal && b->desk.count > 0) { b->desk.place[b->desk.card[--b->desk.count]] = p->place; ++p->count; }
    p = (p == &b->left) ? &b->right : &b->left;
    while (p->count < ed_normal && b->desk.count > 0) { b->desk.place[b->desk.card[--b->desk.count]] = p->place; ++p->count; }
}
void newFight(sBoard *b) {
    newFight_deskDealing(b);
    b->dealer = b->attacker; //for next newFight
    b->countAttack = 0; //fight reset
    b->countDefend = 0;
    b->stage = es_attack;
}




//static void history(sHistory *h, int card, ep place) {
//    h->card[h->count] = card;
//    h->place[h->count++] = place;
//}
//


//static bool isCmdAsAcceptableCommand(const sBoard *b) {
//
//}

//static bool isCmdAsAcceptableCard_isFightHasRank(const sBoard *b) {
//
//}
//static bool isCmdOkCard(const sBoard *b) {
//    switch (b->stage) {
//        case es_attack:
//            if(b->attack.count > 0) {
//                int rank = ((int)b->cmd) % ed_ranks;
//                for (int i = 0; 0 < b->attack.count; ++i) {
//                    if (b->attack.card[i] % ed_ranks == rank || b->defend.card[i] % ed_ranks == rank ) {
//
//                    }
//                }
//            }
//
//    }
//
//
//}

void durModel(sBoard *b) {
    switch (b->stage) {
        case es_newGame:
            newGame(b);
//            break;
        case es_newFight:
            newFight(b);
//            break;
        case es_attack:
//            attack(b);
            break;
//        case es_attackResult:
//            attackResult(b);
//            break;
//        case es_defend:
//            defend(b);
//            break;
//        case es_defendResult:
//            defendResult(b); //todo
//            break;
//        case es_fightCloseAsDefended:
//            fightCloseAsDefended(b);
//            break;
//        case es_fightCloseAsTook:
//            fightCloseAsTook(b);
//            break;
    }
}

void dur() {
    sBoard *b1 = newBoard();
    do {
        durModel(b1);
//        durView(b1);    //отделяем для автономности - возможность потока
//        durControl(b1); //отделяем для автономности - возможность потока
    } while (false);
    dbgView_board(b1);
    free(b1);
//    durView_msg(".> Quit.");
}