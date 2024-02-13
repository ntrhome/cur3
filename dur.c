#include <stdlib.h> //for srand, rand, malloc, free
#include <time.h>   //for srand
#include "dur.h"

static int suit[ed_cards] = { 0,0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,1, 2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,3 };
static int rank[ed_cards] = { 0,1,2,3,4,5,6,7,8, 0,1,2,3,4,5,6,7,8, 0,1,2,3,4,5,6,7,8, 0,1,2,3,4,5,6,7,8 };

static sMatch *newMatch() {
    sMatch *m = malloc(sizeof(sMatch));
    if (m != NULL) {
        for (int i = 0; i < ed_cards; ++i) m->desk.card[i] = i;
        m->left.place  = ep_left,  m->left.score  = 0;
        m->right.place = ep_right, m->right.score = 0;
        m->game = 0;
        m->winner = NULL;
        m->stage = es_newGame;
    }
    return m;
}
