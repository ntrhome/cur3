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
static char *places  = "LLRRa1a2a3a4a5a6d1d2d3d4d5d6PPDD";

static void durDbgViewBoard_Score(sBoard *b) {
    printf("> Score: [Left %d : %d Right].\n", b->left.score, b->right.score);
}

static void durViewPlayer_Desk(sPlayer *player, sDesk *d) {
    char *s = "";
    for (int i = 0; i < player->count; ++i) {
        int card = d->card[player->desk[i]];
        printf("%s%s%c%c%s", s, colorSuits[card / ed_ranks], ranks[card % ed_ranks], suits[card / ed_ranks], colorReset);
        s = ".";
    }
}
static void durViewPlayer(sPlayer *player, sBoard *b) {
    printf("> %s %s: (%d) [%s%c%s]\n[", (player == b->attacker) ? "Attacker" : "Defender", (player == &b->right) ? "Left" : "Right", player->count, colorSuits[b->desk.trump], suits[b->desk.trump], colorReset);
    for (int i = 0; i < player->count-1; ++i) { //suit-rank-sorter
        for (int j = i+1; j < player->count; ++j) {
            if(b->desk.card[player->desk[i]] > b->desk.card[player->desk[j]]) {
                int depot = player->desk[j];
                player->desk[j] = player->desk[i];
                player->desk[i] = depot;
            }
        }
    }
    durViewPlayer_Desk(player, &b->desk);
    printf("] /%s[", (player->count > 16 ) ? "\n" : " ");
    for (int i = 0; i < player->count-1; ++i) { //rank-suit-sorter
        for (int j = i+1; j < player->count; ++j) {
            if( b->desk.card[player->desk[i]] % ed_ranks > b->desk.card[player->desk[j]] % ed_ranks
                |
                b->desk.card[player->desk[i]] % ed_ranks == b->desk.card[player->desk[j]] % ed_ranks
                &
                b->desk.card[player->desk[i]] > b->desk.card[player->desk[j]]
            ) {
                int depot = player->desk[j];
                player->desk[j] = player->desk[i];
                player->desk[i] = depot;
            }
        }
    }
    durViewPlayer_Desk(player, &b->desk);
    printf("].\n");
}

static void durViewFight(sBoard *b) {
    if(b->attack.count == 0) {
        printf("First attack. ");
    } else {
        printf("> Fight: attack: [");
        char *s = "";
        for (int i = 0; i < b->attack.count; ++i) {
            int card = b->desk.card[b->attack.desk[i]];
            printf("%s%s%c%c%s", s, colorSuits[card / ed_ranks], ranks[card % ed_ranks], suits[card / ed_ranks],
                   colorReset);
            s = ".";
        }
        printf("]\n         defend: [");
        s = "";
        for (int i = 0; i < b->defend.count; ++i) {
            int card = b->desk.card[b->defend.desk[i]];
            printf("%s%s%c%c%s", s, colorSuits[card / ed_ranks], ranks[card % ed_ranks], suits[card / ed_ranks],
                   colorReset);
            s = ".";
        }
        printf("].\n");
    }
    printf("Your chosen card (e.g. '6c', 'Kd') or command (e.g. 'q'=quit): ");
}

#ifdef DUR_DEBUG   // [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [ [

static void durDbgViewBoard_Desk(sDesk *d) {
    printf("> Desk: (%2d) [%s%c%s]\n[", d->count, colorSuits[d->trump], suits[d->trump], colorReset);
    char *s = "";
    for(int i = 0; i < ed_cards; ++i){
        printf("%s%s%c%c%s", s, colorSuits[d->card[i] / ed_ranks], ranks[d->card[i] % ed_ranks], suits[d->card[i] / ed_ranks], colorReset);
        s = ".";
    }
    printf("].\n");
}

void durDbgViewBoard(sBoard *b) {
    printf("= = = = = = = = = = = = durDbgViewBoard = = = = = = = = = = = (match_id = %p)\n", b);
    durDbgViewBoard_Score(b);
    durDbgViewBoard_Desk(&b->desk);
    durViewPlayer(b->attacker, b);
    durViewPlayer((b->attacker == &b->right) ? &b->left : &b->right, b);
    printf("> Turn: winner->%s, attacker->%s\n",
           (b->winner == NULL)?"Nemo":(b->winner==&b->left)?"Left":"Right",
           (b->attacker == NULL)?"Nemo":(b->winner==&b->left)?"Left":"Right");
    durViewFight(b);
}

#endif //DUR_DEBUG // ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ] ]

void durView(sBoard *b) {
    printf("durView\n");
    //printf(" sizeof(sBoard)=%lu\n", sizeof(sBoard));
    switch (b->stage) {
        case es_attackView:
            printf("= = = = = = = = = = = = Attack:\n");
            durViewPlayer(b->attacker, b);
            durViewFight(b);
            break;
    }
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
