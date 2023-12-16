#include <stdlib.h> // for srand, rand
#include <time.h>   // for srand, rand

#include "durModel.h"

static void matchInit(DUR_S_MATCH *d) {
    for (int p = 0; p < DUR_PLAYERS; ++p) { //score reset
        d->score[p] = 0;
    }
    d->game.winner = DUR_NONE;              //winner reset
    for (int i = 0; i < DUR_CARDS; ++i) {   //desk filling
        d->game.desk.card [i] = i;
    }
    for (int p = 0; p < DUR_PLAYERS; ++p) { //binding field to player
        d->game.fight.player[p].field = &d->game.fight.field;
    }
    d->stage = DUR_E_STAGE_GAME_INIT;
}

static void gameInit(DUR_S_MATCH *d) {
    if(d->game.winner == DUR_NONE) {    //Жеребьевка (первая игра или ничья).
        srand((int)time(NULL));
        d->game.winner = rand() % DUR_PLAYERS;
        d->game.fight.attacker     =  d->game.winner;
        d->game.fight.firstDealing = !d->game.winner;
    }
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
    for(int p = 0; p < DUR_PLAYERS; ++p) { //set player[p].trump и player[p].hand.count = 0
        d->game.fight.player[p].trump      = d->game.desk.trump;
        d->game.fight.player[p].hand.count = 0;
    }
    d->game.history.count = 0;
    d->stage = DUR_E_STAGE_FIGHT_INIT;
}

static void fightInit(DUR_S_MATCH *d) {
    int turn[DUR_PLAYERS]; //dealing turn forming
    for(int p = 0; p < DUR_PLAYERS; ++p) {
        turn[p] = d->game.fight.firstDealing;
        if(++d->game.fight.firstDealing == DUR_PLAYERS) {
            d->game.fight.firstDealing = 0;
        }
    }
    d->game.fight.firstDealing = d->game.fight.attacker; //save next fight firstDealing
    for(int p = 0; p < DUR_PLAYERS; ++p) { //dealing
        while((d->game.fight.player[turn[p]].hand.count < 6) & (d->game.desk.count > 0)) {
            //1 - убавляем карту в колоде и отмечаем ее новый place
            --d->game.desk.count;
            d->game.desk.place[d->game.desk.count] = turn[p];
            //2 - заносим в историю
            d->game.history.idCard[d->game.history.count] = d->game.desk.count;
            d->game.history.place [d->game.history.count] = turn[p];
            ++d->game.history.count;
            //3 - добавляем карту плейеру
            d->game.fight.player[turn[p]].hand.idCard[d->game.fight.player[turn[p]].hand.count] = d->game.desk.count;
            //d->game.fight.player[turn[p]].hand.card  [d->game.fight.player[turn[p]].hand.count] = d->game.desk.card[d->game.desk.count];
            ++d->game.fight.player[turn[p]].hand.count;
        }
    }
    for(int p = 0; p < DUR_PLAYERS; ++p) { //player[p].isDeskEmpty и player[p].isAttacker
        d->game.fight.player[p].isDeskEmpty = (d->game.desk.count == 0);
        d->game.fight.player[p].isAttacker  = (d->game.fight.attacker == p);
    }
    d->game.fight.field.count = 0;
    d->stage = DUR_E_STAGE_FIGHT;
}

DUR_E_STAGE dur(DUR_S_MATCH *d) {
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
            break;
    }
    return d->stage;
}