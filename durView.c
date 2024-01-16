#include <stdio.h>
#include "durView.h"

static char *suits  = "scdh";
static char *ranks  = "6789XJQKA";
#define DUR_COLOR_F256(f) "\033[1m\033[38;5;" #f "m" //foreground
static char *colorReset = "\033[0m";
static char *colorSuits[] = {
        DUR_COLOR_F256(13),//15//51//13
        DUR_COLOR_F256(4),//14//254//13
        DUR_COLOR_F256(208),
        DUR_COLOR_F256(196)
};
static char *colorPlaces[] = {
        DUR_COLOR_F256(196),
        DUR_COLOR_F256(4),//LEFT//15//254
        DUR_COLOR_F256(2),//RIGHT//7//244
        DUR_COLOR_F256(39),
        DUR_COLOR_F256(40),
        DUR_COLOR_F256(75),
        DUR_COLOR_F256(76),
        DUR_COLOR_F256(111),
        DUR_COLOR_F256(112),
        DUR_COLOR_F256(147),
        DUR_COLOR_F256(148),
        DUR_COLOR_F256(183),
        DUR_COLOR_F256(184),
        DUR_COLOR_F256(219),
        DUR_COLOR_F256(220),
        DUR_COLOR_F256(196)
};

void durView(sBoard *b) {
    printf("durView\n");
//    switch (b->stage) {
//        case es_attackView:
//            break;
//    }
}

static void durViewPlayer(sPlayer *p, sDesk *d) {
    printf("Player '%-5s':  (%2d) [%s%c%s]\n[", p->name, p->count, colorSuits[d->trump], suits[d->trump], colorReset);
    for (int i = 0; i < p->count-1; ++i) { //suit-sorter
        for (int j = i+1; j < p->count; ++j) {
            if(d->card[p->desk[i]] > d->card[p->desk[j]]) {
                int depot = p->desk[j];
                p->desk[j] = p->desk[i];
                p->desk[i] = depot;
            }
        }
    }
    durView_outPlayerDesk(p,d);
    printf("] /%s[", (p->count > 16 ) ? "\n" : " ");
    for (int i = 0; i < p->count-1; ++i) { //rank-sorter
        for (int j = i+1; j < p->count; ++j) {
            if( d->card[p->desk[i]] % ed_ranks > d->card[p->desk[j]] % ed_ranks
                |
                d->card[p->desk[i]] % ed_ranks == d->card[p->desk[j]] % ed_ranks
                &
                d->card[p->desk[i]] > d->card[p->desk[j]]
            ) {
                int depot = p->desk[j];
                p->desk[j] = p->desk[i];
                p->desk[i] = depot;
            }
        }
    }
    durView_outPlayerDesk(p,d);
    printf("].\n");
}

#ifdef DUR_DEBUG

static void durdbgViewDesk(sDesk *d) {
    printf("Desk:            (%2d) [%s%c%s]\n[", d->count, colorSuits[d->trump], suits[d->trump], colorReset);
    char *s = "";
    for(int i = 0; i < ed_cards; ++i){
        printf("%s%s%c%c%s", s, colorSuits[d->card[i] / ed_ranks], ranks[d->card[i] % ed_ranks], suits[d->card[i] / ed_ranks], colorReset);
        s = ".";
    }
    printf("].\n");
}

void durdbgViewBoard(sBoard *b) {
    printf("= = = = = = = = = = = = dur_dbg_viewBoard = = = = = = = = = = = (match_id = %p)\n", b);
    durdbgViewDesk(&b->desk);
    durViewPlayer(&b->left, &b->desk);
    durViewPlayer(&b->right, &b->desk);
//    printf("Score:           [ %d : %d ]\n", b->left.score, b->right.score);
//    durView_outPlayer(&b->left, &b->desk);
//    durView_outPlayer(&b->right, &b->desk);
//    char *s = "Nemo";
//    printf("winner->%s, attacker->%s, dealer->%s\n", (b->winner == NULL)?s:b->winner->name, (b->attacker == NULL)?s:b->attacker->name, (b->dealer == NULL)?s:b->dealer->name);



}


//static void durView_outPlayerDesk(sPlayer *p, sDesk *d) {
//    char *s = "";
//    for (int i = 0; i < p->count; ++i) {
//        int card = d->card[p->desk[i]];
//        printf("%s%s%c%c%s", s, colorSuits[card / ed_ranks], ranks[card % ed_ranks], suits[card / ed_ranks], colorReset);
//        s = ".";
//    }
//}

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

//https://en.wikipedia.org/wiki/Playing_cards_in_Unicode

//https://packagecontrol.io/packages/ANSIescape
//https://habr.com/ru/companies/macloud/articles/558316/
//https://overcoder.net/q/40194/раскраска-текста-в-консоли-с-c

//https://en.wikipedia.org/wiki/ANSI_escape_code#3-bit_and_4-bit


// = = = = = = = = = = = = = = = = = = = = = = = =
//static wchar_t suits[]   = L"♠♣♦♥";
//static wchar_t cards[]   = L"🂦🂧🂨🂩🂪🂫🂭🂮🂡🃖🃗🃘🃙🃚🃛🃝🃞🃑🃆🃇🃈🃉🃊🃋🃍🃎🃁🂶🂷🂸🂹🂺🂻🂽🂾🂱";
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
//static wchar_t places[]  = L"🂠🂬🂼🂢🂣🃒🃓🃂🃃🂲🂳🂤🂥🃔🃕🃟"; //🃏
