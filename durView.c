#include "durView.h"

static void view_score(const sMatch *m) {
    printf(".> Score: (%d) [%d:%d].\n", m->game, m->left.score, m->right.score);
}

static void view_game(const sMatch *m) {
    printf(".> New game. Match=%p.\n", m);
    view_score(m);
}

void durView(sMatch *m) { //in durView.c only this f have not const sBoard (because b->stage)
    if (m != NULL) {
        switch (m->stage) {
            case es_gameView:
                view_game((const sMatch *) m);
                m->stage = es_fightModel;
                break;
//        case es_newFightView:
//            view_newFight((const sMatch *)m);
//            m->stage = es_attack;
//            break;
//        case es_attackView:
//            view_attack((const sMatch *)m);
//            m->stage = es_attackControl;
//            break;
//        case es_defendView:
//            durView_defend((const sBoard *)b);
//            b->stage = es_defendControl;
//            break;
//        case es_attackResultEnoughView:
//            printf(".> The fight is over by the attacker.\n");
//            b->stage = es_fightCloseAsDefended;
//            break;
//        case es_defendResultTookView:
//            printf(".> The fight is taken by the defender.\n");
//            b->stage = es_fightCloseAsTook;
//            break;
        }
    }
}

