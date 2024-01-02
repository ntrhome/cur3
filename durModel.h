#ifndef DUR_DURMODEL_H
#define DUR_DURMODEL_H

#define DUR_NONE        (-5)
#define DUR_PLAYERS     2
#define DUR_HISTORY_MAX 300
#define DUR_RANKS       9
#define DUR_SUITS       4
#define DUR_CARDS       (DUR_RANKS * DUR_SUITS)
#define DUR_NORMAL      6

typedef enum DUR_STAGE {DUR_STAGE_NEW_MATCH, DUR_STAGE_NEW_GAME, DUR_STAGE_NEW_ROUND, DUR_STAGE_FIRE} dur_stage;
typedef enum DUR_PLACE {DUR_DESK=-1, DUR_WHITE, DUR_BLACK, DUR_ATTACK_0, DUR_DEFEND_0, DUR_ATTACK_1, DUR_DEFEND_1, DUR_ATTACK_2, DUR_DEFEND_2, DUR_ATTACK_3, DUR_DEFEND_3, DUR_ATTACK_4, DUR_DEFEND_4, DUR_ATTACK_5, DUR_DEFEND_5, DUR_PILE} dur_place;

typedef struct DUR_S_DESK {
    int card [DUR_CARDS];
    dur_place place[DUR_CARDS];
    int count;
    int trump;
} dur_s_desk;

typedef struct DUR_S_HISTORY {
    int desk [DUR_HISTORY_MAX]; //id of the card from the "desk"
    dur_place place[DUR_HISTORY_MAX];
    int count;
} dur_s_history;

typedef struct DUR_S_FIRE { //один удар: атакующий/отбивающий
    dur_s_desk desk;
    dur_s_history history;
} dur_s_fire;

typedef struct DUR_S_ROUND { //одна полная баталия - от выдачи карт до выдачи карт
    int dealer;
    int attacker;
    dur_s_fire fire;
} dur_s_round;

typedef struct DUR_S_GAME {
    int winner;
    dur_s_round round;
} dur_s_game;

typedef struct DUR_S_MATCH {
    int score[DUR_PLAYERS];
    dur_s_game game;
    dur_stage stage;
} dur_s_match;

typedef struct DUR_S {
    dur_s_match match;
} dur_s;

void dur();

#endif //DUR_DURMODEL_H
