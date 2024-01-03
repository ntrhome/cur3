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
    for (int i = 0; i < DUR_CARDS; ++i) { //desk filling
        m->game.round.desk.card[i] = i;
        m->game.round.desk.place[i] = DUR_DESK; //dbg
    }
    m->game.round.desk.head = DUR_CARDS; //dbg
    m->game.round.desk.trump = m->game.round.desk.card[0] / DUR_RANKS; //dbg
    m->game.winner = DUR_NONE;
}

static void newGame(dur_s_game *g) {
    // - - - - - - - - - - - - - - Жеребьевка (первая игра или ничья)
    if(g->winner == DUR_NONE) {
        srand((int)time(NULL));
        g->winner = rand() % DUR_PLAYERS;
    }
    g->round.attacker =  g->winner; //ресет атакующего и первого берущего карты
    g->round.dealer   = !g->winner;
    // - - - - - - - - - - - - - - desk shuffle, reset place, head, set trump
    srand((int)time(NULL));
    for (int i = 0; i < DUR_CARDS; ++i) {
        int depot = g->round.desk.card[i];
        int iRand = rand() % DUR_CARDS;
        g->round.desk.card[i] = g->round.desk.card[iRand];
        g->round.desk.card[iRand] = depot;
        g->round.desk.place[i] = DUR_DESK;
    }
    g->round.desk.head = DUR_CARDS;
    g->round.desk.trump = g->round.desk.card[0] / DUR_RANKS;
    g->round.history.count = 0; //reset history
    g->round.player[DUR_WHITE].count = 0;
    g->round.player[DUR_BLACK].count = 0;
}

static void newRoundDealing(dur_s_round *r, int player) {
    while((r->desk.head > 0) & (r->player[player].count < DUR_NORMAL)) {
        --r->desk.head; //desk 2 player
        r->player[player].desk[r->player[player].count] = r->desk.head;
        ++r->player[player].count;
        r->desk.place[r->desk.head] = player; //desk 2 place
        r->history.desk[r->history.count] = r->desk.head; //desk 2 history
        r->history.place[r->history.count] = player;
        ++r->history.count;
    }
}

static void newRound(dur_s_round *r) { //r->desk.head
    newRoundDealing(r,  r->dealer);
    newRoundDealing(r, !r->dealer);
    r->dealer = r->attacker; //в следующем раунде первым берет аттакер этого раунда
    if(r->player[r->attacker].count == 0) {
        if(r->player[!r->attacker].count == 0) { //gameOver, ничья

        } else { //gameOver, победа r->attacker

        }
        //todo: gameOver
    } else if(r->player[!r->attacker].count == 0) {//gameOver, победа !r->attacker
        //todo: gameOver
    }
    //todo: game



}



static void croupier(dur_s *d) {
    switch(d->match.stage) {
        case DUR_STAGE_NEW_MATCH:
            newMatch(&d->match);
            d->match.stage = DUR_STAGE_NEW_GAME;
            //admin(d);
            //break; //dbg
        case DUR_STAGE_NEW_GAME:
            newGame(&d->match.game);
            d->match.stage = DUR_STAGE_NEW_ROUND;
            //break;
        case DUR_STAGE_NEW_ROUND:
            newRound(&d->match.game.round);
            //d->match.stage = DUR_STAGE_FIRE;
            admin(d);
            //break;
//        case DUR_STAGE_FIRE:
//            //fire(&d->match.game.round.fire);
//            break;
    }

}

void dur() {
    dur_s* d1 = malloc(sizeof(dur_s));
    d1->match.stage = DUR_STAGE_NEW_MATCH;
    croupier(d1); //todo: все dn надо сохранить в dur_s (список, realloc) и авообрабатывать подряд в croupier
    free(d1);
}
