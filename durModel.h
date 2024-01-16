#ifndef DUR_DURMODEL_H
#define DUR_DURMODEL_H

#define DUR_DEBUG

typedef enum ED { ed_players=2, ed_suits=4, ed_ranks = 9, ed_normal = 6, ed_cards = ed_suits * ed_ranks } ed;
typedef enum ES { es_newGame, es_newFight, es_attackView, es_attackControl, es_attackResult, es_defendView, es_defendControl, es_defendResult } es;
typedef enum EP { ep_left=0, ep_right=1, ep_1attack, ep_2attack, ep_3attack, ep_4attack, ep_5attack, ep_6attack, ep_1defend, ep_2defend, ep_3defend, ep_4defend, ep_5defend, ep_6defend, ep_pile, ep_desk, ep_null } ep;

typedef struct {
    int card [ed_cards];
    int count;
    int trump;
} sDesk;

typedef struct {
    int desk [ed_cards];
    int count;
    int score;
} sPlayer;

typedef struct {
    int desk [ed_cards];
    int count;
} sFight;

typedef struct {
    int desk [ed_cards];
    ep place [ed_cards];
    int count;
} sHistory;

typedef struct {
    sDesk desk;
    sPlayer left, right;
    sPlayer *winner, *attacker, *dealer;
    sFight attack;
    sFight defend;
    sHistory history;
    es stage;
    //ep nextPlace;
}sBoard;

void dur();

#endif //DUR_DURMODEL_H
