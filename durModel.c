#include "durModel.h"
#include <stdlib.h> //for srand, rand, malloc, free
#include <time.h>   //for srand

sMatch *durModel_newMatch() {
    sMatch *m = malloc(sizeof(sMatch));
    LOGIFN(m)
    if (m != NULL) {
        for (int i = 0; i < ed_cards; ++i) m->desk.card[i] = i;
        m->left.place  = ep_left,  m->left.score  = 0;
        m->right.place = ep_right, m->right.score = 0;
        m->winner = NULL;
        m->game = 0;
        m->stage = es_gameModel;

    }
    return m;
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

void durModel(sMatch *m) {
    if (m != NULL) {
        switch (m->stage) {
            case es_gameModel:
                game(m);
                m->stage = es_gameView;
                break;
//        case es_newFight:
//            newFight(m);
//            m->stage = es_newFightView;
//            break;
//        case es_newFightView: //view
//            break;
//        case es_attack:
//            attack(m);
//            m->stage = es_attackView;
//            break;
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
                m->stage = es_cmdQuit;
        }
    }
}

void durModel_closeMatchIfQuit(sMatch **m) {
    if ((*m)->stage == es_cmdQuit) {
        sMatch *mm = *m;
        free(*m);
        *m = NULL;
    }
}
