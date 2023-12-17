#include <stdio.h>
#include <locale.h>
#include <wchar.h>

#include "durView.h"

static wchar_t suits[]   = L"♠♣♦♥";
static wchar_t cards[]   = L"🂦🂧🂨🂩🂪🂫🂭🂮🂡🃖🃗🃘🃙🃚🃛🃝🃞🃑🃆🃇🃈🃉🃊🃋🃍🃎🃁🂶🂷🂸🂹🂺🂻🂽🂾🂱";
static char colorWhite[] = "\033[37m";
static char colorRed[]   = "\033[1;31m"; //"1;"=bright - bordo
static char colorReset[] = "\033[0m";
static wchar_t places[]  = L"🃏🂬🂼🂢🂣🃒🃓🃂🃃🂲🂳🂤🂥🃔🃕🂠"; //🃟

void admin(DUR_S_MATCH match) {
    setlocale(LC_ALL, "");
    printf("=========== ADMIN ===========\n");
    printf("| - - - - - match - - - - - - ");
    printf("| stage = %2d ", match.stage);
    char *s = "";
    printf("| score = [");
    for(int p = 0; p < DUR_PLAYERS; ++p) {
        printf("%s%d", s, match.score[p]);
        s=":";
    }
    printf("]\n");
    printf("| - - - - -  game - - - - - - ");
    printf("| winner = %d ", match.game.winner);
    printf("| dealer = %d ", match.game.fight.dealer);
    printf("| attacker = %d\n", match.game.fight.attacker);
    printf("| DESK  = ");
    char *color;
    for(int i=0; i<DUR_CARDS; ++i){
        int card = match.game.desk.card[i];
        color = ((card / (2*DUR_RANKS)) == 0) ? colorWhite : colorRed;
        printf("%s%lc", color, cards[card]);
    }
    color = ((match.game.desk.trump / 2) == 0) ? colorWhite : colorRed;
    printf("%s (%d) [%s%lc%s]\n", colorReset, match.game.desk.count, color, suits[match.game.desk.trump], colorReset);
    printf("| PLACE = ");
    for(int i=0; i<DUR_CARDS; ++i){
       printf("%lc", places[match.game.desk.place[i]+1]); //"+1" потому как "enum dur_e_place {DUR_E_PLACE_DESK=-1, ...""
    }
    printf("\n");
    printf("| HESK  = ");
    for(int i=0; i<match.game.history.count; ++i){
        int card = match.game.desk.card[match.game.history.desk[i]];
        color = ((card / (2*DUR_RANKS)) == 0) ? colorWhite : colorRed;
        printf("%s%lc", color, cards[card]);
    }
    printf("%s\n", colorReset);
    printf("| HLACE = ");
    for(int i=0; i<match.game.history.count; ++i){
       printf("%lc", places[match.game.history.place[i]+1]); //"+1" потому как "enum dur_e_place {DUR_E_PLACE_DESK=-1, ...""
    }
    printf("\n");
    printf("========== ===== ===========\n");
}




//    int ch;
//printf("ch = ");
//fflush(stdout);
//ch = getchar();
//printf("%d\n", ch);
//printf("%c\n", (char) ch);


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
