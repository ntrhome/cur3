#include <stdio.h>
#include "durView.h"

extern int d_suit [ed_cards];
extern int d_rank [ed_cards];

static const char *const d_rankSymbol   = "6789XJQKA6789XJQKA6789XJQKA6789XJQKA";
static const char *const d_suitSymbol   = "ssssssssscccccccccdddddddddhhhhhhhhh"; //Spades, Clubs, Diamonds, Hearts
static const char *const d_symPlace     = "LRadPD";
static const char *const d_symPlayer[3] = {"Left", "Right", "NN"};

#define COLOR_F256(f) "\033[1m\033[38;5;" #f "m" //foreground
static const char *const d_colorReset   = "\033[0m";
static const char *const d_colorSuit[]  = {
        COLOR_F256(127),
        COLOR_F256(27),
        COLOR_F256(208),
        COLOR_F256(196),
        COLOR_F256(8),   //for place (ep_pile)
        "\033[0m"        //for place (ep_desk)
};

//static void durView_newGame(const sBoard *b) {
//    printf("%s\n", ".> !!!!!!!!!!!!!!!! New game !!!!!!!!!!!!!!!!");
//    printf(".> Score: [%d:%d].\n", b->left.score, b->right.score);
//}
//
//static void durView_newFight(const sBoard *b) {
//    printf(".> New fight. Attacker - %s player.\n", (b->attacker->place)?"Right":"Left");
//}

static void durView_player(const sPlayer *p, const sDesk *d) {
    printf("> Player %s: (%d)\n[", d_symPlayer[(p==NULL)?2:p->place], p->count);
    char *s = "";
    for (int i = 0; i < ed_cards; ++i) { //by suit
        if (d->place[i] == p->place) {
            printf("%s%s%c%c%s", s, d_colorSuit[d_suit[i]], d_rankSymbol[i], d_suitSymbol[i], d_colorReset);
            s = ".";
        }
    }
    printf("] /%s[", (p->count > 16 ) ? "\n" : " ");
    s = ""; //by ranks//
    for (int rank = 0; rank < ed_ranks; ++rank) {
        for (int suit = 0; suit < ed_suits; ++suit) {
            int i = suit * ed_ranks + rank;
            if (d->place[i] == p->place) {
                printf("%s%s%c%c%s", s, d_colorSuit[d_suit[i]], d_rankSymbol[i], d_suitSymbol[i], d_colorReset);
                s = ".";
            }
        }
    }
    printf("].\n");
}

//static void durView_fight(const sBoard *b) {
//    printf(".> Fight [%s%c%s]:\n        - attack: [", colorSuit[b->desk.trump], suits[b->desk.trump], colorReset);
//    durView_cards(b->attack.count, b->attack.card);
//    printf("]\n        - defend: [");
//    durView_cards(b->defend.count, b->defend.card);
//    printf("].\n");
//}
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
//static void durView_attack(const sBoard *b) {
//    durView_symPlayer(b->attacker);
//    durView_fight((const sBoard *)b);
//    durView_prompt((const sBoard *)b);
//}
//static void durView_defend(const sBoard *b) {
//    durView_symPlayer(b->defender);
//    durView_fight((const sBoard *)b);
//    durView_prompt((const sBoard *)b);
//}
//
//void durView(sBoard *b) { //in durView.c only this f have not const sBoard (because b->stage)
//    //printf("durView\n"); //dbg
//    switch (b->stage) {
//        case es_newGameView:
//            durView_newGame((const sBoard *)b);
//            b->stage = es_newFight;
//            break;
//        case es_newFightView:
//            durView_newFight((const sBoard *)b);
//            b->stage = es_attack;
//            break;
//        case es_attackView:
//            durView_attack((const sBoard *)b);
//            b->stage = es_attackControl;
//            break;
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
//    }
//}

//void durView_msg(char *s){
//    printf("%s", s);
//    fflush(stdout);
//}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#ifdef DUR_DEBUG

//static void durView_dbg_boardHistory(const sHistory *h) {
//    printf("> History: (%d)\n[", h->count);
//    durView_cards(h->count, h->card);
//    printf("]\n[");
//    char *s = "";
//    for (int i = 0; i < h->count; ++i) {
//        printf("%s%c%c", s, places[2 * h->place[i]], places[2 * h->place[i] + 1]);
//        s = ".";
//    }
//    printf("].\n");
//}

//static void cardOut(int card) {
//    printf("%s%c%c%s", d_colorSuit[d_suit[card]], d_rankSymbol[card], d_suitSymbol[card], d_colorReset);
//}

static void dbgView_boardDesk(const sDesk *d) { //ASCII
    printf("> Desk: (%d) [%s%c%s]\n[", d->count, d_colorSuit[d->trump], d_suitSymbol[d->trump], d_colorReset);
    char *s = ""; for (int i = 0; i < ed_cards; ++i) { printf("%s%02d", s, i); s = "-"; }
    printf("]-index.\n[");
    s = "";
    for (int i = 0; i < ed_cards; ++i) {
        printf("%s%s%c%c%s", s, d_colorSuit[d_suit[d->card[i]]], d_rankSymbol[d->card[i]], d_suitSymbol[d->card[i]], d_colorReset);
        s = ".";
    }
    printf("]-card[position].\n[");
    s = ""; for (int i = 0; i < ed_cards; ++i) { printf("%s%s%c%c%s", s, d_colorSuit[d->place[i]], d_symPlace[d->place[i]], d_symPlace[d->place[i]], d_colorReset); s = "."; }
    printf("]-place.\n[");
    s = ""; for (int i = 0; i < ed_cards; ++i) { printf("%s%02d", s, d->order[i]); s = "."; }
    printf("]-order[card].\n");
}
void dbgView_board(const sBoard *b) {
    printf("= = = = = = = = = = dbgView_board = (sizeof(sBoard)=%lu, match_id = %p, stage = %d)\n", sizeof(sBoard), b, b->stage);
    dbgView_boardDesk(&b->desk);
    printf("> Score: [%d:%d].\n", b->left.score, b->right.score);
    printf("> Turn: winner=%s, attacker=%s, defender=%s, dealer=%s\n",
           d_symPlayer[(b->winner==NULL)?2:b->winner->place],
           d_symPlayer[(b->attacker==NULL)?2:b->attacker->place],
           d_symPlayer[(b->defender==NULL)?2:b->defender->place],
           d_symPlayer[(b->dealer==NULL)?2:b->dealer->place]);
    durView_player(&b->left, &b->desk);
//    durView_symPlayer(&b->right, &b->desk);
//    durView_fight(b);
//    durView_dbg_boardHistory(&b->history);
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
//     printf("> Desk: (%d) [%s%C%s]\n", d->count, colorSuits[d->trump], suitsU[d->trump], colorReset);
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