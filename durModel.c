#include <stdlib.h> // for srand, rand, malloc, free
#include <time.h>   // for srand, rand

#include "durModel.h"
#include "durView.h"

static void history(dur_s_history *h, int desk, int place) {
    if(h->count < DUR_HISTORY_MAX) {
        h->desk [h->count  ] = desk;
        h->place[h->count++] = place;
    }
}

static void newMatch(dur_s_game *g) { //reset: score, winner, desk
    g->score[DUR_WHITE] = 0;
    g->score[DUR_BLACK] = 0;
    g->winner = DUR_NONE;
    for (int i = 0; i < DUR_CARDS; ++i) { g->round.field.desk.card[i] = i; } //desk filling
    g->round.field.stage = DUR_STAGE_NEW_GAME;
}

static void newGame_AttackerDealer(dur_s_game *g) { //set: attacker, dealer
    if(g->winner == DUR_NONE) { //жеребьевка (первая игра или ничья)
        srand((int)time(NULL));
        g->round.attacker = rand() % DUR_PLAYERS;
    } else {
        g->round.attacker =  g->winner;
    }
    g->round.dealer   = !g->round.attacker;
}

static void newGame_DeskShuffle(dur_s_desk *d) { //set desk
    srand((int)time(NULL));
    for (int i = 0; i < DUR_CARDS; ++i) {
        int iRand = rand() % DUR_CARDS;
        int depot = d->card[iRand];
        d->card[iRand] = d->card[i];
        d->card[i] = depot;
        d->place[i] = DUR_DESK;
    }
    d->head = DUR_CARDS;
    d->trump = d->card[0] / DUR_RANKS;
}

static void newGame(dur_s_game *g) { //set: (attacker, dealer), (desk); reset: players, history
    newGame_AttackerDealer(g);
    newGame_DeskShuffle(&g->round.field.desk);
    g->round.field.player[DUR_WHITE].count = 0; //reset player 0
    g->round.field.player[DUR_BLACK].count = 0; //reset player 1
    g->round.field.history.count = 0;           //reset history
    g->round.field.stage = DUR_STAGE_NEW_ROUND;
}

static void newRound_Dealing(dur_s_field *f, int p) {
    while((f->desk.head > 0) & (f->player[p].count < DUR_NORMAL)) {
        f->player[p].desk[f->player[p].count++] = --f->desk.head; //desk 2 player
        f->desk.place[f->desk.head] = p;                          //desk 2 place
        history(&f->history, f->desk.head, p);      //desk 2 history
    }
}

static void newRound(dur_s_round *r) { //r->desk.head
    newRound_Dealing(&r->field,  r->dealer);
    newRound_Dealing(&r->field, !r->dealer);
    r->dealer = r->attacker; //в следующем раунде первым берет аттакер этого раунда
    r->field.historyField = r->field.history.count; //reset field frame
//    if(r->field.player[r->attacker].count == 0) { //на начало раунда у атакера нет карт
//        if(r->player[!r->attacker].count == 0) { //gameOver, ничья
//
//        } else { //gameOver, победа r->attacker
//
//        }
//        //todo: gameOver
//    } else if(r->player[!r->attacker].count == 0) {//gameOver, победа !r->attacker
//        //todo: gameOver
//    }
//    //todo: game
}

static void croupier(dur_s_game *g) {
    switch(g->round.field.stage) {
        case DUR_STAGE_NEW_MATCH:
            newMatch(g);
            //break;
        case DUR_STAGE_NEW_GAME:
            newGame(g);
            //break;
        case DUR_STAGE_NEW_ROUND:
            newRound(&g->round);
            admin(g);
            //break;
//        case DUR_STAGE_FIRE:
//            //fire(&d->match.game.round.fire);
//            break;
    }
}

void dur() {
    dur_s_game* g1 = malloc(sizeof(dur_s_game));
    g1->round.field.stage = DUR_STAGE_NEW_MATCH;
    croupier(g1); //todo: все dn надо сохранить в dur_s (список, realloc) и авообрабатывать подряд в croupier
    free(g1);
}
