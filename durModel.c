#include <stdlib.h> // for srand, rand, malloc, free
#include <time.h>   // for srand, rand

#include "durModel.h"
#include "durView.h"
#include "durControl.h"

#include <stdio.h>

static sBoard *newBoard() {
    sBoard *b = malloc(sizeof(sBoard));
    for (int i = 0; i < ed_cards; ++i) b->desk.card[i] = i; //desk filling
    b->left.score = 0;
    b->right.score = 0;
    b->left.place = ep_left;
    b->right.place = ep_right;
    b->winner = NULL;
    b->stage = es_newGame;
    return b;
}

static void newGame_deskShuffle(sDesk *d) {
    srand((int)time(NULL));
    int flReshuffle;
    d->count = ed_cards;
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
        int base = 0; //left
        for (int i = ed_talon; i < ed_cards; ++i) { //test the cards intended for first dealing
            if (i == ed_talon + ed_normal) base = 4;
            if (++cases[d->card[i] / ed_ranks + base] > 4) { //more than 4 card of the same suit
                flReshuffle = 1;
                break;
            }
        }
    } while (flReshuffle);
    d->trump = d->card[0] / ed_ranks;
}
static void newGame_playersTurn(sBoard *b) {
    if(b->winner == NULL) { //drawing (first or drawn game)
        srand((int)time(NULL));
        b->attacker = (rand() % ed_players) ? &b->right : &b->left;
    } else {
        b->attacker = b->winner;
    }
    b->dealer = b->attacker;
}
static void newGame(sBoard *b) {
    newGame_deskShuffle(&b->desk);
    b->left.count = 0;
    b->right.count = 0;
    newGame_playersTurn(b);
    b->history.count = 0;
    b->stage = es_newFight;
}

static void history(sHistory *h, int desk, ep place) {
    h->desk[h->count] = desk;
    h->place[h->count++] = place;
}
static void newFight_Dealing(sBoard *b, sPlayer *dealer) {
    while (b->desk.count > 0 & dealer->count < ed_normal) {
        dealer->desk[dealer->count++] = --b->desk.count;
        history(&b->history, b->desk.count, dealer->place);
    }
}
static void newFight(sBoard *b) {
    newFight_Dealing(b, b->dealer);
    newFight_Dealing(b, (b->dealer == &b->right) ? &b->left : &b->right);
    b->dealer = b->attacker;
    b->attack.count = 0;
    b->defend.count = 0;
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
//    //break;
////    case es_fight:
////       //fight(g);
//    //break;
    }
}

void dur() {
    sBoard *b1 = newBoard();
    durModel(b1);
    durView(b1);
    durControl(b1);
    free(b1);
}

