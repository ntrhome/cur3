#ifndef DUR_DUR_H
#define DUR_DUR_H

#include <stdbool.h>
#define DUR_DEBUG

typedef enum ED { ed_players=2, ed_suits=4, ed_ranks = 9, ed_cards = ed_suits * ed_ranks, ed_normal = 6, ed_talon = ed_cards - ed_players * ed_normal } ed;
typedef enum ES { es_newGame, es_newFight, es_attack, es_defend, es_cmd_quit = 1001 } es; //es_attackView, es_attackControl, es_attackResult, es_attackResultEnoughView, es_defend, es_defendView, es_defendControl, es_defendResult, es_defendResultTookView, es_fightCloseAsDefended, es_fightCloseAsTook, es_cmd_quit = 1001, es_cmd_enough, es_cmd_take, es_cmd_newGame, es_cmd_wrong
typedef enum EP { ep_left=0, ep_right=1, ep_attack, ep_defend, ep_pile, ep_desk} ep;

typedef struct {
    int count;
    int order[ed_cards]; //order[index] - card in given order
    int index[ed_cards]; //index[card] - index of the given card
    ep  place[ed_cards]; //place[card] - place of the given card
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
    int fight; //cards count in fight
    sHistory history;
    int game;
    es stage;
    es cmd;
} sBoard;

void dur();

#endif //DUR_DUR_H
