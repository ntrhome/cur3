#include <stdlib.h> //for srand, rand, malloc, free
#include <time.h>   //for srand
#include "dur.h"

sBoard *newBoard() {
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


static bool isDeskShuffleBad(const sDesk *d) { //true - if there are more than 4 same suits in one hand
    int suit[ed_suits];
    for (int i = ed_talon; i < ed_cards; ++i) { //test the cards intended for first dealing
        if (i == ed_talon || i == ed_talon + ed_normal) { suit[0]=0; suit[1]=0; suit[2]=0; suit[3]=0; }
        if (++suit[d->card[i] / ed_ranks] > 4) return true; //more than 4 card of the same suits
    }
    return false;
}
static void deskShuffle(sDesk *d) {
    srand((unsigned)time(NULL));
    d->count = ed_cards;
    bool flReshuffle;
    do { //cards shuffle
        flReshuffle = false;
        for (int i = 0; i < ed_cards; ++i) {
            int r = rand() % ed_cards;
            int depot = d->card[r]; d->card[r] = d->card[i]; d->card[i] = depot;
        }
        int suit[ed_suits]; //test (if there are more than 4 same suits in one hand - Reshuffle)
        for (int i = ed_talon; i < ed_cards; ++i) { //test the cards intended for first dealing
            if (i == ed_talon || i == ed_talon + ed_normal) { suit[0]=0; suit[1]=0; suit[2]=0; suit[3]=0; }
            if (++suit[d->card[i] / ed_ranks] > 4) { //more than 4 card of the same suits
                flReshuffle = true;
                break;
            }
        }
    } while (flReshuffle);
    for (int i = 0; i < ed_cards; ++i) {
        d->position[d->card[i]] = i;
    }
    d->trump = d->card[0] / ed_ranks; //suits of trump
}
static void dur_newGame_playersReset(sBoard *b) {
    b->left.count = 0;
    b->right.count = 0;
    for (int i = 0; i < ed_cards; ++i) { //player holders clearing
        b->left.hold[i]  = 0;
        b->right.hold[i] = 0;
    }
    if(b->winner == NULL) { //roles :drawing (first or drawn game)
        srand((unsigned)time(NULL));
        b->attacker = (rand() % ed_players) ? &b->right : &b->left;
    } else {
        b->attacker = b->winner;
    }
    b->defender = (b->attacker == &b->left) ? &b->right : &b->left;
    b->dealer = &b->left; //b->attacker; //to reduce history it possible to assign permanently &b->left (with implementation of first dealing)
}
void dur_newGame(sBoard *b) {
    dur_newGame_deskShuffle(&b->desk);
    dur_newGame_playersReset(b);
    b->history.count = 0;
    b->stage = es_newFight;
}

static void history(sHistory *h, int card, ep place) {
    h->card[h->count] = card;
    h->place[h->count++] = place;
}

static void newFight_deskDealing(sBoard *b) {
    sPlayer *p = b->dealer;
    for (int n = 0; n < ed_players; ++n) {
        while (p->count < ed_normal && b->desk.count > 0) {
            p->hold[b->desk.card[--b->desk.count]] = true;
            ++p->count;
            history(&b->history, b->desk.card[b->desk.count], p->place);
        }
        p = (p == &b->left) ? &b->right : &b->left;
    }
}
void dur_newFight(sBoard *b) {
    newFight_deskDealing(b);
    b->dealer = b->attacker; //for next newFight
    b->attack.count = 0; //fight reset
    b->defend.count = 0;
    b->stage = es_attack;
}

//static bool isCmdAsAcceptableCommand(const sBoard *b) {
//
//}

static bool isCmdAsAcceptableCard_isFightHasRank(const sBoard *b) {

}
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