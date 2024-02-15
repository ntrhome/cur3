#include "dur.h"
#include "main.h"
#include <stdlib.h> //for srand, rand, malloc, free

sMatch *iterator_sDur(void *set) {
    sDur *dur = (sDur *)set;

    return dur->match[0];
}

sMatch *durNewMatch() {
    sMatch *m = malloc(sizeof(sMatch));
    LOGIF(m == NULL)
    if (m != NULL) {
        m->stage = es_newGame;
        m->game = 0;
        for (int i = 0; i < ed_cards; ++i) m->desk.card[i] = i;
        m->left.place  = ep_left,  m->left.score  = 0;
        m->right.place = ep_right, m->right.score = 0;
        m->winner = NULL;
    }
}