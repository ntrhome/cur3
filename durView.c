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

void durView_cmdWrong(char *s) {
    if (s == NULL) {
        s = "Wrong card or command was typed. Repeat please.";
    }
    printf("!> %s\n", s);
}

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


// = = = = = = = = = = = = = = = = = = = = = = = =
// = = = = = = = = = = = = = = = = = = = = = = = =

// "6789XJQKA"
// "scdh" (Spades, Clubs, Diamonds, Hearts)
// "ptbc" (пики, трефы, бубны, червы)
// "&#@$"
// "♠♣♦♥"
// "♤♧♢♧"
// "♠️♣️♦️♥️"
// 🂢🂣🂤🂥🂦🂧🂨🂩🂪🂫🂭🂮🂡
// 🃒🃓🃔🃕🃖🃗🃘🃙🃚🃛🃝🃞🃑
// 🃂🃃🃄🃅🃆🃇🃈🃉🃊🃋🃍🃎🃁
// 🂲🂳🂴🂵🂶🂷🂸🂹🂺🂻🂽🂾🂱
//https://symbl.cc/en/unicode/table/#playing-cards
//https://symbl.cc/ru/unicode/table/#playing-cards
//https://en.wikipedia.org/wiki/Playing_cards_in_Unicode
//https://en.cppreference.com/w/c/language/string_literal

//https://packagecontrol.io/packages/ANSIescape
//https://habr.com/ru/companies/macloud/articles/558316/
//https://overcoder.net/q/40194/раскраска-текста-в-консоли-с-c
//https://en.wikipedia.org/wiki/ANSI_escape_code#3-bit_and_4-bit

// = = = = = = = = = = = = = = = = = = = = = = = =
//#include <locale.h>
//#include <uchar.h> //for char32_t (since C11)
//#include <wchar.h>
//static char32_t *suitsUNICODE = U"♠♣♦♥";
//static char32_t *cardsUNICODE = U"🂦🂧🂨🂩🂪🂫🂭🂮🂡🃖🃗🃘🃙🃚🃛🃝🃞🃑🃆🃇🃈🃉🃊🃋🃍🃎🃁🂶🂷🂸🂹🂺🂻🂽🂾🂱";
//static char32_t *placesUNICODE= L"🂠🂬🂼🂢🂣🃒🃓🃂🃃🂲🂳🂤🂥🃔🃕🃟"; //🃏
//static char colorReset[] = "\033[0m";
//static char colorRed[]   = "\033[31m"; //"1;"=bright - bordo "\033[1;31m", red: "\x1b[31m"
//static char colorGreen[] = "\033[32m";
//static char colorYellow[]= "\033[33m";
//static char colorBlue[]  = "\033[34m";
//static char colorPurple[]= "\033[35m";
//static char colorCyan[]  = "\033[36m";
//static char colorWhite[] = "\033[37m";
//static char colorGrey[]  = "\033[90m";
//static char colorWHITE[] = "\033[30;107m"; //Black / Bright White
//static char colorBLACK[] = "\033[97;40m";  //Bright White / Black
// static void durDbgViewBoard_Desk(sDesk *d) {//Unicode
//     setlocale(LC_ALL,"");
//     printf(".> Desk: (%d) [%s%C%s]\n", d->count, colorSuits[d->trump], suitsU[d->trump], colorReset);
//     static char32_t s[] = U"  ";
//     for(int i = 0; i < ed_cards; ++i){
//         s[0] = cardsU[d->card[i]];
//         printf("%s%S%s", colorSuits[d->card[i] / ed_ranks], (wchar_t *)s, colorReset);
//     }//printf(".\n0102030405060708091011121314151617181920\n");
// }

//static const char *const colorPlace[] = {
//    DURVIEW_COLOR_F256(4),//LEFT//15//254
//    DURVIEW_COLOR_F256(2),//RIGHT//7//244
//    DURVIEW_COLOR_F256(39),//
//    DURVIEW_COLOR_F256(40),//
//    DURVIEW_COLOR_F256(75),//
//    DURVIEW_COLOR_F256(76),//
//    DURVIEW_COLOR_F256(111),//
//    DURVIEW_COLOR_F256(112),//
//    DURVIEW_COLOR_F256(147),//
//    DURVIEW_COLOR_F256(148),//
//    DURVIEW_COLOR_F256(183),//
//    DURVIEW_COLOR_F256(184),//
//    DURVIEW_COLOR_F256(219),//
//    DURVIEW_COLOR_F256(220),//
//    DURVIEW_COLOR_F256(196),//PILE
//    DURVIEW_COLOR_F256(196)//DESK
//};


//#define COLOR_F256(f) "\033[1m\033[38;5;" #f "m" //foreground
//static const char *const d_colorReset   = "\033[0m";
//static const char *const d_colorSuit[]  = {
//        COLOR_F256(127),
//        COLOR_F256(27),
//        COLOR_F256(208),
//        COLOR_F256(196),
//        COLOR_F256(8),   //for place (ep_pile)
//        "\033[0m"        //for place (ep_desk)
//};
