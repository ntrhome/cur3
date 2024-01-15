#include <stdlib.h> // for srand, rand, malloc, free
#include <time.h>   // for srand, rand

#include "durModel.h"
#include "durView.h"
#include "durControl.h"



static void newBoard_deskInit (sDesk *d) {
    for (int i = 0; i < ed_cards; ++i) {
        d->card[i] = i;
    }
}
static void newBoard_playerInit (sPlayer *p, ep place, char *name) {
    p->place = place;
    p->name = name;
    p->score = 0;
}
static sBoard *newBoard(char *leftName, char *rightName) {
    sBoard *b = malloc(sizeof(sBoard));
    newBoard_deskInit(&b->desk);
    newBoard_playerInit(&b->left, ep_left, leftName);
    newBoard_playerInit(&b->right, ep_right, rightName);
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
            if(++cases[d->card[i] / ed_ranks + base] > 4) {
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
static void newGame(sBoard *b) {
    newGame_deskShuffle(&b->desk);
    b->left.count = 0;
    b->right.count = 0;
    newGame_playersTurn(b);
    b->history.count = 0;
    b->stage = es_newFight;
}

static void newFight_Dealing(sBoard *b,  sPlayer *p) {
    while(b->desk.count > 0 & p->count < ed_normal) {
        p->desk[p->count++] = --b->desk.count;
        b->history.desk[b->history.count] = b->desk.count;
        b->history.place[b->history.count++] = p->place;
    }
}
static void newFight(sBoard *b) {
    newFight_Dealing(b, b->dealer);
    newFight_Dealing(b, (b->dealer->place) ? &b->left : &b->right);
    b->attack.count = 0;
    b->defend.count = 0;
    b->stage = es_fight;
}

static void fight_defended(sBoard *b) {}
static void fight_attack(sBoard *b) {
    // todo: далее ориентир на виев/контрол - ставим подробный стейдж (атака-виев), по которому поток виева отрпортует своим стейджем (атака-виев-показано).
    // Теперь ставим контрол-стейдж (атака-контрол), ожидаем рапорт контрол-потока (атака-контрол-получено) - контрол сам проверяет ввод на осмысленность/приемлемость и выдает полученное.
    // Далее прописываем это дело в историю (надо, ибо учебный анализ ходов) и в файт, и переходим к дефенду.
    // Попутно анализируем наличие карт и команды Qq (Tt), перепрыгивая по ситуации в завершающие файт/гейм стейджи.
}
static void fight_defend(sBoard *b) {}
static void fight_defeat(sBoard *b) {}

//static void fight(sBoard *b) {
//    if(g->defend.count == ed_normal) {
//        fight_defended(g);
//    } else if(g->attack.count == g->defend.count) {
//        fight_attack(g);
//    } else {
//        fight_defend(g);
//    }
//}

void croupier(sBoard *b) {
    switch (b->stage) {
    case es_newGame:
        newGame(b);
    //break;
    case es_newFight:
        newFight(b);
    //break;
    case es_fight:
       //fight(g);
        vb(b);
    //break;


    }
}

void dur() {
    sBoard *b1 = newBoard("Left", "Right");
    croupier(b1);
    free(b1);
}

