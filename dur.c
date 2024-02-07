#include <stdlib.h> //for srand, rand, malloc, free
#include <time.h>   //for srand
#include "dur.h"
#include "durView.h"
//#include <stdio.h>

static int suit[ed_cards] = { 0,0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,1, 2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,3 };
static int rank[ed_cards] = { 0,1,2,3,4,5,6,7,8, 0,1,2,3,4,5,6,7,8, 0,1,2,3,4,5,6,7,8, 0,1,2,3,4,5,6,7,8 };

static sBoard *newBoard() { //burst: +neGame+neFight
    sBoard *b = malloc(sizeof(sBoard));
    if (b != NULL) {
        for (int i = 0; i < ed_cards; ++i) b->desk.order[i] = i; //cards filling
        b->left.place = ep_left;
        b->left.score = 0;
        b->right.place = ep_right;
        b->right.score = 0;
        b->winner = NULL;
        b->game = 0;
        b->stage = es_newGame;
    }
    return b;
}

static bool newGame_shuffle_isBad(const sDesk *d) { //true - if there are more than 4 same suits in one hand
    int sl[ed_suits] = {0}, sr[ed_suits] = {0};
    for (int l = ed_talon, r = ed_talon + ed_normal; r < ed_cards; ++l, ++r) {
        if (++sl[suit[d->order[l]]] > 4) return true;
        if (++sr[suit[d->order[r]]] > 4) return true;
    }
    return false;
}
static void newGame_shuffle_indexAndPlaceInit(sDesk *d) {
    for (int i = 0; i < ed_cards; ++i) {
        d->index[d->order[i]] = i;
        d->place[i] = ep_desk;
    }
}
static void newGame_shuffle(sDesk *d) { //shuffle, place & order filling
    d->count = ed_cards;
    srand((unsigned)time(NULL));
    do { //card shuffle
        for (int i = 0; i < ed_cards; ++i) {
            int r = rand() % ed_cards;
            int depot = d->order[r]; d->order[r] = d->order[i]; d->order[i] = depot;
        }
    } while (newGame_shuffle_isBad(d));
    newGame_shuffle_indexAndPlaceInit(d);
    d->trump = suit[d->order[0]]; //suits of trump
}
static void newGame_players(sBoard *b) {
    b->left.count  = 0;
    b->right.count = 0;
    if(b->winner == NULL) { //roles :drawing (first game or drawn)
        srand((unsigned)time(NULL));
        b->attacker = (rand() % ed_players) ? &b->right : &b->left;
    } else {
        b->attacker = b->winner;
    }
    b->defender = (b->attacker == &b->left) ? &b->right : &b->left;
    b->dealer = &b->right; //first dealing - it doesn't matter who's first
}
static void newGame(sBoard *b) {
    newGame_shuffle(&b->desk);
    newGame_players(b);
    b->fight = 0;
    b->history.count = 0;
    ++b->game;
    b->stage = es_newFight;
}

static void history(sHistory *h, int card, ep place) {
    h->card[h->count] = card;
    h->place[h->count++] = place;
}

static void newFight_dealing(sBoard *b) {
    sPlayer *p = b->dealer;
    bool flNextDealing = (b->desk.count != ed_cards); //at first dealing - 6l, 6r in end of the order
    while (p->count < ed_normal && b->desk.count > 0) {
        b->desk.place[b->desk.order[--b->desk.count]] = p->place;
        ++p->count;
        if (flNextDealing) history(&b->history, b->desk.order[b->desk.count], p->place);
    }
    p = (p == &b->left) ? &b->right : &b->left;
    while (p->count < ed_normal && b->desk.count > 0) {
        b->desk.place[b->desk.order[--b->desk.count]] = p->place;
        ++p->count;
        if (flNextDealing) history(&b->history, b->desk.order[b->desk.count], p->place);
    }
}
static void newFight(sBoard *b) {
    newFight_dealing(b);
    b->dealer = b->attacker; //for next newFight
    b->fight = 0; //fight reset
    b->stage = es_attack;
}





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
            break;
        case es_newFight:
            newFight(b);
            break;
        case es_attack:
//            attack(b);
            b->stage = es_cmd_quit;
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
        durView((const sBoard *)b1);    //отделяем для автономности - возможность потока (в потоках - либо учим симафоры, либо - проще - бьём stage на подуровни ..View и ..Control для синхронизации
//        durControl(b1); //отделяем для автономности - возможность потока
    } while (b1->stage != es_cmd_quit);
    dbgView_board(b1);
    free(b1);
//    durView_msg(".> Quit.");
}