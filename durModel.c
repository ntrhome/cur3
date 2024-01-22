#include <stdlib.h> //for srand, rand, malloc, free
#include <time.h>   //for srand

#include "durModel.h"
#include "durView.h"
#include "durControl.h"

static sBoard *newBoard() {
    sBoard *b = malloc(sizeof(sBoard));
    for (int position = 0; position < ed_cards; ++position) b->desk.card[position] = position; //cards filling
    b->left.place = ep_left;
    b->left.score = 0;
    b->right.place = ep_right;
    b->right.score = 0;
    b->winner = NULL;
    b->stage = es_newGame;
    return b;
}

static void newGame_shuffleDesk(sDesk *d) { //return trump (suits of trump)
    srand((int)time(NULL));
    d->count = ed_cards;
    int flReshuffle;
    do { //cards shuffle
        flReshuffle = 0;
        for (int position = 0; position < ed_cards; ++position) {
            int r;
            int depot = d->card[r = rand() % ed_cards];
            d->card[r] = d->card[position];
            d->card[position] = depot;
        }
        int suit[ed_suits]; //test (if there are more than 4 same suits in one hand - Reshuffle)
        for (int position = ed_talon; position < ed_cards; ++position) { //test the cards intended for first dealing
            if (position == ed_talon || position == ed_talon + ed_normal) { suit[0]=0; suit[1]=0; suit[2]=0; suit[3]=0; }
            if (++suit[d->card[position] / ed_ranks] > 4) { //more than 4 card of the same suits
                flReshuffle = 1;
                break;
            }
        }
    } while (flReshuffle);
    for (int card = 0; card < ed_cards; ++card) { //index filling
        for (int position = 0; position < ed_cards; ++position) {
            if (d->card[position] == card) {
                d->index[card] = position;
                break;
            }
        }
    }
    d->trump = d->card[0] / ed_ranks; //suits of trump
}
static void newGame_setRoles(sBoard *b) {
    if(b->winner == NULL) { //drawing (first or drawn game)
        srand((int)time(NULL));
        b->attacker = (rand() % ed_players) ? &b->right : &b->left;
    } else {
        b->attacker = b->winner;
    }
    b->defender = (b->attacker == &b->left) ? &b->right : &b->left;
    b->dealer = &b->left; //b->attacker; //to reduce history it possible to assign permanently &b->left (with implementation of first dealing)
}
static void newGame(sBoard *b) {
    newGame_shuffleDesk(&b->desk);
    newGame_setRoles(b);
    b->left.count = 0;
    b->right.count = 0;
    b->history.count = 0;
    b->stage = es_newFight;
}

static void history(sHistory *h, int card, ep place) {
    h->card[h->count] = card;
    h->place[h->count++] = place;
}
static void newFight_dealing(sBoard *b) {
    while (b->dealer->count < ed_normal && b->desk.count > 0) {
        b->dealer->card[b->dealer->count++] = b->desk.card[--b->desk.count];
        history(&b->history, b->desk.card[b->desk.count], b->dealer->place);
    }
}
static void newFight(sBoard *b) {
    newFight_dealing(b);
    b->dealer = (b->dealer == &b->left) ? &b->right : &b->left;
    newFight_dealing(b);
    b->dealer = b->attacker; //for next dealing
    b->attack.count = 0;
    b->defend.count = 0;
    b->stage = es_attack;
}

static void attack(sBoard *b) {
    //cheks
    b->stage = es_attackView;
}

void durModel(sBoard *b) {
    switch (b->stage) {
    case es_newGame:
        newGame(b);
//        break;
    case es_newFight:
        newFight(b);
//        break;
    case es_attack:
        attack(b);
        break;
//    case es_cmd_quit:
//        ;
//        exit(0);
    }
//    durView_dbg_board(b); //dbg
}

void dur() {
    sBoard *b1 = newBoard();
    durModel(b1);
    durView(b1);
    durControl(b1);
    free(b1);
}
