#ifndef DUR_DURMODEL_H
#define DUR_DURMODEL_H

#define DUR_DEBUG

typedef enum ED { ed_players=2, ed_suits=4, ed_ranks = 9, ed_normal = 6, ed_cards = ed_suits * ed_ranks } ed;
typedef enum ES { es_newGame, es_newFight, es_fight } es;
typedef enum EP { ep_left=0, ep_right=1, ep_attack1,  ep_defend1} ep;

typedef struct {
    int card [ed_cards];
    int count;
    int trump;
} sDesk;

typedef struct {
    ep place;
    char *name;
    int score;
    int desk [ed_cards];
    int count;
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
}sBoard;

void dur();

#endif //DUR_DURMODEL_H
