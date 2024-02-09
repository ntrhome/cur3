#include <stdio.h>
#include "durView.h"

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

static void view_score(const sBoard *b) {
    printf(".> Score: (%d) [%d:%d].\n", b->game, b->left.score, b->right.score);
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
            int card = suit * ed_ranks + rank;
            if (d->place[card] == p->place) {
                printf("%s%s", s, d_symCard[card]);
                s = ".";
            }
        }
    }
    printf("].\n");
}

static void view_fight(const sBoard *b) {
    printf(".> Fight: (%d) [%s]\n - attack: [", b->fight, d_symTrump[b->desk.trump]);
    char *s = "";
    for (int i = b->history.count - b->fight; i < b->history.count; i+=2) {
        printf("%s%s", s, d_symCard[b->history.card[i]]);
        s = ".";
    }
    printf("]\n - defend: [");
    s = "";
    for (int i = b->history.count - b->fight + 1; i < b->history.count; i+=2) {
        printf("%s%s", s, d_symCard[b->history.card[i]]);
        s = ".";
    }
    printf("].\n");
}

static void view_prompt(const sBoard *b) {
    char *s1, *s2;
    if (b->stage == es_attackView) {
        s1 = "Attack";
        s2 = (b->fight) ? "'e'-enough, " : "";
    } else { //es_defendView
        s1 = "Defend";
        s2 = "'t'-take, ";
    }
    printf("?> %s. Type a card (e.g. '6s', 'Ah') or command (%s'q'-quit): ", s1, s2);
}


//static void durView_prompt(const sBoard *b) {
//    char *s1, *s2;
//    if (b->stage == es_attackView) {
//        if (b->attack.count == 0) {
//            s1 = "First attack"; s2 = "";
//        } else {
//            s1 = "Attack"; s2 = "'e'-enough, ";
//        }
//    } else { //es_defendView
//        if (b->defend.count == ed_normal-1) {
//            s1 = "Last defend"; s2 = "'t'-take, ";
//        } else {
//            s1 = "Defend"; s2 = "'t'-take, ";
//        }
//    }
//    printf("?> %s. Type a card (e.g. '6s', 'Ah') or command (%s'q'-quit): ", s1, s2);
//}
static void view_attack(const sBoard *b) {
//    durView_symPlayer(b->attacker);
    view_fight((const sBoard *)b);
    view_prompt((const sBoard *)b);
}
//static void durView_defend(const sBoard *b) {
//    durView_symPlayer(b->defender);
//    durView_fight((const sBoard *)b);
//    durView_prompt((const sBoard *)b);
//}

static void view_newGame(const sBoard *b) {
    printf("!> - - - Ta-dam! New game. - - -\n");
    view_score(b);
}

static void view_newFight(const sBoard *b) {
    printf(".> New fight. Attacker: %s player.\n", d_symPlayer[b->attacker->place]);
}

void durView(sBoard *b) { //in durView.c only this f have not const sBoard (because b->stage)
    //printf("durView\n"); //dbg
    switch (b->stage) { //пока что без потоков и надобности синхронизации - пытаемся отработать на общих стейджах
        case es_newGameView:
            view_newGame((const sBoard *)b);
            b->stage = es_newFight;
            break;
        case es_newFightView:
            view_newFight((const sBoard *)b);
            b->stage = es_attack;
            break;
        case es_attackView:
            view_attack((const sBoard *)b);
            b->stage = es_attackControl;
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

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#ifdef DUR_DEBUG

static void dbgView_history(const sHistory *h) {
    printf(".> History: (%d)\n[", h->count);
    char *s = "";
    for (int i = 0; i < h->count; ++i) {
        printf("%s%s", s, d_symCard[h->card[i]]);
        s = ".";
    }
    printf("].\n");
}
static void dbgView_desk(const sDesk *d) { //ASCII
    printf(".> Desk: (%d) [%s]\n[", d->count, d_symTrump[d->trump]);
    char *s = ""; for (int i = 0; i < ed_cards; ++i) printf("%s%02d", s, i), s = "-"; printf("]-position\n[");
    s = ""; for (int i = 0; i < ed_cards; ++i) printf("%s%s", s, d_symCard[d->order[i]]), s = "."; printf("]-order\n[");
    s = ""; for (int i = 0; i < ed_cards; ++i) printf("%s%02d", s, d->index[i]), s = "."; printf("]-index\n[");
    s = ""; for (int i = 0; i < ed_cards; ++i) printf("%s%s", s, d_symPlace[d->place[d->order[i]]]), s = "."; printf("]-place.\n");
}
void dbgView_board(const sBoard *b) {
    printf("= = = = = = = = = = dbgView_board = (sizeof(sBoard)=%lu, board_id = %p, stage = %d)\n", sizeof(sBoard), b, b->stage);
    dbgView_desk(&b->desk);
    view_score(b);
    printf(".> Turn: winner=%s, attacker=%s, defender=%s, dealer=%s.\n",
           d_symPlayer[(b->winner==NULL)?2:b->winner->place],
           d_symPlayer[(b->attacker==NULL)?2:b->attacker->place],
           d_symPlayer[(b->defender==NULL)?2:b->defender->place],
           d_symPlayer[(b->dealer==NULL)?2:b->dealer->place]);
    view_player(&b->desk, &b->left);
    view_player(&b->desk, &b->right);
    view_fight(b);
    dbgView_history(&b->history);
    printf("= = = = = = = = = = dbgView_board.");
}

void dbgView_msg(char *s) {
    printf("%s", s);
    fflush(stdout);
}

#endif //DUR_DEBUG


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
