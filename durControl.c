#include <stdio.h>
#include "durModel.h"
#include "durView.h" //ideal - all output make by durView
#include "durControl.h"

static int durControl_inputCardOrCommand(char *msg) { //return card [0..ed_cards) or command (es > 1000)
    printf("%s", msg);
    fflush(stdout);
    fflush (stdin);
    char s[2+1];
    int i = scanf("%2s", s);
    //printf("i=%d, s=%s, s[0]=%d, s[1]=%d, s[2]=%d.\n", i, s, s[0],s[1],s[2]); //dbg
    if (i != 1) return es_cmd_wrong;
    if (s[0] > 96 && s[0] < 123) s[0] -= ('a' - 'A'); //toupper(s[0])
    //command
    if (s[1] == 0) {
        if(s[0] == 'Q') return es_cmd_quit;
        if(s[0] == 'T') return es_cmd_take;
        return es_cmd_wrong;
    }
    //card
    int rank = 0;
    for (; rank < ed_ranks; ++rank) {
        if (s[0] == ranks[rank]) break;
    }
    if (rank == ed_ranks) return es_cmd_wrong;
    if (s[1] > 64 && s[1] < 91)  s[1] += ('a' - 'A'); //tolower(s[0])
    int suit = 0;
    for (; suit < ed_suits; ++suit) {
        if (s[1] == suits[suit]) break;
    }
    if (suit == ed_suits) return es_cmd_wrong;
    return suit * ed_ranks + rank; //card
}

static es durControl_attack(sField *f) {
    int card = durControl_inputCardOrCommand("Choose a card (e.g. '6s', 'Ah') or command ('q'-ec_quit): ");
    switch (card) {
        case es_cmd_quit:
            return es_cmd_quit;
        case es_cmd_wrong:
        case es_cmd_take:
            printf("Bad stuff. Repeat.\n");
            return es_attackControl;
        default: //card
    }
    printf("got card %d\n", card); //dbg
    int flAcceptable = 0;
    if (f->attack.count) { //not first attack
        int rank = card % ed_ranks;
        for (int i = 0; i < f->attack.count; ++i) {
            if (f->attack.card[i] % ed_ranks == rank) {
                flAcceptable = 1;
                break;
            }
        }
        if (!flAcceptable) {
            for (int i = 0; i < f->defend.count; ++i) {
                if (f->defend.card[i] % ed_ranks == rank) {
                    flAcceptable = 1;
                    break;
                }
            }
        }
    } else { //first attack
        for (int i = 0; i < f->player->count; ++i) {
            if (f->player->card[i] == card) {
                flAcceptable = 1;
                break;
            }
        }
    }
    if (!flAcceptable) {
        printf("Bad choice of card. Repeat.\n");
        return es_attackControl;
    }
    //good card for attack
    for (int i = 0; i < f->player->count; ++i) { //withdraw from the player
        if (f->player->card[i] == card) {
            f->player->card[i] = f->player->card[--f->player->count];
            break;
        }
    }
    f->attack.card[f->attack.count++] = card;
    //todo: надо внести в историю, но она недоступна(слить field в board)
    //поскольку заведен structField:player, то лучше уже defender.
    //слить attack c defend (один массив), и добавить туда некстФайт.
    return es_attackResult;
}

void durControl(sBoard *b) {
    printf("durControl\n");
    switch (b->stage) {
        case es_attackControl:
            b->stage = durControl_attack(&b->field);
            break;
    }
}
