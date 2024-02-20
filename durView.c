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

const int cardOfRankSuit[ed_ranks][ed_suits] = {
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

static void score(const sMatch *m) {
    printf(".> Score: (%d) [%d:%d].\n", m->game, m->left.score, m->right.score);
}

static void game(const sMatch *m) {
    printf(".> New game. Match=%p.\n", m);
    score(m);
}

static void player(const  sMatch *m, const  sPlayer *p) {
    char *s = (p == m->attacker) ? "Attacker" : "Defender";
    printf(".> %s %s: (%d)\n[", s, d_symPlayer[p->place], p->count);
    s = "";
    for (int i = 0; i < ed_cards; ++i) { //by suit
        if (m->desk.place[i] == p->place) {
            printf("%s%s", s, d_symCard[i]);
            s = ".";
        }
    }
    printf("] /%s[", (p->count > 16 ) ? "\n" : " ");
    s = ""; //by ranks//
    for (int rank = 0; rank < ed_ranks; ++rank) { //by rank
        for (int suit = 0; suit < ed_suits; ++suit) {
            if (m->desk.place[cardOfRankSuit[rank][suit]] == p->place) { //suit * ed_ranks + rank;
                printf("%s%s", s, d_symCard[cardOfRankSuit[rank][suit]]);
                s = ".";
            }
        }
    }
    printf("].\n");
}
static void fight(const sMatch *m) {
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
static void attack(const sMatch *m) {
    player(m, m->attacker);
    fight(m);
    char *s = (m->fight) ? "'e'-enough, " : "";
    printf("?> Your attack. Please type a card (e.g. '6s', 'Ah') or command (%s'q'-quit): ", s);
}

void durView(sMatch *m) { //in durView.c only this f have not const sBoard (because b->state)
    if (m != NULL) {
        switch (m->state) {
            case es_gameView:
                game((const sMatch *) m);
                m->state = es_fightModel;
                break;
            case es_attackView:
                attack((const sMatch *)m);
                m->state = es_attackControl; //
                break;
            case es_attackHandlerViewWrong:
                printf("!> Your answer is wrong.\n");
                m->state = es_attackView;
                break;
            case es_attackHandlerViewUnacceptable:
                printf("!> Your answer is unacceptable.\n");
                m->state = es_attackView;
                break;
//        case es_attackView:
//            view_attack((const sMatch *)m);
//            m->state = es_attackControl;
//            break;
//        case es_defendView:
//            durView_defend((const sBoard *)b);
//            b->state = es_defendControl;
//            break;
//        case es_attackResultEnoughView:
//            printf(".> The fight is over by the attacker.\n");
//            b->state = es_fightCloseAsDefended;
//            break;
//        case es_defendResultTookView:
//            printf(".> The fight is taken by the defender.\n");
//            b->state = es_fightCloseAsTook;
//            break;
        }
    }
}

