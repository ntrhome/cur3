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
static char *places = "+01ABCDEFGHIJKL-";
static char *colorPlaces[]         = {DUR_COLOR_F256(196),
                                      DUR_COLOR_F256(244),//7
                                      DUR_COLOR_F256(254),//15
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



//static void outPlayer(dur_s_round *r, int player) {
//    char *color;
//    for (int i = 0; i < r->player[player].count; ++i) {
//        int card = r->desk.card[r->player[player].desk[i]];
//        color = ((card / (2*DUR_RANKS)) == 0) ? colorWhite : colorRed;
//        printf("%s%lc ", color, cards[card]); // завершающий пробел нужен для вывода в терминал - там иначе карты с цветом накладываются
//    }
//    color = (r->desk.trump / 2 == 0) ? colorWhite : colorRed;
//    printf("%s(%d/%d) [%s%lc%s]", colorReset, r->player[player].count, DUR_NORMAL, color, suits[r->desk.trump], colorReset);
//}
//
#ifdef DUR_DEBUG
static void outDesk(dur_s_desk *d) {
    printf("  [");
    char *s = "";
    for(int i = 0; i < DUR_CARDS; ++i){
        printf("%s%s%s%c%c", colorReset, s, colorSuits[d->card[i] / DUR_RANKS], ranks[d->card[i] % DUR_RANKS], suits[d->card[i] / DUR_RANKS]);
        s = ".";
    }
    printf("%s]\n  [",colorReset);
    s = "";
    for(int i = 0; i < DUR_CARDS; ++i){
        printf("%s%s%s%c%c", colorReset, s, colorPlaces[d->place[i] + 1], places[d->place[i] + 1], places[d->place[i] + 1]);
        s = ".";
    }
    printf("%s].\n", colorReset);
}

void admin(dur_s_game *g) {
    setlocale(LC_ALL, "");
    printf("=========== ADMIN ===========\n");
    printf("|-MATCH: stage = %d, score = [%d:%d].\n", g->round.field.stage, g->score[DUR_WHITE], g->score[DUR_BLACK]);
    printf("|-GAME:  winner = %d.\n", g->winner);
    printf("|-ROUND: dealer = %d, attacker = %d.\n", g->round.dealer, g->round.attacker);
    printf("|-DESK:  (%d/%d) [%s%c%s]\n", g->round.field.desk.head, DUR_CARDS, colorSuits[g->round.field.desk.trump], suits[g->round.field.desk.trump], colorReset);
    outDesk(&g->round.field.desk);
    //ToDo
//    printf("| - %sWHITE%s: ", colorWHITE, colorReset);
//    outPlayer(&d->match.game.round, 0);
//    printf(".\n");
//    printf("| - %sBLACK%s: ", colorBLACK, colorReset);
//    outPlayer(&d->match.game.round, 1);
//    printf(".\n");
//    printf("| - HISTORY:\n");
//    for(int i = 0; i < d->match.game.round.history.count; ++i){
//        int card = d->match.game.round.desk.card[d->match.game.round.history.desk[i]];
//        char *color = ((card / (2*DUR_RANKS)) == 0) ? colorWhite : colorRed;
//        printf("%s%lc ", color, cards[card]); // завершающий пробел нужен для вывода в терминал - там иначе карты с цветом накладываются
//    }
//    printf("%s.\n", colorReset);
//    for(int i = 0; i < d->match.game.round.history.count; ++i){
//        char *color;
//        switch(d->match.game.round.history.place[i]){
//            case DUR_WHITE: color = colorWHITE; break;
//            case DUR_BLACK: color = colorBLACK; break;
//            default: color = colorGrey;
//        }
//        printf("%s%lc ", color, places[d->match.game.round.history.place[i] + 1]);
//    }
//    printf("%s.\n", colorReset);
//
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
