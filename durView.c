#include <stdio.h>
#include "durView.h"

#define DUR_COLOR_F256(f) "\033[1m\033[38;5;" #f "m" //foreground
static const char *const colorReset = "\033[0m";
static const char *const colorSuit[] = {
    DUR_COLOR_F256(127),
    DUR_COLOR_F256(27),
    DUR_COLOR_F256(208),
    DUR_COLOR_F256(196)
};
//static const char *const colorPlace[] = {
//    DUR_COLOR_F256(4),//LEFT//15//254
//    DUR_COLOR_F256(2),//RIGHT//7//244
//    DUR_COLOR_F256(39),//
//    DUR_COLOR_F256(40),//
//    DUR_COLOR_F256(75),//
//    DUR_COLOR_F256(76),//
//    DUR_COLOR_F256(111),//
//    DUR_COLOR_F256(112),//
//    DUR_COLOR_F256(147),//
//    DUR_COLOR_F256(148),//
//    DUR_COLOR_F256(183),//
//    DUR_COLOR_F256(184),//
//    DUR_COLOR_F256(219),//
//    DUR_COLOR_F256(220),//
//    DUR_COLOR_F256(196),//PILE
//    DUR_COLOR_F256(196)//DESK
//};

static const char *const suit  = "scdh"; //Spades, Clubs, Diamonds, Hearts
static const char *const rank  = "6789XJQKA";
static const char *const place = "LLRRa1a2a3a4a5a6d1d2d3d4d5d6PPDD";

static void durView_score(const sBoard *b) {
    printf("> Score: [%d:%d].\n", b->left.score, b->right.score);
}

static void durView_cards(int count, const int *cards) {
    char *s = "";
    for (int position = 0; position < count; ++position) {
        printf("%s%s%c%c%s", s, colorSuit[cards[position] / ed_ranks], rank[cards[position] % ed_ranks], suit[cards[position] / ed_ranks], colorReset);
        s = ".";
    }
}
static void durView_player(const sField *f) {
    sPlayer *p = f->player;
    printf("> Player %s: (%d) [%s%c%s]\n[", (p->place) ? "Right" : "Left", p->count, colorSuit[f->trump], suit[f->trump], colorReset);
    int tmp[p->count];
    for (int i = 0; i < p->count; ++i) tmp[i] = p->card[i];
    for (int i = 0; i < p->count - 1; ++i) { //suit-rank-sorter
        for (int j = i+1; j < p->count; ++j) {
            if(tmp[i] > tmp[j]) {
                int depot = tmp[j];
                tmp[j] = tmp[i];
                tmp[i] = depot;
            }
        }
    }
    durView_cards(p->count, tmp);
    printf("] /%s[", (p->count > 16 ) ? "\n" : " ");
    for (int i = 0; i < p->count - 1; ++i) { //rank-suit-sorter
        for (int j = i+1; j < p->count; ++j) {
            if(tmp[i] % ed_ranks > tmp[j] % ed_ranks
                ||
                tmp[i] % ed_ranks == tmp[j] % ed_ranks
                &&
                tmp[i] > tmp[j])
            {
                int depot = tmp[j];
                tmp[j] = tmp[i];
                tmp[i] = depot;
            }
        }
    }
    durView_cards(p->count, tmp);
    printf("].\n");
}

static void durView_fight(const sField *f) {
    printf("> Fight - attack: [");
    durView_cards(f->attack.count, f->attack.card);
    printf("]\n        - defend: [");
    durView_cards(f->defend.count, f->defend.card);
    printf("].\n");
//    int flAttack = (f->player == f->attacker);
//    int flEnder = flAttack ? ((f->attack.count) ? 0:1) : ((f->defend.count == ed_normal-1) ? 1:0);
//    printf("%s. Choose card (e.g. '6s', 'Ad) or command ('Q'=quit):", (flAttack) ? ((flEnder) ? "First attack":"Attack") : ((flEnder) ? "Last defend":"Defend"));
////todo: неплохо, но наверное разбить на отдельные ф. атаки и защиты.
}

#ifdef DUR_DEBUG   // [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [

static void durDbgView_boardHistory(const sHistory *h) {
    printf("> History: (%d)\n[", h->count);
    durView_cards(h->count, h->card);
    printf("]\n[");
    char *s = "";
    for (int i = 0; i < h->count; ++i) {
        printf("%s%c%c", s, place[2 * h->place[i]], place[2 * h->place[i] + 1]);
        s = ".";
    }
    printf("].\n");
}

static void durDbgView_boardDesk(const sDesk *d) {//ASCII
    printf("> Desk: (%d) [%s%c%s]\n[", d->count, colorSuit[d->card[0] / ed_ranks], suit[d->card[0] / ed_ranks], colorReset);
    char *s = "";
    for (int i = 0; i < ed_cards; ++i) {
        printf("%s%02d", s, i);
        s = "-";
    }
    printf("].\n[");
    durView_cards(ed_cards, d->card);
    printf("].\n[");
    s = "";
    for (int i = 0; i < ed_cards; ++i) {
        printf("%s%02d", s, d->index[i]); //printf("%s%02d", s, d->index[d->card[i]]);
        s = ".";
    }
    printf("].\n");
}

void durDbgView_board(sBoard *b) {
    printf("= = = = = = = = = = durDbgView_board = (sizeof(sBoard)=%lu, match_id = %p, stage = %d)\n", sizeof(sBoard), b, b->stage);
    durDbgView_boardDesk(&b->desk);
    durView_score(b);
    printf("> Turn: winner = %s, attacker = %s\n",
           (b->field.winner == NULL) ? "Nemo" : (b->field.winner->place)?"Right":"Left",
           (b->field.attacker->place)?"Right":"Left");
    b->field.player = &b->left;
    durView_player(&b->field);
    b->field.player = &b->right;
    durView_player(&b->field);
    durView_fight(&b->field);
    durDbgView_boardHistory(&b->history);
}

#endif //DUR_DEBUG // ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ]

void durView(sBoard *b) {
    printf("durView\n");
//    switch (b->stage) {
//        case es_attackView:
//            printf("= = = = = = = = = = = = Attack:\n");
//            durViewPlayer(b->attacker, b);
//            durViewFight(b);
//            break;
//    }
}

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