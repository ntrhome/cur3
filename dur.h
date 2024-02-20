#pragma once
#include "main.h"
#define DUR_DEBUG
typedef enum ED { ed_players=2, ed_suits=4, ed_ranks = 9, ed_cards = ed_suits * ed_ranks, ed_normal = 6, ed_talon = ed_cards - ed_players * ed_normal } ed;
typedef enum EP { ep_left=0, ep_right=1, ep_attack, es_defend, ep_pile, ep_desk} ep;
typedef enum ES { es_gameModel,  es_gameView,
                  es_fightModel,
                  es_attackModel, es_attackView, es_attackControl, es_attackHandler, es_attackHandlerViewWrong, es_attackHandlerViewUnacceptable,
                  es_defendModel,
                  es_cmd = 100,
                  es_cmdQuit, es_cmdWrong, es_cmdEnough, es_cmdTake, es_cmdYes,
                  } es;

typedef struct {
    int count;
    int card[ed_cards];     //card[position] - card in given position
    int position[ed_cards]; //position[card] - position of the given card
    ep  place[ed_cards];    //place[card]    - place of the given card
    int trump;
} sDesk;

typedef struct {
    int count;
    ep place;
    int score;
} sPlayer;

typedef struct {
    int count;
    int card  [300];
    ep  place [300];
} sHistory;

typedef struct {
    sDesk desk;
    sPlayer left, right;
    sPlayer *winner, *attacker, *defender, *dealer; //roles
    sHistory history;
    int fight; //cards count in fight
    int game; //game N
    volatile es state;
    volatile es cmd;
} sMatch;
