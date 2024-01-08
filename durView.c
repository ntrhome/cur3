#include <stdio.h>
#include <locale.h>
#include <wchar.h>

#include "durView.h"
#include "durModel.h"

static char *suits  = "scdh";
static char *ranks  = "6789XJQKA";
static char *places = "+LRABCDEFGHIJKL-";
#define DUR_COLOR_F256(f) "\033[1m\033[38;5;" #f "m"
static char *colorReset = "\033[0m";
static char *colorSuits[]          = {DUR_COLOR_F256(51),//15
                                      DUR_COLOR_F256(254),//14
                                      DUR_COLOR_F256(208),
                                      DUR_COLOR_F256(196)
                                     };
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
                                      DUR_COLOR_F256(196)
                                     };
static char *playerNames[]         = {"LEFT ","RIGHT"};

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
    printf("| - %s%s%s:   (%d|%s%c%s)\n[", colorPlaces[player + 1], playerNames[player], colorReset, f->player[player].count, colorSuits[f->desk.trump], suits[f->desk.trump], colorReset);
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

static void outField(dur_s_field *f) {
    int fieldFrame = f->history.count - f->fieldHistory;
    int attacked = fieldFrame / 2 + fieldFrame % 2;
    int defended = fieldFrame / 2;
    printf("| - FIELD:   (%d/%d)\n|   attacked: [", attacked, defended);
    char *s = "";
    for(int i = f->fieldHistory; i < f->history.count; i+=2) {
        int card = f->desk.card[f->history.desk[i]];
        printf("%s%s%c%c%s", s, colorSuits[card / DUR_RANKS], ranks[card % DUR_RANKS], suits[card / DUR_RANKS], colorReset);
        s = ".";
    }
    printf("]\n|   defended: [");
    for(int i = f->fieldHistory+1; i < f->history.count; i+=2) {
        int card = f->desk.card[f->history.desk[i]];
        printf("%s%s%c%c%s", s, colorSuits[card / DUR_RANKS], ranks[card % DUR_RANKS], suits[card / DUR_RANKS], colorReset);
        s = ".";
    }
    printf("]\n");
}

void durView_step(dur_s_round *r) {
    int fieldFrame = r->field.history.count - r->field.fieldHistory;
    int player;
    char *msg;
    if(fieldFrame % 2 == 0) { //attack
        player = r->attacker;
        msg = "Your attack. Enter card (e.g. 6h, ac, Ac) or command (Qq - quit): ";
    } else { //defend
        player = !r->attacker;
        msg = "Your defend. Enter card (e.g. 6h, ac, Ac) or command (tt - take, Qq - quit): ";
    }
    printf("=============================\n");
    outPlayer(&r->field, player);
    outField(&r->field);
    printf("%s", msg);
}

#ifdef DUR_DEBUG
static void outDesk(dur_s_desk *d) {
    printf("| - DESK:    (%d|%s%c%s)\n[", d->head, colorSuits[d->trump], suits[d->trump], colorReset);
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
    printf("| - HISTORY: (%d/%d)\n[", f->history.count - f->fieldHistory, f->history.count);
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

void dbgAdmin(dur_s_game *g) {
    setlocale(LC_ALL, "");
    printf("=========== ADMIN ===========\n");
    printf("| - MATCH:   stage = %d, score = [%d:%d].\n", g->round.field.stage, g->score[DUR_LEFT], g->score[DUR_RIGHT]);
    printf("| - GAME:    winner = %d.\n", g->winner);
    printf("| - ROUND:   dealer = %d, attacker = %d.\n", g->round.dealer, g->round.attacker);
    outDesk(&g->round.field.desk);
    outPlayer(&g->round.field, DUR_LEFT);
    outPlayer(&g->round.field, DUR_RIGHT);
    outHistory(&g->round.field);
    //ToDo
    //field
    printf("=============================\n");
}

void dbgBu(char *s, int i, int j) {
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
