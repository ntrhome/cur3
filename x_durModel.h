#ifndef DUR_DURMODEL_H
#define DUR_DURMODEL_H

#include <stdbool.h>

#define DUR_DEBUG

typedef enum ED { ed_players=2, ed_suits=4, ed_ranks = 9, ed_normal = 6, ed_talon = ed_suits * ed_ranks - ed_players * ed_normal, ed_cards = ed_suits * ed_ranks } ed;
typedef enum ES { es_newGame, es_newGameView, es_newFight, es_newFightView, es_attack, es_attackView, es_attackControl, es_attackResult, es_attackResultEnoughView, es_defend, es_defendView, es_defendControl, es_defendResult, es_defendResultTookView, es_fightCloseAsDefended, es_fightCloseAsTook, es_cmdQuit = 1001, es_cmd_enough, es_cmd_take, es_cmd_newGame, es_cmd_wrong } es;
typedef enum EP { ep_left=0, ep_right=1, ep_attack, ep_defend, ep_pile, ep_desk, ep_unknown } ep;

typedef struct {
    int count;
    int card     [ed_cards]; //card[position] = card      - какая карта в заданной позиции?
    int position [ed_cards]; //position[card] = position  - какая позиция у заданной карты?
    int trump;
} sDesk;

typedef struct {
    int count;
    bool hold [ed_cards]; // ..x...x.......x.x.
    ep place;
    int score;
} sPlayer;

typedef struct {
    int count;
    int card [ed_normal];
} sFight;

typedef struct {
    int count;
    int card  [300];
    ep  place [300];
} sHistory;

typedef struct {
    sDesk desk;
    sPlayer left, right;
    sPlayer *winner, *attacker, *defender, *dealer; //roles
    sFight attack;
    sFight defend;
    sHistory history;
    es stage;
    es cmd;
} sBoard;

void dur();

#endif //DUR_DURMODEL_H