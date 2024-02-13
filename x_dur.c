#include <stdlib.h> //for srand, rand, malloc, free
#include <time.h>   //for srand
#include "dur.h"
#include "durView.h"
//#include "durControl.h"

static int suit[ed_cards] = { 0,0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,1, 2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,3 };
static int rank[ed_cards] = { 0,1,2,3,4,5,6,7,8, 0,1,2,3,4,5,6,7,8, 0,1,2,3,4,5,6,7,8, 0,1,2,3,4,5,6,7,8 };
//Lib
static sMatch *durModel_newMatch() {
    sMatch *m = malloc(sizeof(sMatch));
    if (m != NULL) {
        for (int i = 0; i < ed_cards; ++i) m->desk.card[i] = i;
        m->left.place  = ep_left,  m->left.score  = 0;
        m->right.place = ep_right, m->right.score = 0;
        m->game = 0;
        m->winner = NULL;
        m->stage = es_newGame;
    }
    return m;
}

static bool newGame_desk_isBadShuffle(const sDesk *d) { //true - if there are more than 4 same suits in one hand
    int s[ed_suits];
    for (int i = ed_talon; i < ed_cards; i += ed_normal) {
        for (int j = 0; j < ed_suits; ++j) s[j] = 0; //clear
        for (int j = i; j < i + ed_normal; ++j) {
            if (++s[suit[d->card[j]]] > 4) return true;
        }
    }
    return false;
}
static void newGame_desk(sDesk *d) { //shuffle, place & order filling
    d->count = ed_cards;
    srand((unsigned)time(NULL));
    do { //card shuffle
        for (int i = 0; i < ed_cards; ++i) {
            int r = rand() % ed_cards;
            int depot = d->card[r]; d->card[r] = d->card[i]; d->card[i] = depot;
        }
    } while (newGame_desk_isBadShuffle(d));
    for (int i = 0; i < ed_cards; ++i) { //init position and place
        d->position[d->card[i]] = i;
        d->place[i] = ep_desk;
    }
    d->trump = suit[d->card[0]]; //suits of trump
}
static void newGame_players(sMatch *m) {
    m->left.count  = 0;
    m->right.count = 0;
    if(m->winner == NULL) { //roles :drawing (first game or drawn)
        srand((unsigned)time(NULL));
        m->attacker = (rand() % ed_players) ? &m->right : &m->left;
    } else {
        m->attacker = m->winner;
    }
    m->defender = (m->attacker == &m->left) ? &m->right : &m->left;
    m->dealer = &m->right; //first dealing - it doesn't matter who's first
}
static void newGame(sMatch *m) {
    newGame_desk(&m->desk);
    newGame_players(m);
    ++m->game;
    m->fight = 0;
    m->history.count = 0;
}

//static void history(sHistory *h, int card, ep place) {
//    h->card[h->count] = card;
//    h->place[h->count++] = place;
//}
//
//static void newFight_dealing(sBoard *b) {
//    sPlayer *p = b->dealer;
//    bool flNextDealing = (b->desk.count != ed_cards); //at first dealing - 6l, 6r in end of the order
//    while (p->count < ed_normal && b->desk.count > 0) {
//        b->desk.place[b->desk.order[--b->desk.count]] = p->place;
//        ++p->count;
//        if (flNextDealing) history(&b->history, b->desk.order[b->desk.count], p->place);
//    }
//    p = (p == &b->left) ? &b->right : &b->left;
//    while (p->count < ed_normal && b->desk.count > 0) {
//        b->desk.place[b->desk.order[--b->desk.count]] = p->place;
//        ++p->count;
//        if (flNextDealing) history(&b->history, b->desk.order[b->desk.count], p->place);
//    }
//}
//static void newFight(sBoard *b) {
//    newFight_dealing(b);
//    b->dealer = b->attacker; //for next newFight
//    b->fight = 0; //fight reset
//}
//
//static void attack(sBoard *b) {
//    //cheks
//    b->stage = es_attackView;
//}



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

void durModel(sMatch *m) {
    switch (m->stage) {
        case es_newGame:
            newGame(m);
            m->stage = es_newGameView; //es_newFight
            break;
//        case es_newFight:
//            newFight(b);
//            b->stage = es_newFightView; //es_attack
//            break;
//        case es_attack:
//            attack(b);
//            b->stage = es_attackView;
//            break;
//        case es_attackResult:
////            attackResult(b);
//            b->stage = es_cmdQuit;
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
    m->stage = es_cmdQuit;
}

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