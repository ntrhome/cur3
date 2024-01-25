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

static void durView_score(const sBoard *b) {
    printf("> Score: [%d:%d].\n", b->left.score, b->right.score);
}

static void durView_cards(int count, const int *card) {
    char *s = "";
    for (int position = 0; position < count; ++position) {
        printf("%s%s%c%c%s", s, colorSuit[card[position] / ed_ranks], ranks[card[position] % ed_ranks], suits[card[position] / ed_ranks], colorReset);
        s = ".";
    }
}
static void durView_playerCardsBySuit(const sPlayer *p) {
    char *s = "";
    for (int position = 0; position < ed_cards; ++position) {
        if (p->holder[position]) {
            printf("%s%s%c%c%s", s, colorSuit[position / ed_ranks], ranks[position % ed_ranks],
                   suits[position / ed_ranks], colorReset);
            s = ".";
        }
    }
}
static void durView_playerCardsByRank(const sPlayer *p) {
    char *s = "";
    for (int rank = 0; rank < ed_ranks; ++rank) {
        for (int suit = 0; suit < ed_suits; ++suit) {
            int position = suit * ed_ranks + rank;
            if (p->holder[position]) {
                printf("%s%s%c%c%s", s, colorSuit[position / ed_ranks], ranks[position % ed_ranks],
                       suits[position / ed_ranks], colorReset);
                s = ".";
            }
        }
    }
}
static void durView_player(const sPlayer *p, int trump) {
    printf("> Player %s: (%d) [%s%c%s]\n[", (p->place) ? "Right" : "Left", p->count, colorSuit[trump], suits[trump], colorReset);
    durView_playerCardsBySuit(p);
    printf("] /%s[", (p->count > 16 ) ? "\n" : " ");
    durView_playerCardsByRank(p);
    printf("].\n");
}

static void durView_fight(const sBoard *b) {
    printf("> Fight:\n        - attack: [");
    durView_cards(b->attack.count, b->attack.card);
    printf("]\n        - defend: [");
    durView_cards(b->defend.count, b->defend.card);
    printf("].\n");
}

static void durView_attack(const sBoard *b) {
    durView_player(b->attacker, b->desk.trump);
    durView_fight(b);

    char *s1, *s2;
    if (b->attack.count == 0) {
        s1 = "First"; s2 = "";
    } else {
        s1 = "Next";  s2 = "'e'-enough, ";
    }
    printf("%s attack. Type a card (e.g. '6s', 'Ah') or command (%s'q'-quit): ", s1, s2);
}

void durView_msg(char *s){
    printf("%s", s);
    fflush(stdout);
}

void durView(sBoard *b) { //in durView.c only this f have not const sBoard (because b->stage)
    //printf("durView\n"); //dbg
    switch (b->stage) {
        case es_attackView:
            durView_attack((const sBoard *)b);
            b->stage = es_attackControl;
            break;
    }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#ifdef DUR_DEBUG

static void durView_dbg_boardHistory(const sHistory *h) {
    printf("> History: (%d)\n[", h->count);
    durView_cards(h->count, h->card);
    printf("]\n[");
    char *s = "";
    for (int i = 0; i < h->count; ++i) {
        printf("%s%c%c", s, places[2 * h->place[i]], places[2 * h->place[i] + 1]);
        s = ".";
    }
    printf("].\n");
}

static void durView_dbg_boardDesk(const sDesk *d) {//ASCII
    printf("> Desk: (%d) [%s%c%s]\n[", d->count, colorSuit[d->trump], suits[d->trump], colorReset);
    char *s = "";
    for (int i = 0; i < ed_cards; ++i) {
        printf("%s%02d", s, i);
        s = "-";
    }
    printf("].\n[");
    durView_cards(ed_cards, d->card);
    printf("]-card.\n[");
    s = "";
    for (int i = 0; i < ed_cards; ++i) { //показывает где лежит карта (которая находилась бы здесь если бы не тасование)
        printf("%s%02d", s, d->position[i]); //printf("%s%02d", s, d->index[d->card[i]]);
        s = ".";
    }
    printf("]-index.\n");
}

void durView_dbg_board(const sBoard *b) {
    printf("= = = = = = = = = = durView_dbg_board = (sizeof(sBoard)=%lu, match_id = %p, stage = %d)\n", sizeof(sBoard), b, b->stage);
    durView_dbg_boardDesk(&b->desk);
    durView_score(b);
    printf("> Turn: winner=%s, attacker=%s, defender=%s, dealer=%s\n",
           (b->winner   == NULL) ? "Nemo" : (b->winner  ==&b->left)?"Left":"Right",
           (b->attacker == NULL) ? "Nemo" : (b->attacker==&b->left)?"Left":"Right",
           (b->defender == NULL) ? "Nemo" : (b->defender==&b->left)?"Left":"Right",
           (b->dealer   == NULL) ? "Nemo" : (b->dealer  ==&b->left)?"Left":"Right");
    durView_player(&b->left , b->desk.trump);
    durView_player(&b->right, b->desk.trump);
    durView_fight(b);
    durView_dbg_boardHistory(&b->history);
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