#include <stdlib.h> //for srand, rand, malloc, free

#include "dur.h"
sDur dur = { 0, 0};
bool durNewMatch(char *leftName, char *rightName) { //true=new match Ok
    if (dur.count < dur.max) {
        sMatch *m = malloc(sizeof(sMatch));
        if (m != NULL) {
            dur.match[dur.count++] = m;
            return true;
        }
    }
    LOG
    return false;
}

