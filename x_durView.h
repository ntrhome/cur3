#pragma once
#include "dur.h"

void durView(sMatch *m);
void durView_cmdWrong(char *s);

#ifdef DUR_DEBUG ////////////////////////////////////////////////////
    void durView_match(sMatch *m);
#endif //DUR_DEBUG
