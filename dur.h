#pragma once
#include "main.h"
#define DUR_DEBUG
typedef enum ED { ed_players=2, ed_suits=4, ed_ranks = 9, ed_cards = ed_suits * ed_ranks, ed_normal = 6, ed_talon = ed_cards - ed_players * ed_normal } ed;
typedef enum EP { ep_left=0, ep_right=1, ep_fight, ep_pile, ep_desk} ep;
typedef enum ES { es_gameModel,  es_gameView,
                  es_fightModel, es_fightView,

                  es_cmdQuit
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
    es stage;
    es cmd;
} sMatch;
