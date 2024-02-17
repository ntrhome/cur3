#include "durView.h"
#include <stdio.h>

#define R "\033[0m" //reset
#define S "\033[1m\033[38;5;127m"
#define C "\033[1m\033[38;5;27m"
#define D "\033[1m\033[38;5;208m"
#define H "\033[1m\033[38;5;196m"
#define P "\033[1m\033[38;5;8m" //pile
static const char *const d_symCard [36] = {S"6s"R, S"7s"R, S"8s"R, S"9s"R, S"Xs"R, S"Js"R, S"Qs"R, S"Ks"R, S"As"R, C"6c"R, C"7c"R, C"8c"R, C"9c"R, C"Xc"R, C"Jc"R, C"Qc"R, C"Kc"R, C"Ac"R, D"6d"R, D"7d"R, D"8d"R, D"9d"R, D"Xd"R, D"Jd"R, D"Qd"R, D"Kd"R, D"Ad"R, H"6h"R, H"7h"R, H"8h"R, H"9h"R, H"Xh"R, H"Jh"R, H"Qh"R, H"Kh"R, H"Ah"R };
static const char *const d_symTrump [4] = {S"s"R, C"c"R, D"d"R, H"h"R}; //Spades, Clubs, Diamonds, Hearts
static const char *const d_symPlace [6] = {S"LL"R, C"RR"R, D"aa"R, H"dd"R, P"PP"R, "DD"};
static const char *const d_symPlayer[3] = {S"Left"R, C"Right"R, D"Nemo"R};

static const int cardByRankSuit[ed_ranks][ed_suits] = {
        {0,9,18,27},
        {1,10,19,28},
        {2,11,20,29},
        {3,12,21,30},
        {4,13,22,31},
        {5,14,23,32},
        {6,15,24,33},
        {7,16,25,34},
        {8,17,26,35}
};

static void view_score(const sMatch *m) {
    printf(".> Score: (%d) [%d:%d].\n", m->game, m->left.score, m->right.score);
}

static void view_newGame(const sMatch *m) {
    printf(".> New game. Match=%p.\n", m);
    view_score(m);
}

static void view_newFight(const sMatch *m) {
    printf(".> New fight. Attacker: %s player.\n", d_symPlayer[m->attacker->place]);
}

static void view_player(const  sDesk *d, const  sPlayer *p) {
    printf(".> Player %s: (%d)\n[", d_symPlayer[p->place], p->count);
    char *s = "";
    for (int card = 0; card < ed_cards; ++card) { //by suit
        if (d->place[card] == p->place) {
            printf("%s%s", s, d_symCard[card]);
            s = ".";
        }
    }
    printf("] /%s[", (p->count > 16 ) ? "\n" : " ");
    s = ""; //by ranks//
    for (int rank = 0; rank < ed_ranks; ++rank) { //by rank
        for (int suit = 0; suit < ed_suits; ++suit) {
            int card = cardByRankSuit[rank][suit]; //suit * ed_ranks + rank;
            if (d->place[card] == p->place) {
                printf("%s%s", s, d_symCard[card]);
                s = ".";
            }
        }
    }
    printf("].\n");
}

static void view_fight(const sMatch *m) {
    printf(".> Fight: (%d) [%s]\n - attack: [", m->fight, d_symTrump[m->desk.trump]);
    char *s = "";
    for (int i = m->history.count - m->fight; i < m->history.count; i+=2) {
        printf("%s%s", s, d_symCard[m->history.card[i]]);
        s = ".";
    }
    printf("]\n - defend: [");
    s = "";
    for (int i = m->history.count - m->fight + 1; i < m->history.count; i+=2) {
        printf("%s%s", s, d_symCard[m->history.card[i]]);
        s = ".";
    }
    printf("].\n");
}

static void view_attack(const sMatch *m) {
    view_player(&m->desk, m->attacker);
    view_fight((const sMatch *)m);
    char *s = (m->fight) ? "'e'-enough, " : "";
    printf("?> Your attack. Please type a card (e.g. '6s', 'Ah') or command (%s'q'-quit): ", s);
}

void durView(sMatch *m) { //in durView.c only this f have not const sBoard (because b->stage)
    switch (m->stage) {
        case es_newGameView:
            view_newGame((const sMatch *)m);
            m->stage = es_newFight;
            break;
        case es_newFightView:
            view_newFight((const sMatch *)m);
            m->stage = es_attack;
            break;
        case es_attackView:
            view_attack((const sMatch *)m);
            m->stage = es_attackControl;
            break;
//        case es_defendView:
//            durView_defend((const sBoard *)b);
//            b->stage = es_defendControl;
//            break;
//        case es_attackResultEnoughView:
//            printf(".> The fight is over by the attacker.\n");
//            b->stage = es_fightCloseAsDefended;
//            break;
//        case es_defendResultTookView:
//            printf(".> The fight is taken by the defender.\n");
//            b->stage = es_fightCloseAsTook;
//            break;
    }
}

//void durView_cmdWrong() {
//    printf("!> Wrong card or command was typed. Repeat please.\n");
//}

#ifdef DUR_DEBUG ////////////////////////////////////////////////////
void durView_match(sMatch *m) {
    printf("= = = = = = = = = = durView_match : sizeof(sMatch)=%lu, * = %p, stage = %d\n", sizeof(sMatch), m, m->stage);
    view_score(m);
    printf(".> Role: winner=%s, attacker=%s, defender=%s, dealer=%s.\n",
           d_symPlayer[(m->winner  ==NULL)?2:m->winner  ->place],
           d_symPlayer[(m->attacker==NULL)?2:m->attacker->place],
           d_symPlayer[(m->defender==NULL)?2:m->defender->place],
           d_symPlayer[(m->dealer  ==NULL)?2:m->dealer  ->place]);
    printf(".> Desk: (%d) [%s]\n[", m->desk.count, d_symTrump[m->desk.card[0] / ed_ranks]);
    char *s = "";
    for (int i = 0; i < ed_cards; ++i) { printf("%s%02d", s, i); s = "-"; }
    printf("]-id\n[");
    s = "";
    for (int i = 0; i < ed_cards; ++i) { printf("%s%s", s, d_symCard[m->desk.card[i]]); s = "."; }
    printf("]-card\n");
    if (m->stage > es_newGame) {
        s = "";
        printf("[");
        for (int i = 0; i < ed_cards; ++i) { printf("%s%02d", s, m->desk.position[i]); s = "."; }
        printf("]-position\n[");
        s = "";
        for (int i = 0; i < ed_cards; ++i) { printf("%s%s", s, d_symPlace[m->desk.place[m->desk.card[i]]]); s = "."; }
        printf("]-place.\n");
        if (m->stage > es_newFight) {
            view_player(&m->desk, &m->left);
            view_player(&m->desk, &m->right);
            view_fight(m);
        }
    }
    printf("= = = = = = = = = = durView_match.\n");
}
#endif //DUR_DEBUG
