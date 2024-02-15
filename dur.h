#pragma once

#define DUR_DEBUG

typedef enum ED { ed_players=2, ed_suits=4, ed_ranks = 9, ed_cards = ed_suits * ed_ranks, ed_normal = 6, ed_talon = ed_cards - ed_players * ed_normal } ed;
typedef enum ES { es_newGame, es_newGameView, es_newFight, es_newFightView, es_attack, es_attackView, es_attackControl, es_attackResult, es_defend, es_cmdQuit = -100, es_cmdEnough, es_cmdTake, es_cmdWrong, es_cmdNewGame } es; //es_attackView, es_attackControl, es_attackResult, es_attackResultEnoughView, es_defend, es_defendView, es_defendControl, es_defendResult, es_defendResultTookView, es_fightCloseAsDefended, es_fightCloseAsTook, es_cmdQuit = 1001, es_cmd_enough, es_cmd_take, es_cmd_newGame, es_cmd_wrong
typedef enum EP { ep_left=0, ep_right=1, ep_attack, ep_defend, ep_pile, ep_desk} ep;

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
    es stage;
    int game; //game N
    sDesk desk;
    sPlayer left, right;
    sPlayer *winner, *attacker, *defender, *dealer; //roles
    int fight; //cards count in fight
    sHistory history;
    es cmd;
} sMatch;

sMatch *durNewMatch();
