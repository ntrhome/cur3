#include <stdlib.h> // for srand, rand
#include <time.h>   // for srand, rand

#include "durModel.h"
//#include "durView.h"

static void matchInit(dur_s_match *m) {
    for (int p = 0; p < DUR_PLAYERS; ++p) { //score reset
        m->score[p] = 0;
    }
    m->game.winner = DUR_NONE;
    for (int i = 0; i < DUR_CARDS; ++i) {   //desk filling
        m->game.battle.field.desk.card[i] = i;
    }
}

static void gameInit(dur_s_game *g) {
    if(g->winner == DUR_NONE) {    //Жеребьевка (первая игра или ничья).
        srand((int)time(NULL));
        g->winner = rand() % DUR_PLAYERS;
        g->battle.attacker =  g->winner;
        g->battle.dealer   = !g->winner;
    }
    srand((int)time(NULL));   //desk shuffle, place, set head and trump
    for(int i = 0; i < DUR_CARDS; ++i) {
        int depot = g->battle.field.desk.card[i];
        int iRand = rand() % DUR_CARDS;
        g->battle.field.desk.card[i] = g->battle.field.desk.card[iRand];
        g->battle.field.desk.card[iRand] = depot;
        g->battle.field.desk.place[i] = DUR_E_PLACE_DESK;
    }
    g->battle.field.desk.count = DUR_CARDS;
    g->battle.field.history.count = 0;
}

static void history(dur_s_history *h, int desk, int place) {
    if(h->count < DUR_HISTORY_MAX) {
        h->desk [h->count] = desk;
        h->place[h->count] = place;
        ++h->count;
    }
}

static void battleInit(dur_s_battle *b) {
    int player[DUR_PLAYERS] = {b->dealer, !b->dealer}; //dealing
    for (int p = 0; p < DUR_PLAYERS; ++p) {
        int count = 0;
        int i = DUR_CARDS-1;
        for (; i >= 0; --i) {
            if(b->field.desk.place[i] == player[p]) {
                ++count;
            } else if(b->field.desk.place[i] == DUR_E_PLACE_DESK) {
                break;
            }
        }
        for (; i >= 0; --i) {
            if(count >= 6) break;
            b->field.desk.place[i] = player[p];
            ++count;
            history(&b->field.history, i, player[p]);
        }
    }
    b->dealer = b->attacker;
}

static void fight(dur_s_match *m) {
    //ПЕРЕХОДИМ В РЕЖИМ ОДНОЗНАЧНО ДВУХ ИГРОКОВ!
    //собираем и анализируем инфу поля


    //собираем и анализируем инфу атакера, даем ф() на получение хода
    //собираем и анализируем инфу дефендера, ждем ф() ответ
    //анализируем баланс битвы, выставляем стейдж
}

void croupier(dur_s_match *m) {
    switch (m->stage) {
        case DUR_E_STAGE_MATCH_INIT:
            matchInit(m);
            m->stage = DUR_E_STAGE_GAME_INIT;
            break;
        case DUR_E_STAGE_GAME_INIT:
            gameInit(&m->game);
            m->stage = DUR_E_STAGE_BATTLE_INIT;
            break;
        case DUR_E_STAGE_BATTLE_INIT:
            battleInit(&m->game.battle);

            break;
    }
}

// DUR_S_MATCH *durNew() { //do for each new match
//     DUR_S_MATCH *m = malloc(sizeof(DUR_S_MATCH));
//     m->stage = DUR_E_STAGE_GAME_NEW;
//     m->score[DUR_E_PLACE_PLAYER_0] = 0;
//     m->score[DUR_E_PLACE_PLAYER_1] = 0;
//     m->game.playerWinner = DUR_NONE;
//     for(int i = 0; i < DUR_CARDS; ++i) {
//         m->game.desk.card[i] = i;
//     }
//     return m; //point to DUR_S_MATCH as match id
// }
// void durDel(DUR_S_MATCH *m) {
//     free(m);
// }
