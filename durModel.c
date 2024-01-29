#include <stdlib.h> //for srand, rand, malloc, free
#include <time.h>   //for srand

#include "durModel.h"
#include "durView.h"
#include "durControl.h"

static sBoard *newBoard() {
    sBoard *b = malloc(sizeof(sBoard));
    for (int i = 0; i < ed_cards; ++i) b->desk.card[i] = i; //cards filling
    b->left.place = ep_left;
    b->left.score = 0;
    b->right.place = ep_right;
    b->right.score = 0;
    b->winner = NULL;
    b->stage = es_newGame;
    return b;
}

static void newGame_shuffleDesk(sDesk *d) {
    srand((int)time(NULL));
    d->count = ed_cards;
    int flReshuffle;
    do { //cards shuffle
        flReshuffle = 0;
        for (int i = 0; i < ed_cards; ++i) {
            int r;
            int depot = d->card[r = rand() % ed_cards];
            d->card[r] = d->card[i];
            d->card[i] = depot;
        }
        int suit[ed_suits]; //test (if there are more than 4 same suits in one hand - Reshuffle)
        for (int i = ed_talon; i < ed_cards; ++i) { //test the cards intended for first dealing
            if (i == ed_talon || i == ed_talon + ed_normal) { suit[0]=0; suit[1]=0; suit[2]=0; suit[3]=0; }
            if (++suit[d->card[i] / ed_ranks] > 4) { //more than 4 card of the same suits
                flReshuffle = 1;
                break;
            }
        }
    } while (flReshuffle);
    for (int i = 0; i < ed_cards; ++i) {
        d->position[d->card[i]] = i;
    }
    d->trump = d->card[0] / ed_ranks; //suits of trump
}
static void newGame_playersInit(sBoard *b) {
    b->left.count = 0;
    b->right.count = 0;
    for (int i = 0; i < ed_cards; ++i) { //player holders filling
        b->left.holder[i]  = 0;
        b->right.holder[i] = 0;
    }
    if(b->winner == NULL) { //roles :drawing (first or drawn game)
        srand((int)time(NULL));
        b->attacker = (rand() % ed_players) ? &b->right : &b->left;
    } else {
        b->attacker = b->winner;
    }
    b->defender = (b->attacker == &b->left) ? &b->right : &b->left;
    b->dealer = &b->left; //b->attacker; //to reduce history it possible to assign permanently &b->left (with implementation of first dealing)
}
static void newGame(sBoard *b) {
    newGame_shuffleDesk(&b->desk);
    newGame_playersInit(b);
    b->history.count = 0;
    b->stage = es_newGameView;
}

static void history(sHistory *h, int card, ep place) {
    h->card[h->count] = card;
    h->place[h->count++] = place;
}

static void newFight_dealing(sBoard *b) {
    sPlayer *p = b->dealer;
    for (int n = 0; n < ed_players; ++n) {
        while (p->count < ed_normal && b->desk.count > 0) {
            p->holder[b->desk.card[--b->desk.count]] = true;
            ++p->count;
            history(&b->history, b->desk.card[b->desk.count], p->place);
        }
        p = (p == &b->left) ? &b->right : &b->left;
    }
}
static void newFight(sBoard *b) {
    newFight_dealing(b);
    b->dealer = b->attacker; //for next newFight
    b->attack.count = 0;
    b->defend.count = 0;
    b->stage = es_newFightView;
}

static void attack(sBoard *b) {
    //cheks
    b->stage = es_attackView;
}

static int isFightsHasRank(const sBoard *b, int rank) {
    for (int i = 0; i < b->attack.count; ++i) {
        if (rank == b->attack.card[i] % ed_ranks) return 1;
    }
    for (int i = 0; i < b->defend.count; ++i) {
        if (rank == b->defend.card[i] % ed_ranks) return 1;
    }
    return 0;
}
static void attackResult(sBoard *b) {
    if (b->cmd > 1000) { //cmd (b->cmd = es_cmd_wrong; es_cmd_take; es_cmd_enough; es_cmd_newGame; es_cmd_quit; //card)
        if (b->attack.count > 0 && b->cmd == es_cmd_enough) { b->stage = es_defend; return; }
        if (b->cmd == es_cmd_quit) { b->stage = es_cmd_quit; return; }
        b->cmd = es_cmd_wrong;
    } else if ( //card:
        b->attacker->holder[b->cmd]
        &&
        (
            (b->attack.count == 0) //first attack
            ||
            isFightsHasRank(b, (int) b->cmd % ed_ranks) //next attack
        )
    )
    {
        b->attacker->holder[b->cmd] = 0;
        --b->attacker->count;
        b->attack.card[b->attack.count++] = b->cmd;
        history(&b->history, b->cmd, ep_attack);
        b->stage = es_defend;
        return;
    }
    if (b->cmd == es_cmd_wrong) {
        durView_msg("-> Some stuff has been typed. Please try again.\n");
    } else {
        durView_msg("-> Some unacceptable has been typed. Please try again.\n");
    }
    b->stage = es_attackView; //repeat
}

//-----------------------------------------
static void defend(sBoard *b) {
    //cheks
    b->stage = es_defendView;
}
static void defendResult(sBoard *b) {
    if (b->cmd > 1000) { //cmd (b->cmd = es_cmd_wrong; es_cmd_take; es_cmd_enough; es_cmd_newGame; es_cmd_quit; //card)
        if (b->cmd == es_cmd_enough) { b->stage = es_defend; return; }//todo
        if (b->cmd == es_cmd_quit) { b->stage = es_cmd_quit; return; }
        b->cmd = es_cmd_wrong;
    } else if ( //card:
            b->attacker->holder[b->cmd]
            &&
            (
                    (b->attack.count == 0) //first attack
                    ||
                    isFightsHasRank(b, (int) b->cmd % ed_ranks) //next attack
            )
            )
    {
        b->attacker->holder[b->cmd] = 0;
        --b->attacker->count;
        b->attack.card[b->attack.count++] = b->cmd;
        history(&b->history, b->cmd, ep_attack);
        b->stage = es_defend;
        return;
    }
    if (b->cmd == es_cmd_wrong) {
        durView_msg("-> Some stuff has been typed. Please try again.\n");
    } else {
        durView_msg("-> Some unacceptable has been typed. Please try again.\n");
    }
    b->stage = es_attackView; //repeat
}

void durModel(sBoard *b) {
    switch (b->stage) {
    case es_newGame:
        newGame(b);
        break;
    case es_newFight:
        newFight(b);
        break;
    case es_attack:
        attack(b);
        break;
    case es_attackResult:
        attackResult(b);
        break;
    case es_defend:
        defend(b);
        break;
    case es_defendResult:
        defendResult(b);
        break;
    }
//    durView_msg("----------------------\n");
//    durView_dbg_board(b); //dbg
}

void dur() {
    sBoard *b1 = newBoard();
    while (b1->stage != es_cmd_quit)
    {
        durModel(b1);
        durView(b1);    //отделяем для автономности - возможность потока
        durControl(b1); //отделяем для автономности - возможность потока
    }
    free(b1);
    durView_msg(".> Quit.");
}
