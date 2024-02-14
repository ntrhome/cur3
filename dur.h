#pragma once
#include "main.h"

typedef struct {
    int i;
} sMatch;





typedef struct {
    int max; //3
    int count;
    sMatch *match[3];
} sDur;

int durNewMatch(char *leftName, char *rightName);

