#include <stdlib.h> // for srand, rand
#include <time.h>   // for srand, rand

#include "durModel.h"
#include "durView.h"

static void matchInit(DUR_S_MATCH *d) {
    //DUR_S_MATCH
    for (int p = 0; p < DUR_PLAYERS; ++p) { //score reset
        d->score[p] = 0;
    }
    //DUR_S_GAME
    d->game.winner = DUR_NONE;              //winner reset
    //DUR_S_DESK
    for (int i = 0; i < DUR_CARDS; ++i) {   //desk filling
        d->game.desk.card [i] = i;
    }
    d->stage = DUR_E_STAGE_GAME_INIT;
}

static void gameInit(DUR_S_MATCH *d) {
    //DUR_S_GAME
    //DUR_S_FIGHT
    if(d->game.winner == DUR_NONE) {    //Жеребьевка (первая игра или ничья).
        srand((int)time(NULL));
        d->game.winner = rand() % DUR_PLAYERS;
        d->game.fight.attacker =  d->game.winner;
        d->game.fight.dealer   = !d->game.winner;
    }
    //DUR_S_DESK
    srand((int)time(NULL));   //desk shuffle, place, set head and trump
    for(int i = 0; i < DUR_CARDS; ++i) {
        int depot = d->game.desk.card[i];
        int iRand = rand() % DUR_CARDS;
        d->game.desk.card[i] = d->game.desk.card[iRand];
        d->game.desk.card[iRand] = depot;
        d->game.desk.place[i] = DUR_E_PLACE_DESK;
    }
    d->game.desk.count = DUR_CARDS;
    d->game.desk.trump = d->game.desk.card[0] / DUR_RANKS;
    //DUR_S_HISTORY
    d->game.history.count = 0;
    d->stage = DUR_E_STAGE_FIGHT_INIT;
}

static void fightInit(DUR_S_MATCH *d) {
    int dealerCardCount; //dealing
    for(int p = 0; p < DUR_PLAYERS; ++p) {
        dealerCardCount = 0;
        for(int i = 0; i < DUR_CARDS; ++i) {
            if(d->game.desk.place[i] == d->game.fight.dealer) {
                ++dealerCardCount;
            }
        }
        while((dealerCardCount < 6) & (d->game.desk.count > 0)) {
            //1 - убавляем карту в колоде, отмечаем в place ее передачу игроку dealer
            --d->game.desk.count;
            d->game.desk.place[d->game.desk.count] = d->game.fight.dealer; //отмечаем в place ее передачу игроку dealer
            //2 - увеличиваем текущее число карт у текущего игрока
            ++dealerCardCount;
            //3 - заносим в историю
            d->game.history.desk [d->game.history.count] = d->game.desk.count;
            d->game.history.place[d->game.history.count] = d->game.fight.dealer;
            ++d->game.history.count;
        }
        ++d->game.fight.dealer; //следующий игрок для получения карт
        if(d->game.fight.dealer == DUR_PLAYERS) { //корректируем очередность
            d->game.fight.dealer = 0;
        }
    }
    d->game.fight.dealer = d->game.fight.attacker; //save next fight dealer

    d->game.fight.historyCount = d->game.history.count;
    d->stage = DUR_E_STAGE_FIGHT_ATTACK;
}

static void fight(DUR_S_MATCH *d) {
    //ПЕРЕХОДИМ В РЕЖИМ ОДНОЗНАЧНО ДВУХ ИГРОКОВ!
    //собираем и анализируем инфу поля


    //собираем и анализируем инфу атакера, даем ф() на получение хода
    //собираем и анализируем инфу дефендера, ждем ф() ответ
    //анализируем баланс битвы, выставляем стейдж
}

int dur(DUR_S_MATCH *d) {
    switch (d->stage) {
        case DUR_E_STAGE_MATCH_INIT:
            matchInit(d);
            //break;
        case DUR_E_STAGE_GAME_INIT:
            gameInit(d);
            //break;
        case DUR_E_STAGE_FIGHT_INIT:
            fightInit(d);
            //break;
        case DUR_E_STAGE_FIGHT:
            admin(*d);

            break;
    }
    return d->stage;
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
