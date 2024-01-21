#ifndef DUR_DURMODEL_H
#define DUR_DURMODEL_H

#define DUR_DEBUG

typedef enum ED { ed_players=2, ed_suits=4, ed_ranks = 9, ed_normal = 6, ed_talon = ed_suits * ed_ranks - ed_players * ed_normal, ed_cards = ed_suits * ed_ranks } ed;
typedef enum ES { es_newGame, es_newFight, es_attack, es_attackView, es_attackControl, es_attackResult, es_defendView, es_defendControl, es_defendResult, es_cmd_take = 1001, es_cmd_wrong, es_cmd_unacceptable, es_cmd_quit } es;
typedef enum EP { ep_left=0, ep_right=1, ep_attack1, ep_attack2, ep_attack3, ep_attack4, ep_attack5, ep_attack6, ep_defend1, ep_defend2, ep_defend3, ep_defend4, ep_defend5, ep_defend6, ep_pile, ep_desk, ep_unknown } ep;

typedef struct {
    int count;
    int card  [ed_cards]; //card[index] -> card
    int index [ed_cards]; //index[card] -> index
} sDesk;

struct structField;
typedef struct structField sField;
typedef struct {
    ep place;
    int score;
    int count;
    int card [ed_cards];
} sPlayer;

typedef struct {
    int count;
    int card [ed_normal];
} sFight;

struct structField {
    int trump;
    sPlayer *winner, *attacker, *dealer, *player;
    sFight attack;
    sFight defend;
};

typedef struct {
    int count;
    int card  [300];
    ep  place [300];
} sHistory;

typedef struct {
    sDesk desk;
    sPlayer left, right;
    sField field;
    sHistory history;
    es stage;
}sBoard;

void dur();

#endif //DUR_DURMODEL_H
