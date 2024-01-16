#include <stdlib.h> // for srand, rand, malloc, free
#include <time.h>   // for srand, rand

#include "durModel.h"
#include "durView.h"
#include "durControl.h"

static sBoard *newBoard() {
    sBoard *b = malloc(sizeof(sBoard));
    b->score[ep_left] = 0;
    b->score[ep_right] = 0;
    for (int i = 0; i < ed_cards; ++i) b->desk.card[i] = i; //desk filling
    b->winner = ep_unknown;
    b->stage = es_newGame;
    return b;
}
static void newGame_deskShuffle(sDesk *d) {
    srand((int)time(NULL));
    int flReshuffle;
    d->number = ed_cards - ed_players * ed_normal; //after first dealing
    do {
        flReshuffle = 0;
        for (int i = 0; i < ed_cards; ++i) {
            int r = rand() % ed_cards;
            int depot = d->card[r];
            d->card[r] = d->card[i];
            d->card[i] = depot;
        }
        //test (if there are more than 4 same suit in one hand - Reshuffle)
        int cases [ed_players * ed_suits] = {0,0,0,0,0,0,0,0}; //for 2 hands
        int base = 0;
        for (int i = d->number; i < ed_cards; ++i) { //test the cards intended for first dealing
            if (i == ed_cards - ed_normal) base = 4;
            if (++cases[d->card[i] / ed_ranks + base] > 4) { //more than 4 card of the same suit
                flReshuffle = 1;
                break;
            }
        }
    } while (flReshuffle);
    d->trump = d->card[0] / ed_ranks;
}
static void newGame_deskPlace(sDesk *d) { //and first dealing (left, right)
    int i = 0;
    for (; i < d->number; ++i)           d->place[i] = ep_desk;
    for (; i < d->number+ed_normal; ++i) d->place[i] = ep_left;
    for (; i < ed_cards; ++i)            d->place[i] = ep_right;
}
static void newGame_playersTurn(sBoard *b) {
    if(b->winner == ep_unknown) { //drawing (first or drawn game)
        srand((int)time(NULL));
        b->attacker = (rand() % ed_players) ? ep_right : ep_left;
    } else {
        b->attacker = b->winner;
    }
    b->dealer = b->attacker;
}
static void newGame(sBoard *b) {
    newGame_deskShuffle(&b->desk);
    newGame_deskPlace(&b->desk);
    newGame_playersTurn(b);
    b->history.number = 0;
    b->stage = es_newFight;
}

static void history(sHistory *h, int desk, ep place) {
    h->desk[h->number] = desk;
    h->place[h->number++] = place;
}
static void newFight_Dealing(sBoard *b) {
    if (b->desk.number > 0) {
        int dealer = b->dealer;
        for (int p = 0; p < ed_players; ++p) {
            int count = 0;
            for (int i = b->desk.number; i < ed_cards; ++i) if (b->desk.place[i] == dealer) ++count;
            while (b->desk.number > 0 & count++ < ed_normal) {
                b->desk.place[--b->desk.number] = dealer;
                history(&b->history, b->desk.number, dealer);
            }
            dealer != dealer;
        }
        b->dealer = b->attacker;
    }
}
static void newFight(sBoard *b) {
    newFight_Dealing(b);
    b->history.frame = b->history.number;
    b->stage = es_attackView;
}

void durModel(sBoard *b) {
    switch (b->stage) {
    case es_newGame:
        newGame(b);
    //break;
    case es_newFight:
        newFight(b);
        durDbgViewBoard(b); //dbg
    //break;
//    case es_fight:
//       //fight(g);
    //break;
    }
}

void dur() {
    sBoard *b1 = newBoard();
    durModel(b1);
    durView(b1);
    durControl(b1);
    free(b1);
}

