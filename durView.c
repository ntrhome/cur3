#include <stdio.h>
#include <locale.h>
#include <wchar.h>

#include "durView.h"
#include "durModel.h"

#define DUR_COLOR_F256(f) "\033[1m\033[38;5;" #f "m"
static char *colorReset = "\033[0m";
static char *colorSuits[]         = {DUR_COLOR_F256(254),//15
                                    DUR_COLOR_F256(51),//14
                                    DUR_COLOR_F256(202),
                                    DUR_COLOR_F256(196)};
static char *suits  = "scdh";
static char *ranks  = "6789XJQKA";
static char *places = "+LRABCDEFGHIJKL-";
static char *colorPlaces[]         = {DUR_COLOR_F256(196),
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
                                      DUR_COLOR_F256(196)};
static char *players[]              = {"LEFT ","RIGHT"};


static void outPlayer(dur_s_field *f, int player) {
    for (int i = 0; i < f->player[player].count-1; ++i) { //suit-sorter
        for (int j = i+1; j < f->player[player].count; ++j) {
            if(f->desk.card[f->player[player].desk[i]] > f->desk.card[f->player[player].desk[j]]) {
                int depot = f->player[player].desk[j];
                f->player[player].desk[j] = f->player[player].desk[i];
                f->player[player].desk[i] = depot;
            }
        }
    }
    printf("| - %s%s%s: (%d|%s%c%s)\n", colorPlaces[player + 1], players[player], colorReset, f->player[player].count, colorSuits[f->desk.trump], suits[f->desk.trump], colorReset);

    printf("[");
    char *s = "";
    for (int i = 0; i < f->player[player].count; ++i) {
        int card = f->desk.card[f->player[player].desk[i]];
        printf("%s%s%c%c%s", s, colorSuits[card / DUR_RANKS], ranks[card % DUR_RANKS], suits[card / DUR_RANKS], colorReset);
        s = ".";
    }
    s = " ";
    if(f->player[player].count > 16 ) { s = "\n"; }
    printf("] /%s[", s);
    for (int i = 0; i < f->player[player].count-1; ++i) { //rank-sorter
        for (int j = i+1; j < f->player[player].count; ++j) {
            if(f->desk.card[f->player[player].desk[i]] % DUR_RANKS > f->desk.card[f->player[player].desk[j]] % DUR_RANKS) {
                int depot = f->player[player].desk[j];
                f->player[player].desk[j] = f->player[player].desk[i];
                f->player[player].desk[i] = depot;
            }
        }
    }
    for (int i = 0; i < f->player[player].count-1; ++i) { //suite correcting
        for (int j = i+1; j < f->player[player].count; ++j) {
            if(
                (f->desk.card[f->player[player].desk[i]] % DUR_RANKS == f->desk.card[f->player[player].desk[j]] % DUR_RANKS)
                &
                (f->desk.card[f->player[player].desk[i]] / DUR_RANKS > f->desk.card[f->player[player].desk[j]] / DUR_RANKS)
            ) {
                int depot = f->player[player].desk[j];
                f->player[player].desk[j] = f->player[player].desk[i];
                f->player[player].desk[i] = depot;
            }
        }
    }
    s = "";
    for (int i = 0; i < f->player[player].count; ++i) {
        int card = f->desk.card[f->player[player].desk[i]];
        printf("%s%s%c%c%s", s, colorSuits[card / DUR_RANKS], ranks[card % DUR_RANKS], suits[card / DUR_RANKS], colorReset);
        s = ".";
    }
    printf("].\n");
}

#ifdef DUR_DEBUG
static void outDesk(dur_s_desk *d) {
    printf("| - DESK:  (%d|%s%c%s)\n[", d->head, colorSuits[d->trump], suits[d->trump], colorReset);
    char *s = "";
    for(int i = 0; i < DUR_CARDS; ++i){
        printf("%s%s%c%c%s", s, colorSuits[d->card[i] / DUR_RANKS], ranks[d->card[i] % DUR_RANKS], suits[d->card[i] / DUR_RANKS], colorReset);
        s = ".";
    }
    printf("]\n[");
    s = "";
    for(int i = 0; i < DUR_CARDS; ++i){
        printf("%s%s%c%c%s", s, colorPlaces[d->place[i] + 1], places[d->place[i] + 1], places[d->place[i] + 1], colorReset);
        s = ".";
    }
    printf("].\n");
}

static void outHistory(dur_s_field *f) {
    printf("| - HISTORY:\n[");
    char *s = "";
    for(int i = 0; i < f->history.count; ++i){
        int card = f->desk.card[f->history.desk[i]];
        printf("%s%s%c%c%s", s, colorSuits[card / DUR_RANKS], ranks[card % DUR_RANKS], suits[card / DUR_RANKS], colorReset);
        s = ".";
    }
    printf("]\n[");
    s = "";
    for(int i = 0; i < f->history.count; ++i){
        int place = f->history.place[i] + 1;
        printf("%s%s%c%c%s", s, colorPlaces[place], places[place], places[place], colorReset);
        s = ".";
    }
    printf("].\n");
}

void admin(dur_s_game *g) {
    setlocale(LC_ALL, "");
    printf("=========== ADMIN ===========\n");
    printf("| - MATCH: stage = %d, score = [%d:%d].\n", g->round.field.stage, g->score[DUR_LEFT], g->score[DUR_RIGHT]);
    printf("| - GAME:  winner = %d.\n", g->winner);
    printf("| - ROUND: dealer = %d, attacker = %d.\n", g->round.dealer, g->round.attacker);
    outDesk(&g->round.field.desk);
    outPlayer(&g->round.field, DUR_LEFT);
    outPlayer(&g->round.field, DUR_RIGHT);
    outHistory(&g->round.field);
    //ToDo
    //field
    printf("=========== ===== ===========\n");
}

void bu(char *s, int i, int j) {
    printf("!!! BU !!!: %s, %d, %d.\n", s, i, j);
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
