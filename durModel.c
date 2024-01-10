#include <stdlib.h> // for srand, rand, malloc, free
#include <time.h>   // for srand, rand

#include "durModel.h"
#include "durView.h"
#include "durControl.h"



static void newMatch_deskInit (sDesk *d) {
    for (int i = 0; i < ed_cards; ++i) {
        d->card[i] = i;
    }
}
static void newMatch_playersInit (sGame *g, char *leftName, char *rightName) {
    g->left.place = ep_left;
    g->left.name = leftName;
    g->left.score = 0;
    g->right.place = ep_right;
    g->right.name = rightName;
    g->right.score = 0;
    g->winner = NULL; //nullptr //attacker,dealer - rely on C (because needs for dbg only)
}
static sGame *newMatch(char *leftName, char *rightName) {
    sGame *g = malloc(sizeof(sGame));
    newMatch_deskInit(&g->desk);
    newMatch_playersInit(g, leftName, rightName);
    g->stage = es_newGame;
    return g;
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
        int cases [2 * ed_suits] = {0,0,0,0,0,0,0,0};
        int base = 0;
        for (int i = ed_cards - 2 * ed_normal; i < ed_cards; ++i) {
            if(i == ed_cards - ed_normal) base = 4;
            ++cases[d->card[i] / ed_ranks + base];
        }
        for (int c = 0; c < 2 * ed_suits; ++c) {
            if(cases[c] > 4) {
                flReshuffle = 1;
                break;
            }
        }
    } while (flReshuffle);
    d->trump = d->card[0] / ed_ranks;
    d->count = ed_cards;
}
static void newGame_playersTurn(sGame *g) {
    if(g->winner == NULL) { //drawing (first or drawn game)
        srand((int)time(NULL));
        g->attacker = (rand() % ed_players) ? &g->right : &g->left;
    } else {
        g->attacker = g->winner;
    }
    g->dealer = g->attacker;
}
static void newGame(sGame *g) {
    newGame_deskShuffle(&g->desk);
    g->left.count = 0;
    g->right.count = 0;
    newGame_playersTurn(g);
    g->history.count = 0;
    //g->stage = es_newFight;
}

//static sPlayer *other(sGame *g, sPlayer *one) {
//    return (one->place) ? &g->left : &g->right;
//}
static void newFight_Dealing(sGame *g,  sPlayer *p) {
    while(g->desk.count > 0 & p->count < ed_normal) {
        p->desk[p->count++] = --g->desk.count;
        g->history.desk[g->history.count] = g->desk.count;
        g->history.place[g->history.count++] = p->place;
    }
}
static void newFight(sGame *g) {
    newFight_Dealing(g, g->dealer);
    newFight_Dealing(g, (g->dealer->place) ? &g->left : &g->right);

    g->attack.count = 0;
    g->defend.count = 0;
    //g->stage = es_fight;
}

static void fight_defended(sGame *g) {}
static void fight_attack(sGame *g) {
    // todo: далее ориентир на виев/контрол - ставим подробный стейдж (атака-виев), по которому поток виева отрпортует своим стейджем (атака-виев-показано).
    // Теперь ставим контрол-стейдж (атака-контрол), ожидаем рапорт контрол-потока (атака-контрол-получено) - контрол сам проверяет ввод на осмысленность/приемлемость и выдает полученное.
    // Далее прописываем это дело в историю (надо, ибо учебный анализ ходов) и в файт, и переходим к дефенду.
    // Попутно анализируем наличие карт и команды Qq (Tt), перепрыгивая по ситуации в завершающие файт/гейм стейджи.
}
static void fight_defend(sGame *g) {}
static void fight_defeat(sGame *g) {}

static void fight(sGame *g) {
    if(g->defend.count == ed_normal) {
        fight_defended(g);
    } else if(g->attack.count == g->defend.count) {
        fight_attack(g);
    } else {
        fight_defend(g);
    }
}

void croupier(sGame *g) {
    switch (g->stage) {
    case es_newGame:
        newGame(g);
    //break;
    case es_newFight:
        newFight(g);
    //break;
    case es_fight:
       fight(g);
        vg(g);
    //break;


    }
}

void dur() {
    sGame *g1 = newMatch("Left", "Right");
    croupier(g1);


   free(g1);
}

