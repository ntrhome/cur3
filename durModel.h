#ifndef DUR_DURMODEL_H
#define DUR_DURMODEL_H

#define DUR_NONE        (-5)
#define DUR_PLAYERS     2
#define DUR_HISTORY_MAX 300
#define DUR_HAND_NORM   6
#define DUR_RANKS       9
#define DUR_SUITS       4
#define DUR_CARDS       (DUR_RANKS * DUR_SUITS)

typedef enum dur_e_place {DUR_E_PLACE_DESK=-1, DUR_E_PLACE_PLAYER_0, DUR_E_PLACE_PLAYER_1, DUR_E_PLACE_ATTACK_0, DUR_E_PLACE_DEFEND_0, DUR_E_PLACE_ATTACK_1, DUR_E_PLACE_DEFEND_1, DUR_E_PLACE_ATTACK_2, DUR_E_PLACE_DEFEND_2, DUR_E_PLACE_ATTACK_3, DUR_E_PLACE_DEFEND_3, DUR_E_PLACE_ATTACK_4, DUR_E_PLACE_DEFEND_4, DUR_E_PLACE_ATTACK_5, DUR_E_PLACE_DEFEND_5, DUR_E_PLACE_PILE} DUR_E_PLACE;

typedef struct dur_s_field {
    int desk[2 * DUR_HAND_NORM]; //id of the card from the "desk"
    int card[2 * DUR_HAND_NORM];
    int count;
} DUR_S_FIELD;

typedef struct dur_s_desk {
    int card[DUR_CARDS];
    int place[DUR_CARDS];
    int count;
    int trump; //suit
} DUR_S_DESK;

typedef struct dur_s_fight {
    int dealer;   //first dealing player
    int attacker; //fight attack player
    int historyCount;  //fight frame start in history
    DUR_S_FIELD field;
} DUR_S_FIGHT;

typedef struct dur_s_history {
    int desk[DUR_HISTORY_MAX]; //id of the card from the "desk"
    int place[DUR_HISTORY_MAX];
    int count;
} DUR_S_HISTORY;

typedef struct dur_s_game {
    int winner;
    DUR_S_DESK desk;
    DUR_S_FIGHT fight;
    DUR_S_HISTORY history;
} DUR_S_GAME;

typedef enum dur_e_stage {DUR_E_STAGE_MATCH_INIT, DUR_E_STAGE_GAME_INIT, DUR_E_STAGE_FIGHT_INIT, DUR_E_STAGE_FIGHT, DUR_E_STAGE_FIGHT_ATTACK, DUR_E_STAGE_FIGHT_DEFEND} DUR_E_STAGE;

typedef struct dur_s_match {
    int stage;
    int score[DUR_PLAYERS];
    DUR_S_GAME game;
} DUR_S_MATCH;

#define DUR_S_MATCH_INIT {DUR_E_STAGE_MATCH_INIT}

int dur(DUR_S_MATCH *d);





#endif //DUR_DURMODEL_H


// static void matchInit(DUR_S_MATCH *m);
// DUR_S_MATCH *durNew();
// void dur(DUR_S_MATCH *m);
