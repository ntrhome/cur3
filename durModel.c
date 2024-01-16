#include <stdlib.h> // for srand, rand, malloc, free
#include <time.h>   // for srand, rand

#include "durModel.h"
#include "durView.h"
#include "durControl.h"



static void newBoard_playersInit (sBoard *b, char *leftName, char *rightName) {
//    b->left.place = ep_left;
    b->left.name = leftName;
    b->left.score = 0;
    b->left.count = 0;
//    b->right.place = ep_right;
    b->right.name = rightName;
    b->right.score = 0;
    b->right.count = 0;
}
static sBoard *newBoard(char *leftName, char *rightName) {
    sBoard *b = malloc(sizeof(sBoard));
    for (int i = 0; i < ed_cards; ++i) b->desk.card[i] = i; //desk filling
    newBoard_playersInit(b, leftName, rightName);
    b->winner = NULL;
    b->stage = es_newGame;
    return b;
}
static void newGame_deskShuffle(sDesk *d) {
    srand((int)time(NULL));
    int flReshuffle;
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
        for (int i = ed_cards - ed_players * ed_normal; i < ed_cards; ++i) {
            if(i == ed_cards - ed_normal) base = 4;
            if(++cases[d->card[i] / ed_ranks + base] > 4) { //more than 4 card of the same suit
                flReshuffle = 1;
                break;
            }
        }
    } while (flReshuffle);
    d->trump = d->card[0] / ed_ranks;
    d->count = ed_cards;
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
static void newGame_playerInit(sPlayer *p, sDesk *d, sHistory *h) {
    d->count -= ed_normal;
    for (int i = 0; i < ed_normal; ++i) {
        p->desk[i] = d->count + i;
        h->desk[i]
    }
}
static void newGame(sBoard *b) {
    newGame_deskShuffle(&b->desk);
    b->left.count = 0;
    b->right.count = 0;
    newGame_playersTurn(b);
    b->history.count = 0;
    b->stage = es_newFight;
}

static void historySave(sHistory *h, int desk, ep place) {
    h->desk[h->count] = desk;
    h->place[h->count++] = place;
}
static void newFight_Dealing(sBoard *b,  sPlayer *p) {
    while(b->desk.count > 0 & p->count < ed_normal) {
        p->desk[p->count++] = --b->desk.count;
        historySave(&b->history, b->desk.count, (p == &b->left) ? ep_left : ep_right);
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

//static void fight_defended(sBoard *b) {}
//static void fight_attack(sBoard *b) {
//    // todo: далее ориентир на виев/контрол - ставим подробный стейдж (атака-виев), по которому поток виева отрпортует своим стейджем (атака-виев-показано).
//    // Теперь ставим контрол-стейдж (атака-контрол), ожидаем рапорт контрол-потока (атака-контрол-получено) - контрол сам проверяет ввод на осмысленность/приемлемость и выдает полученное.
//    // Далее прописываем это дело в историю (надо, ибо учебный анализ ходов) и в файт, и переходим к дефенду.
//    // Попутно анализируем наличие карт и команды Qq (Tt), перепрыгивая по ситуации в завершающие файт/гейм стейджи.
//}
//static void fight_defend(sBoard *b) {}
//static void fight_defeat(sBoard *b) {}

//static void fight(sBoard *b) {
//    if(g->defend.count == ed_normal) {
//        fight_defended(g);
//    } else if(g->attack.count == g->defend.count) {
//        fight_attack(g);
//    } else {
//        fight_defend(g);
//    }
//}

void durModel(sBoard *b) {
    switch (b->stage) {
    case es_newGame:
        newGame(b);
    //break;
    case es_newFight:
        newFight(b);
        durdbgViewBoard(b); //dbg
    //break;
//    case es_fight:
//       //fight(g);

    //break;


    }
}

void dur() {
    sBoard *b1 = newBoard("Left", "Right");
    durModel(b1);
    durView(b1);
    durControl(b1);
    free(b1);
}

