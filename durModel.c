#include <stdlib.h> // for srand, rand, malloc, free
#include <time.h>   // for srand, rand

#include "durModel.h"
#include "durView.h"

static void history(dur_s_history *h, int desk, int place) {
    if(h->count < DUR_HISTORY_MAX) {
        h->desk [h->count] = desk;
        h->place[h->count] = place;
        ++h->count;
    }
}

static void newMatch(dur_s_match *m) {
    m->score[DUR_WHITE] = 0;
    m->score[DUR_BLACK] = 0;
    for (int i = 0; i < DUR_CARDS; ++i) {
        m->game.round.fire.desk.card[i] = i;
    }
    m->game.winner = DUR_NONE;
    m->stage = DUR_STAGE_NEW_GAME;
}

static void newGame(dur_s_game *g) {
    // - - - - - - - - - - - - - - Жеребьевка (первая игра или ничья)
    if(g->winner == DUR_NONE) {
        srand((int)time(NULL));
        g->winner = rand() % DUR_PLAYERS;
    }
    g->round.attacker =  g->winner; //ресет атакующего и первого берущего карты
    g->round.dealer   = !g->winner;
    // - - - - - - - - - - - - - - desk shuffle, reset place
    srand((int)time(NULL));
    for (int i = 0; i < DUR_CARDS; ++i) {
        int depot = g->round.fire.desk.card[i];
        int iRand = rand() % DUR_CARDS;
        g->round.fire.desk.card[i] = g->round.fire.desk.card[iRand];
        g->round.fire.desk.card[iRand] = depot;
        g->round.fire.desk.place[i] = DUR_DESK;
    }
    g->round.fire.history.count = 0;
    g->round.fire.desk.trump = g->round.fire.desk.card[0] / DUR_RANKS;
    g->round.fire.desk.count = DUR_CARDS; //reset history
}

static void newRound(dur_s_round *r) {
    int count[DUR_PLAYERS] = {0, 0};
    int i = DUR_CARDS - 1;
    for (; i >= 0; --i) {
        if(r->fire.desk.place[i] == DUR_DESK) break;
        if(r->fire.desk.place[i] == DUR_WHITE | r->fire.desk.place[i] == DUR_BLACK) {
            ++count[r->fire.desk.place[i]];
        }
    }
    int turn [DUR_PLAYERS] = {r->dealer, !r->dealer};
    for (int t = 0; t < DUR_PLAYERS; ++t) {
        while((count[turn[t]] < DUR_NORMAL) & (i >= 0)) {
            r->fire.desk.place[i] = turn[t];
            history(&r->fire.history, i, turn[t]);
            ++count[turn[t]];
            --i;
            --r->fire.desk.count;
        }
    }
    r->dealer = r->attacker;
}

static void fire(dur_s_fire *f) {

}

static void croupier(dur_s *d) {
    switch(d->match.stage) {
        case DUR_STAGE_NEW_MATCH:
            newMatch(&d->match);
            d->match.stage = DUR_STAGE_NEW_GAME;
            //break;
        case DUR_STAGE_NEW_GAME:
            newGame(&d->match.game);
            d->match.stage = DUR_STAGE_NEW_ROUND;
            //break;
        case DUR_STAGE_NEW_ROUND:
            newRound(&d->match.game.round);
            d->match.stage = DUR_STAGE_FIRE;
            //break;
        case DUR_STAGE_FIRE:
            fire(&d->match.game.round.fire);
            //break;
    }
    admin(&d->match);
}

void dur() {
    dur_s* d = malloc(sizeof(dur_s));
    d->match.stage = DUR_STAGE_NEW_MATCH;
    croupier(d);
    free(d);
}
