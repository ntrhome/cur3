#ifndef DUR_DURMODEL_H
#define DUR_DURMODEL_H

#define DUR_NONE        (-5)
#define DUR_PLAYERS     2
#define DUR_HISTORY_MAX 300
#define DUR_RANKS       9
#define DUR_SUITS       4
#define DUR_CARDS       (DUR_RANKS * DUR_SUITS)

typedef enum dur_e_stage {DUR_E_STAGE_MATCH_INIT, DUR_E_STAGE_MATCH, DUR_E_STAGE_GAME_INIT, DUR_E_STAGE_GAME, DUR_E_STAGE_BATTLE_INIT, DUR_E_STAGE_BATTLE, DUR_E_STAGE_FIGHT_ATTACK, DUR_E_STAGE_FIGHT_DEFEND} DUR_E_STAGE;
typedef enum dur_e_place {DUR_E_PLACE_DESK=-1, DUR_E_PLACE_PLAYER_0, DUR_E_PLACE_PLAYER_1, DUR_E_PLACE_ATTACK_0, DUR_E_PLACE_DEFEND_0, DUR_E_PLACE_ATTACK_1, DUR_E_PLACE_DEFEND_1, DUR_E_PLACE_ATTACK_2, DUR_E_PLACE_DEFEND_2, DUR_E_PLACE_ATTACK_3, DUR_E_PLACE_DEFEND_3, DUR_E_PLACE_ATTACK_4, DUR_E_PLACE_DEFEND_4, DUR_E_PLACE_ATTACK_5, DUR_E_PLACE_DEFEND_5, DUR_E_PLACE_PILE} DUR_E_PLACE;

typedef struct DUR_S_DESK {
    int card [DUR_CARDS];
    int place[DUR_CARDS];
    int count;
} dur_s_desk;

typedef struct DUR_S_HISTORY {
    int desk [DUR_HISTORY_MAX]; //id of the card from the "desk"
    int place[DUR_HISTORY_MAX];
    int count;
} dur_s_history;

typedef struct DUR_S_FIELD {
    dur_s_desk desk;
    dur_s_history history;
} dur_s_field;

typedef struct DUR_S_BATTLE {
    int dealer;   //first dealing player
    int attacker;
    dur_s_field field;
} dur_s_battle;

typedef struct DUR_S_GAME {
    int winner;
    dur_s_battle battle;
} dur_s_game;

typedef struct DUR_S_MATCH {
    int stage;
    int score[DUR_PLAYERS];
    dur_s_game game;
} dur_s_match;

void croupier(dur_s_match *m);





#endif //DUR_DURMODEL_H


