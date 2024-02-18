#include <stdlib.h> //for srand, rand, malloc, free
#include <time.h>   //for srand
#include "durModel.h"

sMatch *durNewMatch() {
    sMatch *m = malloc(sizeof(sMatch));
    LOGIF(m == NULL)
    if (m != NULL) {
        m->stage = es_newGame;
        m->game = 0;
        for (int i = 0; i < ed_cards; ++i) m->desk.card[i] = i;
        m->left.place  = ep_left,  m->left.score  = 0;
        m->right.place = ep_right, m->right.score = 0;
        m->winner = NULL;
    }
    return m;
}

static void newGame_shuffle(sDesk *d) {
    srand((int)time(NULL));
    d->count = ed_cards;
    bool flReshuffle;
    do { //cards shuffle
        flReshuffle = false;
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
                flReshuffle = true;
                break;
            }
        }
    } while (flReshuffle);
    for (int i = 0; i < ed_cards; ++i) {
        d->position[d->card[i]] = i;
        d->place[i] = ep_desk;
    }
    d->trump = d->card[0] / ed_ranks; //suits of trump
}
static void newGame_players(sMatch *m) {
    m->left.count = 0;
    m->right.count = 0;
    if(m->winner == NULL) { //roles :drawing (first or drawn game)
        srand((int)time(NULL));
        m->attacker = (rand() % ed_players) ? &m->right : &m->left;
    } else {
        m->attacker = m->winner;
    }
    m->defender = (m->attacker == &m->left) ? &m->right : &m->left;
    m->dealer = &m->right; //to reduce history it possible to assign permanently &b->left (with implementation of first dealing)
}
static void newGame(sMatch *m) {
    newGame_shuffle(&m->desk);
    newGame_players(m);
    m->history.count = 0;
}


static void history(sHistory *h, int card, ep place) {
    h->card[h->count] = card;
    h->place[h->count++] = place;
}

static void newFight_dealing(sMatch *m) {
    sPlayer *p = m->dealer;
    for (int n = 0; n < ed_players; ++n) {
        while (p->count < ed_normal && m->desk.count > 0) {
            m->desk.place[m->desk.card[--m->desk.count]] = p->place;
            ++p->count;
            history(&m->history, m->desk.card[m->desk.count], p->place);
        }
        p = (p == &m->left) ? &m->right : &m->left;
    }
}
static void newFight(sMatch *m) {
    newFight_dealing(m);
    m->dealer = m->attacker; //for next newFight
    m->fight = 0;
}

static void attack(sMatch *m) {
    //cheks
    m->stage = es_attackView;
}

static void fight2pile(sMatch *m) {
    int frameBegin = m->history.count - m->fight;
    int frameEnd   = m->history.count;
    for (int i = frameBegin; i < frameEnd; ++i) {
        m->desk.place[m->history.card[i]] = ep_pile;
        history(&m->history, m->history.card[i], ep_pile);
    }
    sPlayer *depot = m->attacker; m->attacker = m->defender; m->defender = depot;
}

static void attackResult(sMatch *m) {
    if (m->cmd < 0) { //cmd
        if (m->cmd == es_cmdEnough && m->fight) {
            fight2pile(m);
            m->stage = es_newFight;
            return;
        }
        if (m->cmd == es_cmdQuit) {
            m->stage = es_cmdQuit;
            return;
        }
        m->cmd = es_cmdWrong; //todo
    } else if (b->attacker->hold[b->cmd] && ((b->attack.count == 0) || attackResult_isFightHasRank(b))) { //card
        b->attacker->hold[b->cmd] = 0;
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


void durModel(sMatch *m) {
    switch (m->stage) {
        case es_newGame:
            newGame(m);
            m->stage = es_newGameView;
            break;
        case es_newGameView: //view
            break;
        case es_newFight:
            newFight(m);
            m->stage = es_newFightView;
            break;
        case es_newFightView: //view
            break;
        case es_attack:
            attack(m);
            m->stage = es_attackView;
            break;
        case es_attackResult:
            attackResult(m);
            break;
//        case es_defend:
//            defend(b);
//            break;
//        case es_defendResult:
//            defendResult(b); //todo
//            break;
//        case es_fightCloseAsDefended:
//            fightCloseAsDefended(b);
//            break;
//        case es_fightCloseAsTook:
//            fightCloseAsTook(b);
//            break;
        default:
            m->stage = es_cmdQuit;
    }
}
