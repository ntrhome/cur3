#include <stdio.h>
#include <locale.h>
#include <wchar.h>

#include "durView.h"
#include "durModel.h"

static wchar_t suits[]   = L"♠♣♦♥";
static wchar_t cards[]   = L"🂦🂧🂨🂩🂪🂫🂭🂮🂡🃖🃗🃘🃙🃚🃛🃝🃞🃑🃆🃇🃈🃉🃊🃋🃍🃎🃁🂶🂷🂸🂹🂺🂻🂽🂾🂱";
static char colorWhite[] = "\033[37m";
static char colorRed[]   = "\033[1;31m"; //"1;"=bright - bordo
static char colorReset[] = "\033[0m";
static wchar_t places[]  = L"🃏🂬🂼🂢🂣🃒🃓🃂🃃🂲🂳🂤🂥🃔🃕🂠"; //🃟

static void outPack(int *pack, int count, int trump) {
    char *color;
    for(int i = 0; i < count; ++i){
        int card = pack[i];
        color = ((card / (2*DUR_RANKS)) == 0) ? colorWhite : colorRed;
        printf("%s%lc", color, cards[card]);
    }
    color = (trump / 2 == 0) ? colorWhite : colorRed;
    printf("%s (%d) [%s%lc%s]\n", colorReset, count, color, suits[trump], colorReset);
}

void admin(dur_s_match *m) {
    setlocale(LC_ALL, "");
    printf("=========== ADMIN ===========\n");
    printf("| - MATCH: stage = %d, score = [%d:%d]\n", m->stage, m->score[0], m->score[1]);
    printf("| - GAME:  winner = %d\n", m->game.winner);
    printf("| - ROUND: attacker = %d, dealer = %d\n", m->game.round.attacker, m->game.round.dealer);
    printf("| - FIRE:  \n");
    printf("| - DESK:  ");
    char *color;
    for(int i = 0; i < DUR_CARDS; ++i){
        int card = m->game.round.fire.desk.card[i];
        color = ((card / (2*DUR_RANKS)) == 0) ? colorWhite : colorRed;
        printf("%s%lc", color, cards[card]);
    }
    color = (m->game.round.fire.desk.trump / 2 == 0) ? colorWhite : colorRed;
    printf("%s (%d) [%s%lc%s]\n", colorReset, m->game.round.fire.desk.count, color, suits[m->game.round.fire.desk.trump], colorReset);
    printf("|  place:  ");
    for(int i = 0; i < DUR_CARDS; ++i){
        printf("%lc", places[m->game.round.fire.desk.place[i]+1]);
    }
    printf("\n");
    printf("| - HISTORY:\n");
    for(int i = 0; i < m->game.round.fire.history.count; ++i){
        int card = m->game.round.fire.desk.card[m->game.round.fire.history.desk[i]];
        color = ((card / (2*DUR_RANKS)) == 0) ? colorWhite : colorRed;
        printf("%s%lc", color, cards[card]);
    }
    printf("%s (%d)\n", colorReset, m->game.round.fire.history.count);
    for(int i = 0; i < m->game.round.fire.history.count; ++i){
        printf("%lc", places[m->game.round.fire.history.place[i]+1]);
    }
    printf("\n");
    printf("========== ===== ===========\n");
}

void bu(int i) {
    printf("!!!!!!! BU %d !!!!!!!\n", i);
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

//https://en.wikipedia.org/wiki/Playing_cards_in_Unicode

//https://packagecontrol.io/packages/ANSIescape
//https://habr.com/ru/companies/macloud/articles/558316/
//https://overcoder.net/q/40194/раскраска-текста-в-консоли-с-c
