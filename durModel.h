#ifndef DUR_DURMODEL_H
#define DUR_DURMODEL_H

typedef enum {DUR_STAGE_DATAINIT, DUR_STAGE_GAMEINIT, DUR_STAGE_ROUNDINIT, DUR_STAGE_ROUNDSTEP} DUR_STAGE;
typedef enum {DUR_PLACE_DESK=-1, DUR_PLACE_PLAYER0, DUR_PLACE_PLAYER1, DUR_PLACE_ATTACK0, DUR_PLACE_DEFEND0, DUR_PLACE_ATTACK1, DUR_PLACE_DEFEND1, DUR_PLACE_ATTACK2, DUR_PLACE_DEFEND2, DUR_PLACE_ATTACK3, DUR_PLACE_DEFEND3, DUR_PLACE_ATTACK4, DUR_PLACE_DEFEND4, DUR_PLACE_ATTACK5, DUR_PLACE_DEFEND5, DUR_PLACE_PILE} DUR_PLACE;

#define DUR_NONE        (-5)
#define DUR_PLAYERS     2
#define DUR_MAXHISTORY  300
#define DUR_HANDNORM    6
#define DUR_RANKS       9
#define DUR_SUITS       4
#define DUR_CARDS       (DUR_RANKS * DUR_SUITS)
#define desk            history

typedef struct FIELD {
    int idDesk[2 * DUR_HANDNORM]; //id from desk
    int cards [2 * DUR_HANDNORM];
    int count;
} DUR_FIELD; //общий для игроков

typedef struct {
    int id   [DUR_CARDS]; //in desk
    int cards[DUR_CARDS];
    int count;
} DUR_HAND;

typedef struct {
    int head;  //desk
    int trump; //desk
    DUR_HAND hand;
    const DUR_FIELD *field;
} DUR_PLAYER;

typedef struct {
    DUR_FIELD field;
    DUR_PLAYER player[DUR_PLAYERS];
} DUR_ROUND;

typedef struct {
    int card [DUR_MAXHISTORY]; //desk + history, причем history содержат id от desk
    int place[DUR_MAXHISTORY]; //desk + history //первые DUR_CARDS place во время игры отражают текущее место карт, поэтому перед сохраненим истории восстанавливаем place для desk (DUR_PLACE_DESK) или же просто помним, что у первых DUR_CARDS place = DUR_PLACE_DESK и восстанавливаем это при репассинге игры
    int count; //history
    int head;  //desk
    int trump; //desk
} DUR_HISTORY;

typedef struct {
    int attacker;
    int firstDealing;
    DUR_ROUND round;
    DUR_HISTORY history;
} DUR_GAME;

#define DUR_DATA_INIT {DUR_STAGE_DATAINIT, {0,0}, DUR_NONE}
typedef struct {
    DUR_STAGE stage;
    int score[DUR_PLAYERS];
    int winner;
    DUR_GAME game;
} DUR_DATA;

void dur(DUR_DATA *d);

#endif //DUR_DURMODEL_H
