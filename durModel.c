#include "durModel.h"
#include <stdlib.h> //for srand, rand, malloc, free
#include <time.h>   //for srand

static const int rankOfCard[ed_cards] = { 0,1,2,3,4,5,6,7,8,0,1,2,3,4,5,6,7,8,0,1,2,3,4,5,6,7,8,0,1,2,3,4,5,6,7,8 };
static const int suitOfCard[ed_cards] = { 0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3 };

sMatch *durModel_newMatch() {
    sMatch *m = malloc(sizeof(sMatch));
    LOGIFN(m)
    if (m != NULL) {
        for (int i = 0; i < ed_cards; ++i) m->desk.card[i] = i;
        m->left.place  = ep_left,  m->left.score  = 0;
        m->right.place = ep_right, m->right.score = 0;
        m->winner = NULL;
        m->game = 0;
        m->state = es_gameModel;
    }
    return m;
}
void durModel_newMatch_quitCheckout(sMatch **pm) {
    if ((*pm)->state == es_cmdQuit) {
        free(*pm);
        *pm = NULL;
    }
}

static void game_desk(sDesk *d) { //new game desk init
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
        int suit[ed_suits]; //shuffle test (if there are more than 4 same suits in one hand - Reshuffle)
        for (int i = ed_talon; i < ed_cards; ++i) { //test the cards intended for first dealing
            if (i == ed_talon || i == ed_talon + ed_normal) { suit[0]=0; suit[1]=0; suit[2]=0; suit[3]=0; }
            if (++suit[d->card[i] / ed_ranks] > 4) { //more than 4 card of the same suits
                flReshuffle = true;
                break;
            }
        }
    } while (flReshuffle);
    d->trump = d->card[0] / ed_ranks; //trump init (suit)
    for (int i = 0; i < ed_cards; ++i) { //position & place init
        d->position[d->card[i]] = i;
        d->place[i] = ep_desk;
    }
}
static void game_players(sMatch *m) { //new game players init
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
static void game(sMatch *m) {
    game_desk(&m->desk);
    game_players(m);
    m->history.count = 0;
}

static void history(sHistory *h, int card, ep place) {
    h->card[h->count] = card;
    h->place[h->count++] = place;
}

static void fight_dealing(sMatch *m) {
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
static void fight(sMatch *m) {
    fight_dealing(m);
    m->dealer = m->attacker; //for next newFight
    m->fight = 0;
}

static void attack(sMatch *m) {
}

static bool attackHandler_isFightHasRank(const sMatch *m) {
    int rank = rankOfCard[m->cmd];
    for (int i = m->history.count - m->fight; i < m->history.count; ++i) {
        if (rankOfCard[m->history.card[i]] == rank) return true;
    }
    return false;
}
static es attackHandler(sMatch *m) {
    if (m->cmd > es_cmd) { //command
        if (m->fight && m->cmd == es_cmdEnough) {
            int fightBegin = m->history.count - m->fight;
            int fightEnd   = m->history.count;
            for (int i = fightBegin; i < fightEnd; ++i) {
                m->desk.place[m->history.card[i]] = ep_pile;
                history(&m->history, m->history.card[i], ep_pile);
            }
            sPlayer *depot = m->attacker; m->attacker = m->defender; m->defender = depot;
            return es_fightModel;
        }
        if (m->cmd == es_cmdQuit) return es_cmdQuit;
        return es_attackHandlerViewWrong;
    } else {            //card
        if (m->desk.place[m->cmd] == m->attacker->place && (m->fight == 0 || attackHandler_isFightHasRank(m))) {
            m->desk.place[m->cmd] = ep_attack;
            ++m->fight;
            --m->attacker->count;
            history(&m->history, m->cmd, ep_attack);
//if (m->attacker->count == ed_normal) return es_cmdQuit; //dbg
//return es_attackModel; //dbg
            return es_defendModel;
        }
        return es_attackHandlerViewUnacceptable;
    }
}

static void defend(sMatch *m) {
}

static bool defendHandler_isDefendingCardBeatAttackingCard(const sMatch *m) {
    if (suitOfCard[m->history.card[m->history.count - 1]] == suitOfCard[m->cmd] ) {
        return rankOfCard[m->history.card[m->history.count - 1]] < rankOfCard[m->cmd];
    } else {
        return suitOfCard[m->cmd] == m->desk.trump;
    }
}
static es defendHandler(sMatch *m) {
    if (m->cmd > es_cmd) { //command
        if (m->fight && m->cmd == es_cmdTake) {
            int fightBegin = m->history.count - m->fight;
            int fightEnd   = m->history.count;
            for (int i = fightBegin; i < fightEnd; ++i) {
                m->desk.place[m->history.card[i]] = m->defender->place;
                history(&m->history, m->history.card[i], m->defender->place);
            }
            m->defender->count += m->fight;
            return es_fightModel;
        }
        if (m->cmd == es_cmdQuit) return es_cmdQuit;
        return es_defendHandlerViewWrong;
    } else {               //card
        if ((m->desk.place[m->cmd] == m->defender->place) && defendHandler_isDefendingCardBeatAttackingCard(m)) {
            m->desk.place[m->cmd] = ep_defend;
            ++m->fight;
            --m->defender->count;
            history(&m->history, m->cmd, ep_defend);
            return es_attackModel;
        }
        return es_defendHandlerViewUnacceptable;
    }
}

void durModel(sMatch *m) {
    if (m != NULL) {
        switch (m->state) {
            case es_gameModel:
                game(m);
                m->state = es_gameView;
                break;
            case es_gameView:
                break;
            case es_fightModel:
                fight(m);
                m->state = es_attackModel;
                break;
            case es_attackModel:
                attack(m);
                m->state = es_attackView;
            case es_attackView:
            case es_attackControl:
                break;
            case es_attackHandler:
                m->state = attackHandler(m);
                break;
            case es_attackHandlerViewWrong:
            case es_attackHandlerViewUnacceptable:
                break;
            case es_defendModel:
                defend(m);
                m->state = es_defendView;
                break;
            case es_defendView:
            case es_defendControl:
                break;
            case es_defendHandler:
                m->state = defendHandler(m);
                break;
            case es_defendHandlerViewWrong:
            case es_defendHandlerViewUnacceptable:
                break;
//        case es_attackResult:
//            attackResult(m);
//            break;
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
                LOG
                m->state = es_cmdQuit;
        }
    }
}
