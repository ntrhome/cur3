#ifndef DUR_DURVIEW_H
#define DUR_DURVIEW_H

#include "durModel.h"

void durView_step(dur_s_round *r);

#ifdef DUR_DEBUG
void dbgAdmin(dur_s_game *g);
void dbgBu(char *s, int i, int j);
#endif //DUR_DEBUG

#endif //DUR_DURVIEW_H
