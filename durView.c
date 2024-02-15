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

static void view_score(const sMatch *m) {
    printf(".> Score: (%d) [%d:%d].\n", m->game, m->left.score, m->right.score);
}




void durView_match(sMatch *m) {
    printf("= = = = = = = = = = durView_match : sizeof(sMatch)=%lu, * = %p, stage = %d\n", sizeof(sMatch), m, m->stage);
    view_score(m);
    printf(".> Role: winner=%s, attacker=%s, defender=%s, dealer=%s.\n",
           d_symPlayer[(m->winner  ==NULL)?2:m->winner  ->place],
           d_symPlayer[(m->attacker==NULL)?2:m->attacker->place],
           d_symPlayer[(m->defender==NULL)?2:m->defender->place],
           d_symPlayer[(m->dealer  ==NULL)?2:m->dealer  ->place]);
    printf(".> Desk: (%d) [%s]\n[", m->desk.count, d_symTrump[m->desk.trump]);
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
    }
    printf("= = = = = = = = = = durView_match.\n");
}
