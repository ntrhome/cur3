#include <stdio.h>
#include <locale.h>
#include <wchar.h>

#include "durView.h"
#include "durModel.h"

static wchar_t suits[]   = L"♠♣♦♥";
static wchar_t cards[]   = L"🂦🂧🂨🂩🂪🂫🂭🂮🂡🃖🃗🃘🃙🃚🃛🃝🃞🃑🃆🃇🃈🃉🃊🃋🃍🃎🃁🂶🂷🂸🂹🂺🂻🂽🂾🂱";
static char colorReset[] = "\033[0m";
static char colorRed[]   = "\033[31m"; //"1;"=bright - bordo "\033[1;31m", red: "\x1b[31m"
static char colorGreen[] = "\033[32m";
static char colorYellow[]= "\033[33m";
static char colorBlue[]  = "\033[34m";
static char colorPurple[]= "\033[35m";
static char colorCyan[]  = "\033[36m";
static char colorWhite[] = "\033[37m";
static char colorGrey[]  = "\033[90m";
static char colorWHITE[] = "\033[30;107m"; //Black / Bright White
static char colorBLACK[] = "\033[97;40m";  //Bright White / Black
static wchar_t places[]  = L"🂠🂬🂼🂢🂣🃒🃓🃂🃃🂲🂳🂤🂥🃔🃕🃟"; //🃏

static void outPlayer(dur_s_round *r, int player) {
    char *color;
    for (int i = 0; i < r->player[player].count; ++i) {
        int card = r->desk.card[r->player[player].desk[i]];
        color = ((card / (2*DUR_RANKS)) == 0) ? colorWhite : colorRed;
        printf("%s%lc ", color, cards[card]); // завершающий пробел нужен для вывода в терминал - там иначе карты с цветом накладываются
    }
    color = (r->desk.trump / 2 == 0) ? colorWhite : colorRed;
    printf("%s(%d/%d) [%s%lc%s]", colorReset, r->player[player].count, DUR_NORMAL, color, suits[r->desk.trump], colorReset);
}

#ifdef DUR_DEBUG
static void outDesk(dur_s_desk *d) {
    char *color;
    for(int i = 0; i < DUR_CARDS; ++i){
        color = ((d->card[i] / (2*DUR_RANKS)) == 0) ? colorWhite : colorRed;
        printf("%s%lc ", color, cards[d->card[i]]); // завершающий пробел нужен для вывода в терминал - там иначе карты с цветом накладываются
    }
    color = (d->trump / 2 == 0) ? colorWhite : colorRed;
    printf("%s(%d/%d) [%s%lc%s]\n", colorReset, d->head, DUR_CARDS, color, suits[d->trump], colorReset);
    for(int i = 0; i < DUR_CARDS; ++i){
        switch(d->place[i]){
            case DUR_WHITE: color = colorWHITE; break;
            case DUR_BLACK: color = colorBLACK; break;
            default: color = colorGrey;
        }
        printf("%s%lc ", color, places[d->place[i] + 1]);
    }
    printf("%s.\n", colorReset);
}

void admin(dur_s *d) {
    setlocale(LC_ALL, "");
    printf("=========== ADMIN ===========\n");
    printf("| - DUR:   .\n");
    printf("| - MATCH: stage = %d, score = [%d:%d].\n", d->match.stage, d->match.score[0], d->match.score[1]);
    printf("| - GAME:  winner = %d.\n", d->match.game.winner);
    printf("| - ROUND: dealer = %d, attacker = %d.\n", d->match.game.round.dealer, d->match.game.round.attacker);
    printf("| - DESK:  \n");
    outDesk(&d->match.game.round.desk);
    printf("| - %sWHITE%s: ", colorWHITE, colorReset);
    outPlayer(&d->match.game.round, 0);
    printf(".\n");
    printf("| - %sBLACK%s: ", colorBLACK, colorReset);
    outPlayer(&d->match.game.round, 1);
    printf(".\n");
    printf("| - HISTORY:\n");
    for(int i = 0; i < d->match.game.round.history.count; ++i){
        int card = d->match.game.round.desk.card[d->match.game.round.history.desk[i]];
        char *color = ((card / (2*DUR_RANKS)) == 0) ? colorWhite : colorRed;
        printf("%s%lc ", color, cards[card]); // завершающий пробел нужен для вывода в терминал - там иначе карты с цветом накладываются
    }
    printf("%s.\n", colorReset);
    for(int i = 0; i < d->match.game.round.history.count; ++i){
        char *color;
        switch(d->match.game.round.history.place[i]){
            case DUR_WHITE: color = colorWHITE; break;
            case DUR_BLACK: color = colorBLACK; break;
            default: color = colorGrey;
        }
        printf("%s%lc ", color, places[d->match.game.round.history.place[i] + 1]);
    }
    printf("%s.\n", colorReset);

    printf("=========== ===== ===========\n");
}

void bu(char *s, int i, int j) {
    printf("!!!!!!! BU !!!!!!!: %s, %d, %d\n", s, i, j);
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