#ifndef DUR_DURMODEL_H
#define DUR_DURMODEL_H

#define DUR_DEBUG

#define DUR_ERROR       (-7)
#define DUR_QUIT        (-6)
#define DUR_NONE        (-5)
#define DUR_BAD         (-4) //abrakadabra
#define DUR_WRONG       (-3) //acceptable but incorrect
#define DUR_PLAYERS     2
#define DUR_HISTORY_MAX 300
#define DUR_RANKS       9
#define DUR_SUITS       4
#define DUR_CARDS       (DUR_RANKS * DUR_SUITS)
#define DUR_NORMAL      6

typedef enum DUR_STAGE {DUR_STAGE_NEW_MATCH=100, DUR_STAGE_NEW_GAME, DUR_STAGE_NEW_ROUND, DUR_STAGE_ROUND, DUR_STAGE_GAME_OVER, DUR_STAGE_QUIT, DUR_STAGE_ERROR} dur_stage;
typedef enum DUR_PLACE {DUR_DESK=-1, DUR_LEFT, DUR_RIGHT, DUR_ATTACK_0, DUR_DEFEND_0, DUR_ATTACK_1, DUR_DEFEND_1, DUR_ATTACK_2, DUR_DEFEND_2, DUR_ATTACK_3, DUR_DEFEND_3, DUR_ATTACK_4, DUR_DEFEND_4, DUR_ATTACK_5, DUR_DEFEND_5, DUR_PILE} dur_place;

typedef struct DUR_S_FIELD_DESK {
    int card [DUR_CARDS];
    dur_place place[DUR_CARDS];
    int head;
    int trump;
} dur_s_desk;

typedef struct DUR_S_FIELD_PLAYER {
    int desk [DUR_CARDS];
    int count;
} dur_s_player;

typedef struct DUR_S_FIELD_HISTORY {
    int desk [DUR_HISTORY_MAX]; //id of the card from the "desk"
    dur_place place[DUR_HISTORY_MAX];
    int count;
} dur_s_history;

typedef struct DUR_S_FIELD {
    dur_s_desk desk;
    dur_s_player player[DUR_PLAYERS];
    dur_s_history history;
    int fieldHistory;
    //dur_place fieldStage; //n: DUR_ATTACK_n, DUR_DEFEND_n
    dur_stage stage;
} dur_s_field;

typedef struct DUR_S_ROUND {
    int dealer;   //первый берущий карты
    int attacker; //атакующий
    dur_s_field field;
} dur_s_round;

//struct DUR_S_GAME;
//typedef struct DUR_S_GAME dur_s_game;
typedef struct DUR_S_GAME {
    int score[DUR_PLAYERS];
    int winner;
    dur_s_round round;
//    int id; //id текущей игры (имя)
//    dur_s_game *tail;
//    dur_s_game *head;
} dur_s_game;

void dur();

#endif //DUR_DURMODEL_H
