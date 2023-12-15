#include <stdlib.h> // for srand, rand
#include <time.h>   // for srand, rand

#include "durModel.h"

static void dataInit(DUR_DATA *d) {
    for (int i = 0; i < DUR_CARDS; ++i) { //desk filling, with appropriate history.place and history.count
        d->game.history.card [i] = i;
        d->game.history.place[i] = DUR_PLACE_DESK;
    }
    d->game.history.count = DUR_CARDS;
    for (int p = 0; p < DUR_PLAYERS; ++p) { //binding field to player and field.count = 0
        d->game.round.player[p].field = &d->game.round.field;
    }
    d->stage = DUR_STAGE_GAMEINIT;
}

static void gameInit(DUR_DATA *d) {
    if(d->winner == DUR_NONE) { //Жеребьевка (первая игра или ничья).
        srand((int)time(NULL));
        d->winner = rand() % DUR_PLAYERS;
        d->game.attacker     =  d->winner;
        d->game.firstDealing = !d->winner;
    }
    srand((int)time(NULL));   //desk shuffle, set head and trump
    for(int i = 0; i < DUR_CARDS; ++i) {
        int depot = d->game.desk.card[i];
        int iRand = rand() % DUR_CARDS;
        d->game.desk.card[i] = d->game.desk.card[iRand];
        d->game.desk.card[iRand] = depot;
    }
    d->game.desk.head  = DUR_CARDS;
    d->game.desk.trump = d->game.desk.card[0] / DUR_RANKS;
    d->stage = DUR_STAGE_ROUNDINIT;
}

static void roundInit(DUR_DATA *d) {
    for (int p = 0; p < DUR_PLAYERS; ++p) {
        d->game.round.player[p].head  = d->game.desk.head;
        d->game.round.player[p].trump = d->game.desk.trump;
        d->game.round.player[p].hand.count = 0;
        for(int i = 0; i < DUR_CARDS; ++i) {
            if(d->game.desk.place[i] == p){
                d->game.round.player[p].hand.id   [d->game.round.player[p].hand.count] = i;
                d->game.round.player[p].hand.cards[d->game.round.player[p].hand.count] = d->game.desk.card[i];
                ++d->game.round.player[p].hand.count;
            }
        }
    }
    int dealingTurn[DUR_PLAYERS] = {d->game.firstDealing, !d->game.firstDealing};
    for (int p = 0; p < DUR_PLAYERS; ++p) {
        while((d->game.round.player[p].hand.count < 6) & (d->game.desk.head > 0)) {
            d->game.history.card [d->game.history.count] = --d->game.desk.head;
            d->game.history.place[d->game.history.count] = p;
            ++d->game.history.count;
            d->game.round.player[dealingTurn[p]].hand.id   [d->game.round.player[p].hand.count] = d->game.desk.head;
            d->game.round.player[dealingTurn[p]].hand.cards[d->game.round.player[p].hand.count] = d->game.desk.card[d->game.desk.head];
            ++d->game.round.player[p].hand.count;
            d->game.desk.place[d->game.desk.head] = p;
        }
    }
    d->game.round.field.count = 0;
    d->stage = DUR_STAGE_ROUNDSTEP;
}

void dur(DUR_DATA *d) {
    switch (d->stage) {
        case DUR_STAGE_DATAINIT:
            dataInit(d);
            break;
        case DUR_STAGE_GAMEINIT:
            gameInit(d);
            break;
        case DUR_STAGE_ROUNDINIT:
            roundInit(d);
            break;
        case DUR_STAGE_ROUNDSTEP:
            break;
    }
}